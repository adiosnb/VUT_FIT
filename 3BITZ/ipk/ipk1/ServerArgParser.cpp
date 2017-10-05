#include "ServerArgParser.h"

/**
 * Parsing line arguments for server.
 * @param argc number of arguments
 * @param argv list of C strings woth line arguments
 * @return  struct server_arg_t with parsed values
 */
ServerArgParser::server_arg_t ServerArgParser::parse_arg(int argc, char **argv) {
    server_arg_t arguments;
    std::string line_arg;

    if (argc > 1){
        if (argc > 5){
            throw "Too much arguments";
        }

        for (int i = 1; i < argc ; i ++){
            line_arg = argv[i];
            if (line_arg == "-p"){
                if (argc > i+1) {
                    line_arg = argv[++i];
                    arguments.listen_port = stoi(line_arg);
                }
                else throw "Missing argument";
            }
            else if (line_arg == "-r"){
                if (argc > i+1) {
                    arguments.root_folder = argv[++i];
                }
                else throw "Missing argument";
            }
            else {
                throw "Wrong arguments";
            }

        }
    }

    return arguments;
}