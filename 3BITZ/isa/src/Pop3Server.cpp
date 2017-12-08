/**************************************
 *   File :                  Pop3Server.cpp
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Created :               15.11.2017
 *   Compiled:               g++ 7.2.1
 *   Project :               popser
 ***************************************/

#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <cctype>
#include <fstream>
#include <unistd.h>
#include "Pop3Server.h"
#include "Exception.h"
#include "md5.h"
#include "debug.h"

short int Pop3Server::uid = 0;

Pop3Server::Pop3Server(ArgParser *args, int *lock) {
	parser = args;
	mail_dir = new MailDir(parser->getDirectory());
	this->lock = lock;
	state = State::Authorization;
	read_auth_file();
	my_lock = false;
}

Pop3Server::~Pop3Server() {
	delete(mail_dir);
	if (my_lock) *lock -= 1;
}

std::string Pop3Server::recv_msg(std::string msg) {

	std::string token;
	std::vector<std::string> tokens;

	size_t eol_inedx = msg.find("\r\n");

	if (eol_inedx == std::string::npos){
		throw Exception(234, "POP3Server didn't find CRLF in recieved message.");
	}


	unsigned int i = 0;
	unsigned long first, last;

	size_t str_len = msg.size();

	try {
			while (i < msg.size()) {
			while ((!isgraph(msg[i])) && str_len > i) i++;
			first = i;

			while (isgraph(msg[i]) && str_len > i) i++;
			last = i;

			if (first != last) {
				token = msg.substr(first, last-first);
				tokens.push_back(token);
			}
		}
	}
	catch (std::out_of_range &e) {}

	if (tokens.empty()){
		return "-ERR none argument";
	}

	// if there is no entry, just return -ERR
	if (tokens.empty())
		return "-ERR";

	std::string command = tokens[0];
	std::transform(command.begin(), command.end(), command.begin(), toupper);

	if (command == "QUIT"){
		return quit(tokens);
	} else if (command == "STAT") {
		return stat(tokens);
	} else if (command == "LIST") {
		return list(tokens);
	} else if (command == "RETR") {
		return retr(tokens);
	} else if (command == "DELE") {
		return dele(tokens);
	} else if (command == "NOOP") {
		return noop(tokens);
	} else if (command == "RSET") {
		return rset(tokens);
	} else if (command == "QUIT") {
		return quit(tokens);
	} else if (command == "UIDL") {
		return uidl(tokens);
	} else if (command == "USER") {
		return user(tokens);
    } else if (command == "PASS") {
		return pass(msg);
	} else if (command == "APOP") {
		return apop(tokens);
	} else if (command == "TOP"){
		return top(tokens);
	} else {
		return "-ERR unknown command";
	}

}

std::string Pop3Server::user(std::vector<std::string> &tokens) {
	if (state != State::Authorization) return "-ERR you cannot use this command in transaction state";
	if (!parser->getClear_pass()) return "-ERR unknown command";
	if (tokens.size() != 2) return "-ERR wrong number of arguments";

	input_username = tokens[1];
	state = State::UserAuth;
	return "+OK please write password";
}

std::string Pop3Server::stat(std::vector<std::string> &tokens) {
	if (tokens.size() != 1) 			return "-ERR too many arguments";
	if (state != State::Transaction) 	return "-ERR you cannot user this command here";

	std::string return_string = "+OK ";
	long sizes = 0;
	long num_of_mail = 0;
	for (auto &item:mail_dir->msgs){
		if (!item->is_delete()){
			num_of_mail++;
			sizes += item->getSize();
		}
	}

	return_string += std::to_string(num_of_mail) + " ";
	return_string += std::to_string(sizes);

	return return_string;
}

