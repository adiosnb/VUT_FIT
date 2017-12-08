//
// Created by atomasov on 10/5/17.
//

#ifndef ISA_EXCEPTION_H
#define ISA_EXCEPTION_H

#define E_GETOPT 1
#define E_PARSER_CHECK  2

#include <string>
#include <exception>
#include "MailMsg.h"

/**
 * Exception - this helps debug application. You can specify what() msg, which is returned when
 * exception happen.
 */
class Exception : public std::exception{
public:
    int error_num;
    std::string error_msg;

	/**
	 * Constructor
	 * @param num Error number
	 * @param msg Message which is printed when error occurs.
	 */
    Exception(int num, const std::string &msg);

	/**
	 * Construcotr without error num
	 * @param msg Message which is printed when error occurs.
	 */
	Exception(const std::string &msg);

    void print_error();

    void print_and_exit();

	/**
	 * Overriding base method for printing error.
	 */
	const char *what() const throw () override;
};

/**
 * QuitServer - excpetion signalizing when host is quitting the system and memory should be freed.
 */
class QuitServer : public std::exception {};

/**
 * SendMailMsg - signalizing server if file should be send. Pop3server object handle this exception and let know MsgBuffer
 * that file should be send directly with out c++ string conversion.
 */
class SendMailMsg : public std::exception {
public:
	SendMailMsg(std::string server_response, MailMsg *msg): response(server_response), msg(msg) {};

	std::string response;
	MailMsg *msg;
};

/**
 * SendMailMsgLines - signalizing server if only several lines of mali should be send. Pop3server object handle this
 * exception and let know MsgBuffer that file should be send directly with out c++ string conversion.
 */
class SendMailMsgLines : std::exception {
public:

	std::string response;
	MailMsg *msg;
	unsigned long lines;

	SendMailMsgLines(std::string response, MailMsg *msg, unsigned long lines): response(response), msg(msg), lines(lines) {};
};

/**
 * HelpException - throw if -h flag is set. Program handle this exception, write help and quit.
 */
class HelpException : std::exception {};

/**
 * ResetAndExit - exception is signalizing server to reset and stop. It happens when you
 * write just -r in argv.
 */
class ResetAndExit : public std::exception {};

/**
 * ResetAndContinue - exception is signalizing server to reset mail directory before it
 * server to client.
 */
class ResetAndStart : public std::exception {};

#endif //ISA_EXCEPTION_H
