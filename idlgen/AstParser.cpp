#include <clang-c/Index.h>
#include <iostream>
#include <string>

#include "FunctionRep.hpp"

enum CXChildVisitResult functionPrinter(CXCursor cursor, CXCursor, CXClientData parserState) {
    auto access = clang_getCXXAccessSpecifier(cursor);
    if (access != CX_CXXAccessSpecifier::CX_CXXPublic) {
        return CXChildVisit_Continue;
    }

    if (cursor.kind == CXCursorKind::CXCursor_CXXMethod || cursor.kind == CXCursorKind::CXCursor_Constructor) {
        auto str = clang_getCursorSpelling(cursor);
        std::string funcName(clang_getCString(str));
        FunctionRep funcRep(funcName, cursor.kind == CXCursor_Constructor);
        clang_disposeString(str);

        auto retType = clang_getCursorType(cursor);
        auto retTypeCXStr = clang_getTypeSpelling(retType);
        std::string retTypeStr(clang_getCString(retTypeCXStr));
        if (retTypeStr.find(' ') != std::string::npos) {
            funcRep.setReturnType(retTypeStr.substr(0, retTypeStr.find(' ')));
        } else {
            funcRep.setReturnType(retTypeStr);
        }
        clang_disposeString(retTypeCXStr);

        int numArgs = clang_Cursor_getNumArguments(cursor);
        for (int i = 0; i < numArgs; i++) {
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

            funcRep.addArgmument(argTypeStr, argName);
        }
        std::cout << (funcRep.isConstructor() ? "  Constructor: " : "  Function: ") << funcRep << std::endl;
    } else if (cursor.kind == CXCursorKind::CXCursor_CXXBaseSpecifier) {
        auto str = clang_getCursorDisplayName(cursor);
        std::cout << "  Base: " << clang_getCString(str) << std::endl;
        clang_disposeString(str);
    }
    return CXChildVisit_Continue;
}

enum CXChildVisitResult classPrinter(CXCursor cursor, CXCursor, CXClientData) {
    if (cursor.kind == CXCursorKind::CXCursor_ClassDecl) {
        CXString str = clang_getCursorDisplayName(cursor);
        std::cout << "Class: " << clang_getCString(str) << std::endl;
        clang_disposeString(str);

        clang_visitChildren(cursor, functionPrinter, nullptr);
    }

    return CXChildVisit_Continue;
}

bool checkForOutputErrors(CXTranslationUnit tu) {
    bool fatalError = false;

    unsigned int numDiagnostics = clang_getNumDiagnostics(tu);
    for (unsigned int diagIdx = 0; diagIdx < numDiagnostics; diagIdx++) {
        CXDiagnostic diag = clang_getDiagnostic(tu, diagIdx);

        CXString diagCategoryStr = clang_getDiagnosticCategoryText(diag);
        CXString diagText = clang_getDiagnosticSpelling(diag);

        CXDiagnosticSeverity diagSeverity = clang_getDiagnosticSeverity(diag);
        if (diagSeverity >= CXDiagnostic_Fatal) {
            fatalError = true;
        }

        std::cerr << clang_getCString(diagCategoryStr) << "(" << diagSeverity <<
                    "): " << clang_getCString(diagText) << std::endl;
    }

    return fatalError;
}

void usage(const char* executable_name) {
    std::cout << "Usage: "  << executable_name << " <source_file> [<source_file> ...]" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        usage(argv[0]);
        return 1;
    }

    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit transUnit = clang_parseTranslationUnit(index, 0, argv, argc, 0, 0, CXTranslationUnit_None);

    if (checkForOutputErrors(transUnit)) {
        std::cerr << "WARNING: Compilation unit has errors. Please fix them!" << std::endl;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(transUnit);
    clang_visitChildren(cursor, classPrinter, nullptr);

    clang_disposeTranslationUnit(transUnit);
    clang_disposeIndex(index);

    return 0;
}
