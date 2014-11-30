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

enum optionIndex { UNKNOWN, HELP, TEMPLATE, CLANG_OPTIONS };
const option::Descriptor usage[] = {
    {UNKNOWN,       0,  "", "",              option::Arg::None,
        "USAGE: idlgen [options] <file>\n\n"
        "Options:" },
    {HELP,          0,  "", "",              option::Arg::None,
        "  --help  \tPrint usage and exit." },
    {TEMPLATE,      0, "t", "template",      Arg::Required,
        "  --template, -t  \tUse given template file." },
    {CLANG_OPTIONS, 0, "o", "clang-options", option::Arg::Optional,
        "  --clang-options, -o  \tUse given clang options."},
    {UNKNOWN,       0, "",  "",              option::Arg::None,
        "\nExamples:\n"
        "  idlgen -t mygenerator.hpp.tpl MyHeader.hpp\n"},
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
        std::cerr << "Exactly one template must be specified: " << options[TEMPLATE].count() << " found." << std::endl;
        return 1;
    } else if (parse.nonOptionsCount() != 1) {
        std::cerr << "Exactly one source file must be specified: " << parse.nonOptionsCount() << " found." << std::endl;
        for (int i = 0; i < parse.nonOptionsCount(); ++i) {
            std::cerr << "  found: " << parse.nonOption(i) << std::endl;
        }

        return 1;
    }


    AstParser generator;

    generator.setTemplate(options[TEMPLATE].last()->arg);
    generator.setFile(parse.nonOption(0));

    return generator.generateOutput(std::cout, std::cerr);
}
