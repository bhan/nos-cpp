#include <clang-c/Index.h>
#include <iostream>

enum CXChildVisitResult visitorFunc(CXCursor cursor, CXCursor parent, CXClientData data) {
    CXType type = clang_getCursorType(cursor);
    CXString str = clang_getTypeKindSpelling(type.kind);
    std::cout << "Type: " << clang_getCString(str) << std::endl;
    clang_disposeString(str);

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
    clang_visitChildren(cursor, visitorFunc, nullptr);

    clang_disposeTranslationUnit(transUnit);
    clang_disposeIndex(index);

    return 0;
}
