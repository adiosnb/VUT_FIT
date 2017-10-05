#ifndef CONNECTION
#define CONNECTION

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// TODO delete:
#define echo(a) std::cout << a << std::endl;

namespace conn {

    enum commands {unknown = 0, del , get, put, lst, mkd, rmd};
    enum type_socket {server = 0, client};

    /**
     * Class Connection - black box class for handling socket. Every necessary function for correct socket operations
     * are writen here. This class is summary of Client socket functions and server functions. n future should be split
     * into two separate child class.
     */
    class Connection {
    public :
        //premenne;
        int tcp_port;
        std::string url;


        type_socket type;
        socklen_t client_len;

        int socket_fd;
        int connection_fd;
        struct sockaddr_in server_addr, client_addr;
        struct hostent *hostent_server = NULL;



        /**
         * Constructor for server socket.
         * @param type identification of server/client
         * @param port port number where socket shoudl be created.
         */
        Connection(type_socket type, uint16_t port);

        /**
         * Constructor for client side socket.
         * @param type identifier of client (just value from enum).
         * @param port Destintion port of server, socket tries to connect there.
         * @param url URL address of destiation server.
         */
        Connection(type_socket type, uint16_t port, std::string url);
        /**
         * opening socket
         */
        int open_socket();

        /*
         * Functions are black box for kernel callings.
         */

        void connect();
        void listen();
        void accept();
        void close_con();
        void close_socket();


        /*
         * Functions below are handlers for sending and receiving http msg and files.
         */

        void send(std::string msg);
        std::string recv();
        std::string recv_header();

        void send_msg_and_file(std::string msg, FILE *source);
        void recv_msg_and_file(std::string &msg, FILE *destination);

        void send_file(FILE *source);
        void recv_file(FILE *destination, long file_len);
    };

}

#endif // CONNECTION