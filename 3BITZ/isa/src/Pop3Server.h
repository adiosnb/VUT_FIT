//
// Created by atomasov on 10/11/17.
//

#ifndef ISA_POP3SERVER_H
#define ISA_POP3SERVER_H

#include <iostream>
#include <vector>
#include <mutex>

#include "ArgParser.h"
#include "MailDir.h"

#define HOSTNAME_MAX_LEN 256

/**
 * States of pop3 server.
 * Update state is never used, because operation provided by update state are defined in destructors of object, so
 * delele(pop3server) is invoked and everything is done.
 */
enum class State {Authorization, UserAuth, Transaction, Update,};

/**
 * Pop3server represents protocol behaviour of communication. Separate instance of this object is created for each
 * connected host. Incoming communication is parsed and corresponding operation is called. This object works only
 * with strings, no IO operations are used.
 */
class Pop3Server {
private:

	static short int uid;

    State state;					///< State of pop3server
	ArgParser *parser;				///< ptr to user defined arguments
    std::string username;			///< username specified in auth-file
    std::string password;			///< password specified in auth-file

	std::string input_username;		///< user spacefied by USER command from host
	std::string timestamp;			///< timestamp when server start, used to calcucalte md5 hash od password

	int *lock;						///< MailDir lock, used to mutual exclusion
	int my_lock;					///< Do I own a lock ?
	MailDir *mail_dir;				///< ptr to abstract object which represents mail dir


public:
	explicit Pop3Server(ArgParser *args, int *lock);
	~Pop3Server();
	/**
	 * Read credentials about user especially username and password from auth-file specified in arg_parser.
	 */
    void read_auth_file();

	/**
	 * Shift message received by message buffer to pop3server. Message is parsed inside this functions and corresponding
	 * operation of pop3 server is called.
	 * @param msg Message from msg_buffer.
	 * @return std::string answer of pop3server which should be send to socket.
	 */
	std::string recv_msg(std::string msg);

	/**
	 * Welcome new user with servre ready message and timespamp.
	 * @return answer (server ready and timestamp)
	 */
	std::string greeting();

private:
	/**
	 * Operations specified by RFC of pop3.
	 */

	std::string stat(std::vector<std::string> &tokens);

	std::string list(std::vector<std::string> &tokens);

	std::string retr(std::vector<std::string> &tokens);

	std::string dele(std::vector<std::string> &tokens);

	std::string noop(std::vector<std::string> &tokens);

	std::string rset(std::vector<std::string> &tokens);

	std::string user(std::vector<std::string> &tokens);

	std::string pass(std::string msg);

	std::string apop(std::vector<std::string> &tokens);

	std::string quit(std::vector<std::string> &tokens);

	std::string uidl(std::vector<std::string> &tokens);

	std::string top(std::vector<std::string> &tokens);
};


#endif //ISA_POP3SERVER_H
