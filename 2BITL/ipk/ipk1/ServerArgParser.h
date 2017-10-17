#ifndef SERVER_ARG_PARSER
#define SERVER_ARG_PARSER

#include <iostream>
namespace ServerArgParser {

    typedef struct {
        int listen_port = 6677;
        std::string root_folder = "";
    } server_arg_t;

    server_arg_t parse_arg(int argc, char **argv);

    /**
     * Checks if server arguments are correct.
     * @param arguments Parsed server arguments.
     */
    void check_arg(server_arg_t arguments);
}

#endif //SERVER_ARG_PARSER