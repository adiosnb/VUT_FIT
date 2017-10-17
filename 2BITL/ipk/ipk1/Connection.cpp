#include <cstring>
#include <unistd.h>
#include "Connection.h"
#include "HttpHeader.h"

using namespace conn;

//this si constructor for server
Connection::Connection(type_socket type, uint16_t port) {
    this->type = type;
    this->tcp_port = port;
    this->socket_fd = open_socket();

    //setting sockaddr_in parameters
    bzero(&this->server_addr,sizeof(this->server_addr));
    this->server_addr.sin_family = AF_INET;
    this->server_addr.sin_addr.s_addr = INADDR_ANY;
    this->server_addr.sin_port = htons(port);

    //binding socket
    if(bind(this->socket_fd,(struct sockaddr *) &this->server_addr, sizeof(this->server_addr))){
        throw "Error: Bindingsocket";
    }

    this->client_len = sizeof(client_addr);
}


//this is constructor for client
Connection::Connection(type_socket type, uint16_t port, std::string url){
    this->type = type;
    this->tcp_port = port;
    this->socket_fd = open_socket();
    this->connection_fd = socket_fd;
    this->url = url;

    //setting sockaddr_in parameters
    this->hostent_server = gethostbyname(url.c_str());
    if (this->hostent_server == NULL){
        throw "ERROR: DNS lookup";
    }

    bzero(&this->server_addr,sizeof(this->server_addr));
    this->server_addr.sin_family = AF_INET;
    this->server_addr.sin_port = htons(port);
    bcopy(this->hostent_server->h_addr_list[0],
    &this->server_addr.sin_addr.s_addr, this->hostent_server->h_length);


    this->client_len = sizeof(client_addr);
}


int Connection::open_socket() {
    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd < 0) {
        throw "Error while opening socket";
    }
}

void Connection::listen() {
    if (::listen(this->socket_fd, 5) < 0){
        throw "Error: listen(socket_fd)";
    }
}

void Connection::connect() {
    if (::connect(this->socket_fd, (struct sockaddr *) &this->server_addr, sizeof(this->server_addr)) < 0){
        throw "Error: connect";
    }
}

void Connection::accept() {
    if ((this->connection_fd = ::accept(this->socket_fd, (struct sockaddr *) &this->client_addr, &this->client_len)) < 0) {
        throw "Error: accept()";
    }
}

void Connection::send(std::string msg) {
    if(write(this->connection_fd, msg.c_str(), msg.length()) < 0){
        throw "Error: writing message to connection";
    }
}

std::string Connection::recv() {
    std::string return_string;
    size_t segment_len = 4096;
    ssize_t recv_len = 0;
    char buffer[segment_len +1];

    do {
        recv_len = read(this->connection_fd, buffer, segment_len);
        if (recv_len < 0){
            throw "Error: reading from socket";
        }
        return_string += buffer;
        bzero(buffer, segment_len +1);
    } while (segment_len == recv_len);

    return return_string;
}

std::string Connection::recv_header() {
    std::string return_string;
    size_t segment_len = 4096;
    ssize_t recv_len = 0;
    char buffer[segment_len +1];

    do {
        recv_len = read(this->connection_fd, buffer, 1);
        if (recv_len < 0){
            throw "Error: reading from socket";
        }
        return_string += buffer;
        bzero(buffer, segment_len +1);
        if (return_string.find("\r\n\r\n") < return_string.length()) break;
    } while (1);

    return return_string;
}

void Connection::close_con() {
    if (::close(this->connection_fd) < 0) {
        throw "Error: closing current connection";
    }
}

void Connection::close_socket() {
    if (::close(this->socket_fd) < 0) {
        throw "Error: closing current socket";
    }
}


void Connection::send_msg_and_file(std::string msg, FILE *source){
    size_t segment_len = 4096*4;
    char buffer[segment_len+1];
    size_t read_char = 0;
    const char *c_msg = msg.c_str();

    long deliminter;

    for (int i = 0; i < msg.length() / segment_len; i++){
        if (write(this->connection_fd, &c_msg[i], segment_len) < 0){
            throw "Error: send_msg_and_files";
        }
    }

    deliminter = msg.length() % segment_len;
    strcpy(buffer, &c_msg[msg.length() / segment_len]);


    if ((read_char = fread(&buffer[deliminter],1,segment_len - deliminter,source)) < 0){
        throw "Error: send_msg_and_file: read";
    }



    if (write(connection_fd, buffer, read_char + deliminter) < 0){
        throw "Error sending data:send_file";
    }

    while ((read_char = fread(buffer, 1, segment_len, source)) > 0){
        if (write(connection_fd, buffer, read_char) < 0){
            throw "Error: send_msg_and_file:send_file";
        }
    }
}


void Connection::recv_msg_and_file(std::string &msg, FILE *destination){
    size_t segment_len = 4096;
    ssize_t recv_len = 0;
    char char_buffer;
    char buffer[segment_len +1];
    std::string header_end = http::CRLF;
    header_end += http::CRLF;

    while (1){
        if (read(this->connection_fd, &char_buffer, 1) < 0){
            throw "Error: recv_msg_and_file";
        }
        msg += char_buffer;

        if (msg.find(header_end) < msg.length()){
            break;
        }
    }

    do {
        recv_len = read(this->connection_fd, buffer, segment_len);
        if (recv_len < 0){
            throw "Error: reading from socket";
        }
        fwrite(buffer, 1, recv_len, destination);
        bzero(buffer, segment_len +1);
    } while (segment_len == recv_len);

}




void Connection::send_file(FILE *input_file) {
    size_t segment_len = 4096;
    char buffer[segment_len+1];
    size_t read_char = 0;

    while ((read_char = fread(buffer, 1, segment_len, input_file)) > 0){
        if (write(connection_fd, buffer, read_char) < 0){
            throw "Error sending data:send_file";
        }
    }
}

void Connection::recv_file(FILE *destination, long file_len) {
    size_t segment_len = 4096*2;
    size_t recv_len = 0;
    long file_size = 0;
    char buffer[segment_len +1];

    do {
        recv_len = read(this->connection_fd, buffer, segment_len);
        if (recv_len < 0){
            throw "Error: reading from socket";
        }
        fwrite(buffer, 1, recv_len, destination);
        bzero(buffer, segment_len +1);
        file_size += segment_len;
    } while (segment_len == recv_len && file_size < file_len);
}
