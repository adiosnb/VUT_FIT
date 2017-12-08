/**************************************
 *   File :                  MsgBuffer.cpp
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Created :               15.11.2017
 *   Compiled:               g++ 7.2.1
 *   Project :               popser
 ***************************************/


//
// Created by atomasov on 10/21/17.
//

#include <sys/socket.h>
#include <unistd.h>

#include <utility>
#include <cstring>
#include "MsgBuffer.h"
#include "debug.h"
#include "Exception.h"

MsgBuffer::MsgBuffer(int socket) {
	this->socket = socket;
}

int MsgBuffer::is_readed() {
	if (inner_q.find("\r\n") == std::string::npos){
		this->flush();
		this->recv_msg();
		return inner_q.find("\r\n") != std::string::npos;
	} else {
		return true;
	}
}

void MsgBuffer::recv_msg() {
	ssize_t len = 0;
	char buffer[BUFFER_SIZE + 1] = {0,};
	size_t buffer_len = BUFFER_SIZE;
	inner_q = "";

	while ((len = recv(socket, buffer, buffer_len, 0)) > 0){
		inner_q += buffer;
	}

	echo(len);
	if (errno != EWOULDBLOCK && len == -1){
		throw Exception("Error while recv_msg");
	}

	if (len == 0) {
		echo("Socket has been disconected");
		throw QuitServer();
	}
}

void MsgBuffer::set_q(std::string msg){
	this->out_q = std::move(msg);
}

void MsgBuffer::send_msg() {
	ssize_t len = 0;
	out_q += "\r\n";

	if ((len = send(socket, out_q.c_str(), out_q.size(), 0)) < 0){
		throw Exception("Error while send_msg()");
	}
}

void MsgBuffer::flush() {
	inner_q = "";
}

std::string MsgBuffer::get_line() {
	unsigned long eol = inner_q.find("\r\n") + 2;

	std::string ret_str = inner_q.substr(0, eol);
	inner_q = inner_q.substr(eol);

	return ret_str;
}

void MsgBuffer::direct_send(SendMailMsg &exc) {
	size_t buffer_size = 0;
	char *buffer = (char *)calloc(sizeof(char), exc.response.size() + exc.msg->getEngPosition() + 3);
	char *msg_ptr = exc.msg->get_content();

	echo("Response size >> " + std::to_string(exc.response.size()));
	echo("Content size >> " + std::to_string(exc.msg->getSize()));

	strcpy(buffer, exc.response.c_str());
	buffer_size = strlen(buffer);

	long msg_size = exc.msg->getEngPosition();
	for (long i = 0 ; i < msg_size ; i++){
		buffer[buffer_size++] = msg_ptr[i];
	}

	echo("Buffer size >> " + std::to_string(buffer_size));

	buffer[buffer_size++] = '.';
	buffer[buffer_size++] = '\r';
	buffer[buffer_size++] = '\n';

	if (send(socket, buffer, buffer_size, 0) < 0){
		throw Exception("MsgBuffer error : direct_send");
	}

	free(buffer);
}

void MsgBuffer::direct_send_lines(SendMailMsgLines &exc) {
	echo("Exception handlen in msg buffer");

	size_t buffer_size = 0;
	char *buffer = (char *)calloc(sizeof(char), exc.response.size() + exc.msg->getSize() + 3);
	char *msg_ptr = exc.msg->get_content();

	echo("Response size >> " + std::to_string(exc.response.size()));
	echo("Content size >> " + std::to_string(exc.msg->getSize()));

	strcpy(buffer, exc.response.c_str());
	buffer_size = strlen(buffer);

	long msg_size = exc.msg->getSize();
	unsigned long lines = 0;
	long i;
	for (i = 0 ; i < msg_size && i < 4 ; i++){
		buffer[buffer_size++] = msg_ptr[i];
	}

	for (; i < msg_size ; i++){
		if (strncmp("\r\n\r\n", &msg_ptr[i-3], 4)){
			buffer[buffer_size++] = msg_ptr[i];
		} else {
			echo("Found end of header");
			buffer[buffer_size++] = msg_ptr[i++];
			break;
		}
	}

	for (; i < msg_size && lines < exc.lines ; i++){
		if (msg_ptr[i] == '\n'){
			lines++;
		}
		buffer[buffer_size++] = msg_ptr[i];
	}

	buffer[buffer_size++] = '.';
	buffer[buffer_size++] = '\r';
	buffer[buffer_size++] = '\n';

	echo("Buffer size >> " + std::to_string(buffer_size));

	if (send(socket, buffer, buffer_size, 0) < 0){
		throw Exception("MsgBuffer error : direct_send");
	}

	free(buffer);
}
