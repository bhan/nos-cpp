#include <ctype.h>
#include <clang-c/Index.h>
#include <iostream>
#include <string>
#include <ctemplate/template.h>

#include "AstParser.hpp"
#include "ClassRep.hpp"
#include "FunctionRep.hpp"
#include "FieldRep.hpp"

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

void AstParser::printClassTemplate(std::ostream& os, ClassRep& classRep) {
    ctemplate::TemplateDictionary dict("class");

    dict.SetValue("CLASS_NAME", classRep.getName());

    std::string classUpper(classRep.getName());
    std::for_each(classUpper.begin(), classUpper.end(), convert());
    dict.SetValue("CLASS_UPPERCASE", classUpper);

    for (auto &funcRep : classRep.getFunctions()) {
        if (funcRep->isConstructor()) {
            auto ctorDict = dict.AddSectionDictionary("CTORS");
            ctorDict->SetValue("CTOR_ARGS_WITH_TYPES", funcRep->getArgNamesWithTypes());
            ctorDict->SetValue("CTOR_ARGS", funcRep->getArgNames());
        } else {
            auto funcDict = dict.AddSectionDictionary("METHOD_IMPLS");
            funcDict->SetValue("METHOD_RET_TYPE", funcRep->getReturnType());
            funcDict->SetValue("METHOD_NAME", funcRep->getName());
            funcDict->SetValue("METHOD_ARGS", funcRep->getArgNames());
            funcDict->SetValue("METHOD_ARGS_TYPES", funcRep->getArgTypes());
            funcDict->SetValue("METHOD_ARGS_WITH_TYPES", funcRep->getArgNamesWithTypes());
        }
    }

    for (auto &fieldRep : classRep.getPublicFields()) {
        auto fieldDict = dict.AddSectionDictionary("PUBLIC_FIELDS");
        fieldDict->SetValue("FIELD_NAME", fieldRep->getName());
        fieldDict->SetValue("FIELD_TYPE", fieldRep->getType());
        fieldDict->SetValue("FIELD_NAME_CAMEL_CASE", fieldRep->getNameCamelCase()->c_str());
    }

    std::string output;
    ctemplate::ExpandTemplate(templateFilename_, ctemplate::DO_NOT_STRIP, &dict, &output);
    os << output;
}

void AstParser::printTemplates(std::ostream& os, std::vector<ClassRep*>& classes) {
    for (auto &classRep : classes) {
        if (classRep->isNetObj()) {
            printClassTemplate(os, *classRep);
        }
    }
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

void AstParser::setFile(std::string filename) {
    inputFilename_ = filename;
}

void AstParser::setTemplate(std::string filename) {
    templateFilename_ = filename;
}

bool AstParser::generateOutput(std::ostream& output, std::ostream& error) {
    auto index = clang_createIndex(0, 0);
    auto transUnit = clang_parseTranslationUnit(index, inputFilename_.c_str(), NULL, 0, 0, 0, CXTranslationUnit_None);

    if (checkForOutputErrors(error, transUnit)) {
        // TODO: hook up to clang in makefile so we can stop ignoring this
        //return false;
    }

    std::unique_ptr<std::vector<ClassRep*> > classes(new std::vector<ClassRep*>());

    auto cursor = clang_getTranslationUnitCursor(transUnit);
    clang_visitChildren(cursor, classPrinter, classes.get());

    clang_disposeTranslationUnit(transUnit);
    clang_disposeIndex(index);

    printTemplates(output, *classes.get());

    return true;
}
