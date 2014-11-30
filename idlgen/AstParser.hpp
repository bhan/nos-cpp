#ifndef _AST_PARSER_HPP
#define _AST_PARSER_HPP

#include <clang-c/Index.h>

#include <iostream>
#include <vector>

class ClassRep;

class AstParser {
  public:
    AstParser() {};
    ~AstParser() {};

    void setTemplate(std::string filename);
    void setFile(std::string filename);

    bool generateOutput(std::ostream& output, std::ostream& error);

  private:
    std::string templateFilename_;
    std::string inputFilename_;

    void printClassTemplate(std::ostream& os, ClassRep& classRep);
    void printTemplates(std::ostream& os, std::vector<ClassRep*>& classes);
    bool checkForOutputErrors(std::ostream& output, CXTranslationUnit tu);
};

#endif /* _AST_PARSER_HPP */
