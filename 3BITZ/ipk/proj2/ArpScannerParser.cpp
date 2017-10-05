#include "ArpScannerParser.h"

namespace Parser {
    arg_t parse_arguments(int argc, char **argv) {
        arg_t parser_arguments;

        if (argc != 5) throw "Wrong number of arguments";

        for (int i = 1 ; i < argc ; i++){
            std::string arg = argv[i];
            if (arg == "-i"){
                parser_arguments.interface = argv[++i];
                continue;
            }
            else if (arg == "-f"){
                parser_arguments.out_file = argv[++i];
                continue;
            }
            else throw "Arg Parser error";
        }

        return parser_arguments;
    }

    void check_arguments(arg_t arguments) {
        if (arguments.interface == ""){
            throw "No interface entered";
        }
        if (arguments.out_file == ""){
            throw "No out xml file entered";
        }
    }
}
