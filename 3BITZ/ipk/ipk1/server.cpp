#include <unistd.h>
#include <csignal>
#include <sys/stat.h>
#include <algorithm>
#include <stdlib.h>

#include "ServerArgParser.h"
#include "Connection.h"
#include "HttpHeader.h"
#include "ServerFunctions.h"

/**
 * This is server body. It is prepared to run in single process/thread. This function might be called after accepting
 * new connection. Server recv HttpRequest here, parse request and tries to make response to client.
 * @param connection member of Class Connection with valid connection_fd.
 * @param arguments Parsed line arguments.
 */
void server_body(conn::Connection connection, ServerArgParser::server_arg_t arguments);

/**
 * Interupt signal handler. Catch SIGINT, close socket file descriptors and quit program.
 * @param sig_num
 */
void server_interupt_handler(int sig_num);

int stay_in_cycle;

conn::Connection *glob_p_connection;/**< Pointer to Connection object of server. Interupt handler needs fd of socket. */

int main(int argc, char **argv){
    stay_in_cycle = true;

    signal(SIGINT, server_interupt_handler);

    ServerArgParser::server_arg_t arguments;
    try {
        arguments = ServerArgParser::parse_arg(argc,argv);

        conn::Connection server_conn(conn::server, arguments.listen_port);
        glob_p_connection = &server_conn;

        server_conn.listen();
        while (stay_in_cycle) {
            server_conn.accept();

            try {
                // after client is connected to server, calling main server function
                server_body(server_conn, arguments);
            }
            catch (const char *error){
                //if any error occurs here is cough and error message is sent to client
                std::string msg;
                std::string error_str = error;
                if (error == "File not found."){
                    msg = "HTTP/1.1 404 Not found";
                }
                else if (error_str.find("Directory not found.") < error_str.length()){
                    msg = "HTTP/1.1 404 Not found";
                }
                else {
                    msg = "HTTP/1.1 400 Bad Request";
                }

                msg += http::CRLF;
                msg += http::CRLF;
                msg += error;
                server_conn.send(msg);
            }

            //std::cout << "Closing connection" << std::endl << std::endl ;
            server_conn.close_con();
        }
        server_conn.close_socket();

    }
    catch (const char* error){
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}



void server_body(conn::Connection connection, ServerArgParser::server_arg_t arguments) {
    std::string msg = "";
    std::string request;
    std::string user="";
    std::vector<std::string> files;
    struct stat file_stat;

    unsigned long type_pos;

    FILE *file;

    http::HttpHeader server_header(http::RESPONSE, http::NOTHING, msg, msg);
    server_header.content_encoding = http::IDENTITY;

    msg = connection.recv_header();

    request = msg.substr(0,msg.find(http::CRLF));

    if (request.substr(0,3) == "GET") server_header.request_type = http::GET;
    else if (request.substr(0,3) == "PUT")server_header.request_type = http::PUT;
    else if (request.substr(0,6) == "DELETE") server_header.request_type = http::DELETE;
    else throw "Bad request";


    if ((type_pos = request.rfind("?type=")) <= request.length()){
        if (request.substr(type_pos + 6, 4) == "file") server_header.file_type = http::FILE;
        else if (request.substr(type_pos + 6, 6) == "folder") server_header.file_type = http::FOLDER;
        else throw "Bad request";
    } else server_header.file_type = http::FILE;

    size_t addr_start = request.find_first_of("/");
    server_header.path = "./" + arguments.root_folder ;
    server_header.path += request.substr(addr_start, type_pos - addr_start );

    user = request.substr(addr_start+1, type_pos - addr_start );
    user = user.substr(0, user.find("/"));
    user = user.substr(0, user.find("/"));

    files = server_functions::ls_files("./"+arguments.root_folder);

    if (std::find(files.begin(), files.end(), user) == files.end()){
        throw "User Account Not Found.";
    }

    if (server_header.file_type == http::FOLDER){
        if (server_header.request_type == http::GET){
            files = server_functions::ls_files(server_header.path);
            std::sort(files.begin(), files.end());

            msg = "";
            for (int i = 0 ; i < files.size(); i++){
                if (files[i] == "." || files[i] == "..") continue;
                msg += files[i] + "\t";
            }
            server_header.content_lenght = msg.length();
            server_header.content_type = "text/plain";
            msg += http::CRLF;
            connection.send(server_header.make_response(200,"OK") + msg);
            return;
        }

        if (server_header.request_type == http::PUT){
            server_functions::make_dir(server_header.path);
            connection.send(server_header.make_response(200,"OK"));
            return;
        }

        if (server_header.request_type == http::DELETE) {
            server_functions::remove_dir(server_header.path);
            connection.send(server_header.make_response(200,"OK"));
            return;
        }
    }
    else {
        if (server_header.request_type == http::DELETE){
            server_functions::delete_file(server_header.path);
            connection.send(server_header.make_response(200,"OK"));
            return;
        }

        if (server_header.request_type == http::PUT){

            std::string file_len_str = msg.substr(msg.find("Content-Length:"));
            file_len_str = file_len_str.substr(0, file_len_str.find(http::CRLF));
            file_len_str = file_len_str.substr(file_len_str.find(" "), file_len_str.length() - file_len_str.find(" "));

            if ((file = fopen(server_header.path.c_str(), "wb")) == NULL ){
                throw "Unknown error.";
            }
            connection.recv_file(file,atol(file_len_str.c_str()));
            fclose(file);
            connection.send(server_header.make_response(200,"OK"));
            return;
        }

        if (server_header.request_type == http::GET){
            if ((file = fopen(server_header.path.c_str(), "rb")) == NULL){
                if (errno == ENOENT) throw "File not found.";
                else throw "Unknown error.";
            }
            stat(server_header.path.c_str(), &file_stat);
            server_header.content_lenght = std::to_string(file_stat.st_size);

            if (!S_ISREG(file_stat.st_mode)){
                throw "Not a file.";
            }

            connection.send_msg_and_file(server_header.make_response(200, "OK"),file);


            fclose(file);
            return;
        }
    }

}

void server_interupt_handler(int sig_num){
    stay_in_cycle = false;
    close(glob_p_connection->socket_fd);
    exit(EXIT_SUCCESS);
}