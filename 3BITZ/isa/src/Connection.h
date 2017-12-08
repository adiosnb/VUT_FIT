#ifndef CONNECTION
#define CONNECTION

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "debug.h"
#include "HostConnection.h"


/**
 * Class Connection - black box class for handling socket. Every necessary function for correct socket operations
 * are writen here. This class is summary of Client socket functions and server functions. n future should be split
 * into two separate child class.
 */
class Connection {
public :
    //premenne;
    uint16_t tcp_port;
    int socket_fd;
    struct sockaddr_in server_addr;
    struct hostent *hostent_server = NULL;

    /**
     * Constructor for server socket.
     * @param type identification of server/client
     * @param port port number where socket shoudl be created.
     */
	Connection(uint16_t port);
	~Connection();


	/**
     * opening socket
     */
    void open_socket();

    /*
     * Functions are black box for kernel callings.
     */
    void listen();
    HostConnection* accept();
    void close_socket();
    void set_nonblock();
};



#endif // CONNECTION