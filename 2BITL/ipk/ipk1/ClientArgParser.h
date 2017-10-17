#ifndef CLIENT_ARG_PARSER
#define CLIENT_ARG_PARSER

#include <iostream>
#include "Connection.h"

namespace ClientArgParser {

    typedef struct {
        conn::commands cmd;
        std::string remote_url;
        std::string remote_path;
        std::string local_path;
        int remote_port = -1;
    } client_arg_t;

    /**
     * Parsing clientargument.
     * @param argc Number fo argument.
     * @param argv List of c strings including arguments.
     * @return Structure of client arguments.
     */
    client_arg_t parse_arg(int argc, char **argv);

    /**
     * Split path to destination file/folder from URL address.
     * @param arguments_struct There is stored path and url.
     * @param input URK address
     */
    void parse_url_and_path(client_arg_t &arguments_struct, std::string input);

    /**
     * Checks if client arguments are correct.
     * @param arguments Parsed client arguments.
     */
    void check_arg(client_arg_t arguments);



}
#endif //CLIENT_ARG_PARSER