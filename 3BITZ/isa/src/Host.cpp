/**************************************
 *   File :                  Host.cpp
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Created :               15.11.2017
 *   Compiled:               g++ 7.2.1
 *   Project :               popser
 ***************************************/


//
// Created by atomasov on 10/21/17.
//
#include <unistd.h>

#include "Host.h"
#include "Exception.h"
#include "debug.h"

Host::Host(ArgParser *parser, int *lock, HostConnection *connection) {
	this->parser = parser;
	this->lock = lock;
	this->connection = connection;
	echo("initializing pop3server object in Host constructor")
	this->pop3server = new Pop3Server(parser, lock);
}

void Host::run() {
	echo("host run");
	if (connection->buffer->is_readed()){
		try {
			echo("msg received and forwarded to pop3server");
			connection->buffer->set_q(pop3server->recv_msg(connection->buffer->get_line()));
			connection->buffer->send_msg();
		}
		catch (SendMailMsg &exc){
			echo("SendMailMsg exception raised");
			connection->buffer->direct_send(exc);
		}
		catch (SendMailMsgLines &exc){
			echo("SendMailMsgLines exception raised");
			connection->buffer->direct_send_lines(exc);
		}
		catch (QuitServer &exc){
			connection->buffer->set_q("+OK Goodbye");
			connection->buffer->send_msg();
			throw QuitServer();
		}
	}
}

Host::~Host() {
	delete connection;
	delete pop3server;
}

HostConnection *Host::get_connection() {
	return connection;
}

void Host::say_hello() {
	echo("Saying helo");
	connection->buffer->set_q(pop3server->greeting());
	connection->buffer->send_msg();
}
