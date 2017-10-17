#include <iostream>
#include <cstring>
#include<arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include "HttpHeader.h"


#include "ClientArgParser.h"
#include "Connection.h"

conn::Connection *glob_p_connection;

int main(int argc, char **argv) {

    std::string http_header = "";
    std::string http_body = "";
    FILE *file;
    struct stat file_stat;

    try {
        ClientArgParser::client_arg_t arguments;
        arguments = ClientArgParser::parse_arg(argc, argv);
        ClientArgParser::check_arg(arguments);

        std::string msg;
        conn::Connection client_conn(conn::client, arguments.remote_port, arguments.remote_url);
        glob_p_connection = &client_conn;
        http::HttpHeader proto(http::GET, http::FILE, arguments.remote_url, arguments.remote_path);

        switch (arguments.cmd){
            case conn::del:
                proto.request_type = http::DELETE;
                proto.file_type = http::FILE;
                proto.content_type = "text/plain";

                break;
            case conn::get:
                proto.request_type = http::GET;
                proto.file_type = http::FILE;
                break;
            case conn::put:
                proto.request_type = http::PUT;
                proto.file_type = http::FILE;
                break;
            case conn::lst:
                proto.request_type = http::GET;
                proto.file_type = http::FOLDER;
                proto.content_type = "text/plain";
                break;
            case conn::mkd:
                proto.request_type = http::PUT;
                proto.file_type = http::FOLDER;
                proto.content_type = "text/plain";

                break;
            case conn::rmd:
                proto.request_type = http::DELETE;
                proto.file_type = http::FOLDER;
                proto.content_type = "text/plain";

                break;

        }





        if (proto.file_type == http::FILE && proto.request_type == http::PUT){
            if ((file = fopen(arguments.local_path.c_str(), "rb")) == NULL){
                throw "Unknown error.";
            }

            client_conn.connect();
            stat(arguments.local_path.c_str(),&file_stat);
            proto.content_lenght = std::to_string(file_stat.st_size);
            client_conn.send_msg_and_file(proto.make_request(), file);
            fclose(file);
        }
        else {
            client_conn.connect();
            client_conn.send(proto.make_request());
        }


        http_header = client_conn.recv_header();

        std::string http_code = http_header.substr(http_header.find(" ")+1,3);



        if (http_code == "200"){


            if (proto.file_type == http::FILE && proto.request_type == http::GET){
                std::string content_len = http_header.substr(http_header.find("Content-Length:"));
                content_len = content_len.substr(0, content_len.find(http::CRLF));
                content_len = content_len.substr(content_len.find(" "), content_len.length() - content_len.find(" "));

                if (arguments.local_path == "") {
                    arguments.local_path = arguments.remote_path.substr(arguments.remote_path.rfind("/") + 1);
                }
                if ((file = fopen(arguments.local_path.c_str(), "wb")) == NULL){
                    throw "Unknown error.";
                }

                client_conn.recv_file(file,atol(content_len.c_str()));
                fclose(file);
            }
            else {
                http_body = client_conn.recv();
            }

            if (proto.file_type == http::FOLDER && proto.request_type == http::GET){
                std::cout << http_body ;
            }

        }
        else {
            std::string return_msg = client_conn.recv();
            std::cout << return_msg << std::endl;
            exit(EXIT_FAILURE);
        }



        client_conn.close_socket();
    }
    catch (const char *error){
        std::cerr << error << std::endl;

        glob_p_connection->close_socket();
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}