//
// Created by atomasov on 10/21/17.
//

#ifndef ISA_SERVER_H
#define ISA_SERVER_H


#include <mutex>
#include <map>
#include "Connection.h"
#include "ArgParser.h"
#include "Pop3Server.h"
#include "Host.h"

/**
 * This object server hosts and welcome socket. It si responsible to accept incoming hosts and server to already
 * connected hosts. Server stores all connected hosts in hosts_list. Host objects and file descriptors are saved there.
 * This solution makes possible to use nonblocking socket and correct memory handling with no memory leaks.
 */
class Server {
private:
	Connection *conn_sokcet;			/**< ptr to Connection object, this object is created in main() and stores
 * 											 file descriptors to welcome socket*/
	ArgParser *arg_parser;				///< ptr to parser where all arguments options are stored
	int server_locked = false;			///< easy version of mail dor lock
	std::map<int, Host*> hosts_list;	///< Host are saved there to correct memory handling


public:
	Server(ArgParser *arg, Connection *connection);
	~Server();

	/**
	 * Endless loop which accepting new hosts and communicating with connected host. It is responsible for deleting
	 * outcoming hosts as well.
	 */
	void run();

	/**
	 * Write new incoming host in internal list of hosts.
	 * @param new_connection Host object.
	 */
	void create_host(HostConnection *new_connection);
};


#endif //ISA_SERVER_H
