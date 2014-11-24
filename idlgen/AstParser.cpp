#include <clang-c/Index.h>
#include <iostream>

enum CXChildVisitResult functionPrinter(CXCursor cursor, CXCursor, CXClientData) {
    CX_CXXAccessSpecifier access = clang_getCXXAccessSpecifier(cursor);
    if (access != CX_CXXAccessSpecifier::CX_CXXPublic) {
        return CXChildVisit_Continue;
    }

    if (cursor.kind == CXCursorKind::CXCursor_CXXMethod) {
        CXString str = clang_getCursorDisplayName(cursor);
        std::cout << "  Function: " << clang_getCString(str) << std::endl;
        clang_disposeString(str);
    } else if (cursor.kind == CXCursorKind::CXCursor_Constructor) {
        CXString str = clang_getCursorDisplayName(cursor);
        std::cout << "  Constructor: " << clang_getCString(str) << std::endl;
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

    CXCursor cursor = clang_getTranslationUnitCursor(transUnit);
    clang_visitChildren(cursor, classPrinter, nullptr);

    clang_disposeTranslationUnit(transUnit);
    clang_disposeIndex(index);

    return 0;
}
