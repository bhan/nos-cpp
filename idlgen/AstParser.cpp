#include <clang-c/Index.h>
#include <iostream>
#include <string>
#include <ctemplate/template.h>

#include "ClassRep.hpp"
#include "FunctionRep.hpp"

static enum CXChildVisitResult functionPrinter(CXCursor cursor, CXCursor, CXClientData parserState) {
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
    }
    return CXChildVisit_Continue;
}

static enum CXChildVisitResult classPrinter(CXCursor cursor, CXCursor, CXClientData data) {
    auto classes = static_cast<std::vector<ClassRep*>* >(data);
    if (cursor.kind == CXCursorKind::CXCursor_ClassDecl) {
        auto str = clang_getCursorDisplayName(cursor);
        std::unique_ptr<ClassRep> classRep(new ClassRep(clang_getCString(str)));
        clang_disposeString(str);

        clang_visitChildren(cursor, functionPrinter, classRep.get());

        classes->push_back(classRep.release());
    }

    return CXChildVisit_Continue;
}

static void printClassTemplate(ClassRep& classRep) {
    std::cout << classRep << std::endl;
}

static void printTemplates(std::vector<ClassRep*>& classes) {
    for (auto &classRep : classes) {
        printClassTemplate(*classRep);
    }
}

static bool checkForOutputErrors(CXTranslationUnit tu) {
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

        std::cerr << clang_getCString(diagCategoryStr) << "(" << diagSeverity <<
                    "): " << clang_getCString(diagText) << std::endl;
    }

    return fatalError;
}

static void usage(const char* executable_name) {
    std::cout << "Usage: "  << executable_name << " <source_file> [<source_file> ...]" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        usage(argv[0]);
        return 1;
    }

    auto index = clang_createIndex(0, 0);
    auto transUnit = clang_parseTranslationUnit(index, 0, argv, argc, 0, 0, CXTranslationUnit_None);

    if (checkForOutputErrors(transUnit)) {
        std::cerr << "WARNING: Compilation unit has errors. Please fix them!" << std::endl;
    }

    std::unique_ptr<std::vector<ClassRep*> > classes(new std::vector<ClassRep*>());

    auto cursor = clang_getTranslationUnitCursor(transUnit);
    clang_visitChildren(cursor, classPrinter, classes.get());

    clang_disposeTranslationUnit(transUnit);
    clang_disposeIndex(index);

    printTemplates(*classes.get());

    return 0;
}