std::string Pop3Server::list(std::vector<std::string> &tokens) {
	if (state != State::Transaction) return "-ERR you cannot use this command here";
	if (tokens.size() > 2) return "-ERR too many arguments";

	std::string ret_str = "+OK ";
	std::string msgs;

	if (tokens.size() == 1){

		int num_of_mail = 0;
		long sizes = 0;
		for (auto &item:mail_dir->msgs){
			if (!item->is_delete()){
				num_of_mail++;
				sizes += item->getSize();
				msgs += std::to_string(item->getId()) + " " +
						std::to_string(item->getSize()) + "\r\n";
			}
		}

		ret_str += std::to_string(num_of_mail) + " ";
		ret_str += std::to_string(sizes) + "\r\n";
		ret_str += msgs;
		ret_str += ".";

		return ret_str;
	} else {
		std::string::size_type str_size;
		unsigned long id ;

		try {
			id = std::stoul(tokens[1], &str_size);
		} catch (std::invalid_argument &e){
			return "-ERR invalid argument";
		}

		if (str_size < tokens[1].size()){
			return "-ERR unknown id";
		}

		if (id > mail_dir->msgs.size() || id == 0){
			ret_str = "-ERR no such message, only " + std::to_string(mail_dir->msgs.size())
					  + " messages in maildrop";
			return ret_str;
		}

		MailMsg *msg = mail_dir->get_msg_from_cur_by_id(id);

		if (msg->is_delete())
			return "-ERR no such message";

		ret_str += tokens[1] + " " + std::to_string(msg->getSize());
		return ret_str;
	}
}

std::string Pop3Server::retr(std::vector<std::string> &tokens) {
	if (state != State::Transaction) return "-ERR you cannot use this command";
	if (tokens.size() != 2) return "-ERR wrong number of arguments";


	unsigned long id;
	std::string::size_type str_size;
	try {
		id = std::stoul(tokens[1], &str_size);
	} catch (std::invalid_argument &e){
		return "-ERR invalid argument";
	}

	if (str_size < tokens[1].size()) {
		return "-ERR unknown id";
	}
	if (mail_dir->msgs.size() < id || id == 0) return "-ERR no such message";

	MailMsg *msg = mail_dir->get_msg_from_cur_by_id(id);
	if (msg->is_delete()) return "-ERR no such message";

	std::string ret_str = "+OK ";
	ret_str += std::to_string(msg->getSize()) + " octets\r\n";

	// this request another sending of data because in msg can be raw data
	throw SendMailMsg(ret_str, msg);

	return ret_str;
}

std::string Pop3Server::dele(std::vector<std::string> &tokens) {
	if (state != State::Transaction) 	return "-ERR you cannot use this command";
	if (tokens.size() != 2) 			return "-ERR wrong number of arguments";

	std::string::size_type str_size;
	unsigned long id = std::stoul(tokens[1], &str_size);

	if (str_size < tokens[1].size()){
		return "-ERR unknown id";
	}

	if (id > mail_dir->msgs.size() || id == 0)
		return "-ERR no such message";

	std::string ret_str;
	MailMsg *msg = mail_dir->get_msg_from_cur_by_id(id);
	if (!msg->is_delete()){
		msg->set_delete();
		ret_str = "+OK message ";
		ret_str += std::to_string(id);
		ret_str += " deleted";
		return ret_str;
	} else {
		ret_str = "-ERR message ";
		ret_str += std::to_string(id);
		ret_str += " already deleted";
		return ret_str;
	}
}

std::string Pop3Server::noop(std::vector<std::string> &tokens) {
	if (tokens.size() == 1){
		return "+OK";
	} else {
		return "-ERR";
	}
}

std::string Pop3Server::rset(std::vector<std::string> &tokens) {
	if (state != State::Transaction) return "-ERR you cannot use this command";
	if (tokens.size() > 1) return "-ERR too much arguments";

	for (auto &item: mail_dir->msgs){
		item->unset_delete();
	}
	std::string ret_str = "+OK mailbox have ";
	ret_str += std::to_string(mail_dir->msgs.size()) + " messages";
	return ret_str;
}

