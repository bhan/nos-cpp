#include <algorithm>
#include <ctype.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include <clang-c/Index.h>
#include <ctemplate/template.h>

#include "AstParser.hpp"
#include "ClassRep.hpp"
#include "FunctionRep.hpp"
#include "FieldRep.hpp"

AstParser::AstParser() : clangOptionsArray_(nullptr), clangOptionsArrayCount_(0) {
}

AstParser::~AstParser() {
    if (clangOptionsArray_ != nullptr) {
        delete[] clangOptionsArray_;
        clangOptionsArray_ = nullptr;
        clangOptionsArrayCount_ = 0;
    }
}

static enum CXChildVisitResult classMemberPrinter(CXCursor cursor, CXCursor, CXClientData parserState) {
    auto classRep = static_cast<ClassRep*>(parserState);

    auto access = clang_getCXXAccessSpecifier(cursor);
    if (access != CX_CXXAccessSpecifier::CX_CXXPublic) {
        return CXChildVisit_Continue;
    }

    if (cursor.kind == CXCursorKind::CXCursor_CXXMethod || cursor.kind == CXCursorKind::CXCursor_Constructor) {
        auto str = clang_getCursorSpelling(cursor);
        std::string funcName(clang_getCString(str));
        std::unique_ptr<FunctionRep> funcRep(new FunctionRep(funcName, cursor.kind == CXCursor_Constructor));
        clang_disposeString(str);

        auto retType = clang_getCursorType(cursor);
        auto retTypeCXStr = clang_getTypeSpelling(retType);
        std::string retTypeStr(clang_getCString(retTypeCXStr));
        if (retTypeStr.find(' ') != std::string::npos) {
            funcRep->setReturnType(retTypeStr.substr(0, retTypeStr.find(' ')));
        } else {
            funcRep->setReturnType(retTypeStr);
        }
        clang_disposeString(retTypeCXStr);

        auto numArgs = clang_Cursor_getNumArguments(cursor);
        for (decltype(numArgs) i = 0; i < numArgs; i++) {
            CXCursor argCursor = clang_Cursor_getArgument(cursor, i);

            // ret type
            auto argType = clang_getCursorType(argCursor);
            auto typeStr = clang_getTypeSpelling(argType);
            std::string argTypeStr(clang_getCString(typeStr));
            clang_disposeString(typeStr);

            // arg name
            auto argStr = clang_getCursorSpelling(argCursor);
            std::string argName(clang_getCString(argStr));
            clang_disposeString(argStr);

            funcRep->addArgmument(argTypeStr, argName);
        }

        classRep->addFunction(funcRep.release());
    } else if (cursor.kind == CXCursorKind::CXCursor_CXXBaseSpecifier) {
        auto str = clang_getCursorDisplayName(cursor);
        classRep->addBaseClass(clang_getCString(str));
        clang_disposeString(str);
    } else if (cursor.kind == CXCursorKind::CXCursor_FieldDecl) {
        auto str = clang_getCursorDisplayName(cursor);
        std::unique_ptr<FieldRep> field(new FieldRep(clang_getCString(str)));
        clang_disposeString(str);

        auto fieldType = clang_getCursorType(cursor);
        auto retTypeCXStr = clang_getTypeSpelling(fieldType);
        field->setType(clang_getCString(retTypeCXStr));
        clang_disposeString(retTypeCXStr);

        classRep->addPublicField(field.release());
    }

    return CXChildVisit_Continue;
}

static enum CXChildVisitResult classPrinter(CXCursor cursor, CXCursor, CXClientData data) {
    auto classes = static_cast<std::vector<ClassRep*>* >(data);
    if (cursor.kind == CXCursorKind::CXCursor_ClassDecl) {
        auto str = clang_getCursorDisplayName(cursor);
        std::unique_ptr<ClassRep> classRep(new ClassRep(clang_getCString(str)));
        clang_disposeString(str);

        clang_visitChildren(cursor, classMemberPrinter, classRep.get());

        classes->push_back(classRep.release());
    }

    return CXChildVisit_Continue;
}

struct convert {
    void operator()(char& c) {
        c = toupper(static_cast<unsigned char>(c));
    }
};

void AstParser::populateClassDictionary(ctemplate::TemplateDictionary* dict, ClassRep* classRep) {
    dict->SetValue("CLASS_NAME", classRep->getName());

    std::string classUpper(classRep->getName());
    std::for_each(classUpper.begin(), classUpper.end(), convert());
    dict->SetValue("CLASS_UPPERCASE", classUpper);

    for (auto &funcRep : classRep->getFunctions()) {
        if (funcRep->isConstructor()) {
            auto ctorDict = dict->AddSectionDictionary("CTORS");
            ctorDict->SetValue("CTOR_ARGS_WITH_TYPES", funcRep->getArgNamesWithTypes());
            ctorDict->SetValue("CTOR_ARGS", funcRep->getArgNames());
        } else {
            auto funcDict = dict->AddSectionDictionary(funcRep->getReturnType() == "void" ? "VOID_METHOD_IMPLS" : "METHOD_IMPLS");
            funcDict->SetValue("METHOD_RET_TYPE", funcRep->getReturnType());
            funcDict->SetValue("METHOD_NAME", funcRep->getName());
            funcDict->SetValue("METHOD_ARGS", funcRep->getArgNames());
            funcDict->SetValue("METHOD_ARGS_TYPES", funcRep->getArgTypes());
            funcDict->SetValue("METHOD_ARGS_WITH_TYPES", funcRep->getArgNamesWithTypes());
        }
    }

    for (auto &fieldRep : classRep->getPublicFields()) {
        auto fieldDict = dict->AddSectionDictionary("PUBLIC_FIELDS");
        fieldDict->SetValue("FIELD_NAME", fieldRep->getName());
        fieldDict->SetValue("FIELD_TYPE", fieldRep->getType());
        fieldDict->SetValue("FIELD_NAME_CAMEL_CASE", fieldRep->getNameCamelCase()->c_str());
    }
}

