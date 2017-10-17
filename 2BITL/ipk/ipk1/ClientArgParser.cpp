#include "ClientArgParser.h"
#include "Connection.h"
#include <iostream>

ClientArgParser::client_arg_t ClientArgParser::parse_arg(int argc, char **argv) {
    ClientArgParser::client_arg_t arguments;

    if(argc > 1 and argc < 5) {
        std::string command = argv[1];

        if (command == "del") arguments.cmd = conn::del;
        else if (command == "get") arguments.cmd = conn::get;
        else if (command == "put") arguments.cmd = conn::put;
        else if (command == "lst") arguments.cmd = conn::lst;
        else if (command == "mkd") arguments.cmd = conn::mkd;
        else if (command == "rmd") arguments.cmd = conn::rmd;
        else throw "Error: wrong input function";

        std::string second_arg = argv[2];
        ClientArgParser::parse_url_and_path(arguments, second_arg);
        std::string local_path ;
        if (argc == 4) local_path = argv[3];
        else local_path = "";

        arguments.local_path = local_path;

    }
    else {
        throw "Wrong number of parameters";
    }

    return arguments;
}

void ClientArgParser::check_arg(client_arg_t arguments) {

    if (arguments.cmd == conn::unknown){
        throw "Error";
    }

    if (arguments.remote_url == "") {
        throw "Error";
    }

}

void ClientArgParser::parse_url_and_path(client_arg_t &arguments_struct, std::string input) {

    std::string http_prefix = "http://";
    std::string port_number;

    if (input.find(http_prefix) > input.length()){
        throw "URL parser error";
    }

    //parsing URL
    arguments_struct.remote_url = input.substr(http_prefix.length(),input.length());
    arguments_struct.remote_url = arguments_struct.remote_url.substr(0, arguments_struct.remote_url.find_first_of(":"));

    arguments_struct.remote_path = input.substr(http_prefix.length());

    //parsing port number
    port_number = arguments_struct.remote_path.substr(arguments_struct.remote_path.find_first_of(":")+1);
    port_number = port_number.substr(0, port_number.find_first_of("/"));
    arguments_struct.remote_port = stoi(port_number);

    arguments_struct.remote_path = arguments_struct.remote_path.substr(arguments_struct.remote_path.find_first_of("/"));

}