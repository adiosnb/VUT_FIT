//
// Created by atomasov on 10/21/17.
//

#ifndef ISA_HOST_H
#define ISA_HOST_H


#include "ArgParser.h"
#include "MsgBuffer.h"
#include "HostConnection.h"
#include "Pop3Server.h"

/**
 * class Host - this object holds information about connected host and created and instance of pop3server.
 */
class Host {
private:
	ArgParser *parser;
	int *lock;
	HostConnection *connection;
	Pop3Server *pop3server;

public:
	/**
	 * constructor - init valuse. Creates instance on po3server.
	 * @param parser ptr oto parserojbect, whre arguments are paarsed.
	 * @param lock ptr to simple lock for maildir.
	 * @param connection new connection created by Connection::accept()
	 */
	Host(ArgParser *parser, int *lock, HostConnection *connection);

	/**
	 * Destructor delete connection and pop3server
	 */
	~Host();

	HostConnection *get_connection();

	void run();

	void say_hello();
};


#endif //ISA_HOST_H