void AstParser::printClassTemplate(std::ostream& os, ClassRep& classRep) {
    ctemplate::TemplateDictionary dict("class");

    populateClassDictionary(&dict, &classRep);

    std::string output;
    ctemplate::ExpandTemplate(templateFilename_, ctemplate::DO_NOT_STRIP, &dict, &output);
    os << output;
}

void AstParser::printRegistrarTemplate(std::ostream& os, ctemplate::TemplateDictionary& dict) {
    std::string output;
    ctemplate::ExpandTemplate(registrarFilename_, ctemplate::DO_NOT_STRIP, &dict, &output);
    os << output;
}

void AstParser::printClientRegistrarTemplate(std::ostream& os, ctemplate::TemplateDictionary& dict) {
    std::string output;
    ctemplate::ExpandTemplate(clientRegistrarFilename_, ctemplate::DO_NOT_STRIP, &dict, &output);
    os << output;
}

bool AstParser::printTemplates(std::ostream& error, std::vector<ClassRep*>& classes) {
    ctemplate::TemplateDictionary dict("registrar");

    for (auto &classRep : classes) {
        if (classRep->isNetObj()) {
            if (!registrarFilename_.empty() || !clientRegistrarFilename_.empty()) {
                auto classDict = dict.AddSectionDictionary("CLASSES");
                populateClassDictionary(classDict, classRep);
            }

            std::ofstream outputFile;
            outputFile.open(outputDirectory_ + "/" + classRep->getName() + "Generated.hpp", std::ios::out | std::ios::trunc | std::ios::binary);
            printClassTemplate(outputFile, *classRep);
            outputFile.close();
        }
    }

    if (!registrarFilename_.empty()) {
        std::ofstream outputFile;
        outputFile.open(outputDirectory_ + "/Registrar.cpp", std::ios::out | std::ios::trunc | std::ios::binary);
        printRegistrarTemplate(outputFile, dict);
        outputFile.close();
    }
    if (!clientRegistrarFilename_.empty()) {
        std::ofstream outputFile;
        outputFile.open(outputDirectory_ + "/ClientRegistrar.cpp", std::ios::out | std::ios::trunc | std::ios::binary);
        printClientRegistrarTemplate(outputFile, dict);
        outputFile.close();
    }

    return true;
}

bool AstParser::checkForOutputErrors(std::ostream& os, CXTranslationUnit tu) {
    bool fatalError = false;

    auto numDiagnostics = clang_getNumDiagnostics(tu);
    for (decltype(numDiagnostics) diagIdx = 0; diagIdx < numDiagnostics; diagIdx++) {
        CXDiagnostic diag = clang_getDiagnostic(tu, diagIdx);

        auto diagCategoryStr = clang_getDiagnosticCategoryText(diag);
        auto diagText = clang_getDiagnosticSpelling(diag);

        auto diagSeverity = clang_getDiagnosticSeverity(diag);
        if (diagSeverity >= CXDiagnostic_Fatal) {
            fatalError = true;
        }

        os << clang_getCString(diagCategoryStr) << "(" << diagSeverity <<
                    "): " << clang_getCString(diagText) << std::endl;
    }

    return fatalError;
}

void AstParser::addFile(std::string filename) {
    inputFilenames_.push_back(filename);
}

void AstParser::addClangOption(std::string option) {
    clangOptions_.push_back(option);
}

void AstParser::setTemplate(std::string filename) {
    templateFilename_ = filename;
}

void AstParser::setRegistrar(std::string filename) {
    registrarFilename_ = filename;
}

void AstParser::setClientRegistrar(std::string filename) {
    clientRegistrarFilename_ = filename;
}
void AstParser::setOutputDirectory(std::string dirname) {
    outputDirectory_ = dirname;
}

void AstParser::buildClangOptionsArray() {
    clangOptionsArray_ = new const char*[clangOptions_.size()];
    for (size_t i = 0; i < clangOptions_.size(); ++i) {
        clangOptionsArray_[i] = clangOptions_[i].c_str();
    }
    clangOptionsArrayCount_ = clangOptions_.size();
}

const char *const *AstParser::getClangOptions() const {
    return clangOptionsArray_;
}

size_t AstParser::getClangOptionsCount() const {
    return clangOptionsArrayCount_;
}

bool AstParser::generateOutput(std::ostream& output, std::ostream& error) {
    std::unique_ptr<std::vector<ClassRep*> > classes(new std::vector<ClassRep*>());

    buildClangOptionsArray();

    auto index = clang_createIndex(0, 0);
    for (auto &filename : inputFilenames_) {
        auto transUnit = clang_parseTranslationUnit(index, filename.c_str(), getClangOptions(), getClangOptionsCount(), 0, 0, CXTranslationUnit_None);

        if (checkForOutputErrors(error, transUnit)) {
            // TODO: hook up to clang in makefile so we can stop ignoring this
            //return false;
        }

        auto cursor = clang_getTranslationUnitCursor(transUnit);
        clang_visitChildren(cursor, classPrinter, classes.get());

        clang_disposeTranslationUnit(transUnit);
    }
    clang_disposeIndex(index);

    return printTemplates(error, *classes.get());
}
