/**************************************
 *   File :                  Server.cpp
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
#include <sys/time.h>
#include <sys/select.h>
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

#include "Server.h"
#include "Host.h"
#include "Exception.h"

Server::Server(ArgParser *arg, Connection *connection) {
	this->arg_parser = arg;
	this->conn_sokcet = connection;
}

void Server::run() {
	int jj = 1;
	int max_fd = -1;

	fd_set sockets;

	HostConnection *new_host_connection;

	echo("Listening on connection socket");
	conn_sokcet->listen();

	while (jj){
		FD_ZERO(&sockets);

		// adding server socket to list of sockets
		FD_SET(conn_sokcet->socket_fd, &sockets);
		max_fd = conn_sokcet->socket_fd;

		// adding hosts sockets into structure
		for ( auto &item:hosts_list){
			if (max_fd < item.first) max_fd = item.first;
			FD_SET(item.first, &sockets);
		}

		if ((select(max_fd + 1, &sockets, NULL, NULL, NULL)) < 0 ){
			throw Exception("select error in server: ->run");
		}

		// checing if server socket has new accept
		if (FD_ISSET(conn_sokcet->socket_fd, &sockets)){
			if ((new_host_connection = conn_sokcet->accept()) != NULL){
				create_host(new_host_connection);
			}
		}

		// checking if host got some msg to read
		for (auto item = hosts_list.begin(); item != hosts_list.end();){
			if (FD_ISSET(item->first, &sockets)){
				try {
					item->second->run();
				}
				catch (QuitServer &q){
					echo("Host will be deleted");
					delete item->second;
					item = hosts_list.erase(item);
					continue;
				}

			}
			item++;
		}
	}
}

void Server::create_host(HostConnection *new_connection) {
	auto *new_host = new Host(arg_parser, &server_locked, new_connection);

	echo("Adding new host to container");
	hosts_list.insert(std::pair<int, Host*>(new_connection->connection_fd, new_host));

	echo("say hello to new host");
	new_host->say_hello();
}

Server::~Server() {
	for (auto item = hosts_list.begin(); item != hosts_list.end();){
		delete item->second;
		item = hosts_list.erase(item);
	}
}
