#include <ctype.h>
#include <iostream>
#include <string>

#include "AstParser.hpp"
#include "optionparser.h"

struct Arg : public option::Arg {
    static void printError(const char* msg1, const option::Option& opt, const char* msg2) {
        std::cerr << "ERROR: " << msg1 << opt.name << msg2 << std::endl;
    }

    static option::ArgStatus Required(const option::Option& option, bool msg) {
        if (option.arg != NULL && option.arg[0] != '\0') {
            return option::ARG_OK;
        }

        if (msg) {
            printError("Option '", option, "' requires a single value.\n");
        }

        return option::ARG_ILLEGAL;
    }
};

enum optionIndex { UNKNOWN, HELP, TEMPLATE, REG_TEMPLATE, CLANG_OPTIONS, OUTPUT_DIR };
const option::Descriptor usage[] = {
    {UNKNOWN,       0,  "", "",              option::Arg::None,
        "USAGE: idlgen [options] <file> [<file> ...]\n\n"
        "Options:" },
    {HELP,          0,  "", "",              option::Arg::None,
        "  --help  \tPrint usage and exit." },
    {TEMPLATE,      0, "t", "class-template",Arg::Required,
        "  --class-template, -t  \tUse given template file for each class." },
    {REG_TEMPLATE,  0, "r", "registrar",     Arg::Required,
        "  --registrar, -r  \tUse given template file for the registrar output." },
    {CLANG_OPTIONS, 0, "c", "clang-options", option::Arg::Optional,
        "  --clang-options, -c  \tUse given clang options."},
    {OUTPUT_DIR,    0, "o", "output-dir",    Arg::Required,
        "  --output-dir, -o  \tUse the given directory for output files." },
    {UNKNOWN,       0, "",  "",              option::Arg::None,
        "\nExamples:\n"
        "  idlgen -t mygenerator.hpp.tpl -o output MyHeader.hpp\n"},
    {0,0,0,0,0,0}
};

int main(int argc, char* argv[]) {
    // skip the program name if present
    if (argc > 0) {
        --argc;
        ++argv;
    }

    option::Stats stats(usage, argc, argv);
    auto options = new option::Option[stats.options_max];
    auto buffer = new option::Option[stats.buffer_max];
    option::Parser parse(usage, argc, argv, options, buffer);

    if (parse.error()) {
        std::cerr << "Parse error" << std::endl;
        return 1;
    }

    if (options[HELP] || argc == 0) {
        option::printUsage(std::cout, usage);
        return 0;
    }

    if (options[TEMPLATE].count() != 1) {
        std::cerr << "Exactly one class template must be specified." << std::endl;
        return 1;
    } else if (options[OUTPUT_DIR].count() != 1) {
        std::cerr << "Exactly one output directory must be specified." << std::endl;
        return 1;
    } else if (options[REG_TEMPLATE].count() > 1 || options[REG_TEMPLATE].last()->arg == nullptr) {
        std::cerr << "Only one registrar template may be specified." << std::endl;
        return 1;
    } else if (parse.nonOptionsCount() < 1) {
        std::cerr << "Source file(s) must be specified: " << parse.nonOptionsCount() << " found." << std::endl;
        return 1;
    }

    AstParser generator;

    generator.setTemplate(options[TEMPLATE].last()->arg);
    generator.setOutputDirectory(options[OUTPUT_DIR].last()->arg);
    if (options[REG_TEMPLATE].count() == 1) {
        generator.setRegistrar(options[REG_TEMPLATE].last()->arg);
    }

    for (int i = 0; i < parse.nonOptionsCount(); ++i) {
        generator.addFile(parse.nonOption(i));
    }

    return generator.generateOutput(std::cout, std::cerr);
}