std::string Pop3Server::pass(std::string msg) {
	if (!parser->getClear_pass()) 		return state = State::Authorization, "-ERR unknown command";
	if (state != State::UserAuth) 		return state = State::Authorization, "-ERR you need  to specify user first";
	if (input_username != username ) 	return state = State::Authorization, "-ERR wrong password";

	std::string password = msg.substr(5, msg.size() - 7);
	echo(password);
	echo(password.size());

	if (password != this->password) return state = State::Authorization, "-ERR wrong password";

	if (*lock)						return state = State::Authorization, "-ERR mailbox is currently used";

	echo("Real pass >> " + password);

	*lock += 1;	// locking acces into server
	my_lock++;
	mail_dir->read_dirs();
	state = State::Transaction;
	return "+OK you have access to your mailbox";
}

std::string Pop3Server::apop(std::vector<std::string> &tokens) {
	echo(md5(timestamp + password));
	if (state != State::Authorization) 	return "-ERR cannot use this command";
	if (tokens.size() != 3) 			return "-ERR wrong number of arguments";
	if (tokens[1] != username ||
		tokens[2] != md5(timestamp + password))
										return "-ERR wrong password";
	if (*lock) 							return "-ERR mail box is currently used";

	*lock += 1;
	my_lock++;
	mail_dir->read_dirs();
	state = State::Transaction;
	return "+OK password correct";
}

std::string Pop3Server::quit(std::vector<std::string> &tokens) {
    if (tokens.size() > 1) return "-ERR too much arguments";
	throw QuitServer();
}

std::string Pop3Server::uidl(std::vector<std::string> &tokens) {
	if (state != State::Transaction) return "-ERR you cannot use this command";
	if (tokens.size() > 2 ) return "-ERR too much arguments";

	std::string ret_str = "+OK ";
	if (tokens.size() == 1){
		for (auto &item: mail_dir->msgs){
			ret_str += std::to_string(item->getId()) + " " + item->get_uidl() + "\r\n";
		}
	} else {

		std::string::size_type str_size;
		unsigned long id;

		try {
			id = std::stoul(tokens[1], &str_size);
		} catch (std::invalid_argument &e){
			return "-ERR invalid argument";
		}


		if (str_size < tokens[1].size()){
			return "-ERR unknown id";
		}

		if (id > mail_dir->msgs.size() || id == 0)
			return "-ERR no such message";

		MailMsg *msg = mail_dir->get_msg_from_cur_by_id(id);

		if (msg->is_delete()){
			return "-ERR no such message";
		}

		ret_str += std::to_string(msg->getId()) + " " + msg->get_uidl();
	}
	return ret_str;
}

void Pop3Server::read_auth_file() {
	std::ifstream in_file;
	in_file.open(parser->getAuth_file());
	std::string line;

	in_file >> std::noskipws;

	std::getline(in_file, line);
	username = line.substr(line.find('=') + 2);
	std::getline(in_file, line);
	password = line.substr(line.find('=') + 2);

	echo(username);
	echo(password);

	in_file.close();
}

std::string Pop3Server::greeting() {
	std::string ret_str = "+OK POP3 server ready ";
	char hostname[HOSTNAME_MAX_LEN];
	std::time_t t = std::time(NULL);

	gethostname(hostname, HOSTNAME_MAX_LEN);
	timestamp = "";
	timestamp += "<" + std::to_string(getpid()+uid++) + "." + std::to_string(t) + "@" + hostname + ">";

	echo("Timestamp = " + timestamp);
	ret_str += timestamp;
	return ret_str;
}

std::string Pop3Server::top(std::vector<std::string> &tokens) {
	std::string ret_str = "+OK\r\n";

	if (state != State::Transaction) return "-ERR you cannot use this command here";
	if (tokens.size() != 3 ) return "-ERR wrong num of arguments";

	unsigned long msg, lines;
	std::string::size_type st_msg, st_lines;

	try {
		msg = std::stoul(tokens[1], &st_msg);
		lines = std::stoul(tokens[2], &st_lines);
	} catch (std::invalid_argument &e){
		return "-ERR invalid argument";
	}

	if (msg > mail_dir->msgs.size() || msg == 0){
		return "-ERR no such message";
	}

	MailMsg *ptr_msg = mail_dir->get_msg_from_cur_by_id(msg);

	if (ptr_msg->is_delete()){
		return "-ERR no such message";
	}

	throw SendMailMsgLines(ret_str, ptr_msg, lines);

	return ret_str;
}
