#ifndef _AST_PARSER_HPP
#define _AST_PARSER_HPP

#include <iostream>
#include <vector>

#include <clang-c/Index.h>
#include "ctemplate/template.h"

class ClassRep;

class AstParser {
  public:
    AstParser() {};
    ~AstParser() {};

    void setTemplate(std::string filename);
    void setRegistrar(std::string filename);
    void setOutputDirectory(std::string path);
    void addFile(std::string filename);

    bool generateOutput(std::ostream& output, std::ostream& error);

  private:
    std::string templateFilename_;
    std::string registrarFilename_;
    std::string outputDirectory_;
    std::vector<std::string> inputFilenames_;

    void populateClassDictionary(ctemplate::TemplateDictionary* dict, ClassRep* classRep);
    void printClassTemplate(std::ostream& os, ClassRep& classRep);
    void printRegistrarTemplate(std::ostream& os, ctemplate::TemplateDictionary& dict);
    bool printTemplates(std::ostream& os, std::vector<ClassRep*>& classes);
    bool checkForOutputErrors(std::ostream& output, CXTranslationUnit tu);
};

#endif /* _AST_PARSER_HPP */
