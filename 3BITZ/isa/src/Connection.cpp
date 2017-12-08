/**************************************
 *   File :                  Connection.cpp
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Created :               15.11.2017
 *   Compiled:               g++ 7.2.1
 *   Project :               popser
 ***************************************/


#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#include "Connection.h"
#include "Exception.h"

//this si constructor for server
Connection::Connection(uint16_t port) {
	echo("Conncection Constructor");
	this->tcp_port = port;
	echo(port);

	//setting sockaddr_in parameters
	bzero(&this->server_addr, sizeof(this->server_addr));
	this->server_addr.sin_family = AF_INET;
	this->server_addr.sin_addr.s_addr = INADDR_ANY;
	this->server_addr.sin_port = htons(this->tcp_port);

	open_socket();

	//binding socket
	if (bind(this->socket_fd, (struct sockaddr *) &this->server_addr, sizeof(this->server_addr))) {
		this->close_socket();
		throw Exception("Error: Bindingsocket");
	}
}


void Connection::open_socket() {
	echo(tcp_port);
    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd < 0) {
        throw Exception("Error while opening socket");
    }
}

void Connection::listen() {
    if (::listen(this->socket_fd, 5) < 0){
        throw Exception("Error: listen(socket_fd)");
    }
}


HostConnection* Connection::accept() {
	HostConnection *new_host = new HostConnection();

	errno = 0;

    if ((new_host->connection_fd = ::accept(
			this->socket_fd, (struct sockaddr *) &new_host->host_addr, &new_host->socklen)) < 0) {
		delete new_host;
		echo(errno);
		echo(EAGAIN);
		echo(EWOULDBLOCK);
		//if (errno == EWOULDBLOCK)
			return NULL;
		//else
        //	throw Exception("Error: accept()");
    }


	int flags = fcntl(new_host->connection_fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	if (fcntl(new_host->connection_fd, F_SETFL, flags) < 0){
		echo("fcntl error ");
		delete new_host;
		return NULL;
	}

	new_host->buffer = new MsgBuffer(new_host->connection_fd);
	return new_host;
}

void Connection::close_socket() {
    if (::close(this->socket_fd) < 0) {
        throw Exception("Error: closing current socket");
    }
}

void Connection::set_nonblock() {
	int flags = fcntl(socket_fd, F_GETFL, 0);
	flags |= O_NONBLOCK;

	if (fcntl(socket_fd, F_SETFL, flags) < 0){
		throw Exception("Connection >> error while fcntl");
	}
}

Connection::~Connection() {
	this->close_socket();
}
