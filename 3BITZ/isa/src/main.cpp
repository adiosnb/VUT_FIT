/**************************************
 *   File :                  main.cpp
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Created :               15.11.2017
 *   Compiled:               g++ 7.2.1
 *   Project :               popser
 ***************************************/


#include <iostream>
#include <csignal>
#include <unistd.h>

#include "ArgParser.h"
#include "Exception.h"
#include "Connection.h"
#include "Server.h"
#include "MailDirLogger.h"


Server *server;				/**< Global ptr to server instance. Need for memory cleaning. */
Connection *server_con;		/**< Global ptr to server connection. Need for memory cleaning. */

/**
 * Interupt signal handler.
 * @param sig
 */
void sig_int_handler(int sig);


using namespace std;

int main(int argc, char **argv) {

	signal(SIGINT, &sig_int_handler);


    ArgParser parser = ArgParser(argc, argv);
	echo("Parsed arguments");

    try {
        parser.parse();
		try {
			parser.check();
		}
		catch (ResetAndStart &r){
			MailDirLogger::reset();
		}
		catch (ResetAndExit &r){
			MailDirLogger::reset();
			return 0;
		}
		catch (HelpException &h){
			parser.print_help();
			return 0;
		}

		echo("Arguments checked and OK");
		echo("Creating connection");
        server_con = new Connection(parser.getPort());
		server_con->set_nonblock();

		echo("Creating and running server");
		server = new Server(&parser, server_con);
		server->run();
    }

    catch (Exception &e){
		delete server;
		delete server_con;
        e.print_error();
		return e.error_num;
    }

	delete server;
	delete server_con;

    return 0;
}


void sig_int_handler(int sig) {
	echo("Signal capture in main");
	echo("Killing server");
	delete server;
	delete server_con;
	exit(EXIT_SUCCESS);
}
