/**************************************
 *   File :                  MailMsg.cpp
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Created :               15.11.2017
 *   Compiled:               g++ 7.2.1
 *   Project :               popser
 ***************************************/


//
// Created by atomasov on 10/10/17.
//

#include "MailMsg.h"
#include "debug.h"
#include "Exception.h"
#include <fstream>
#include <regex>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include "MailDirLogger.h"

void MailMsg::read_content() {
    /*
     * TODO: use C style file reading
     * malloc array of chars where mail will be read
     *
     */

	echo("Reading : " + full_path);

    int file;
	dots = 0;

	int index_in_buffer, index_in_content;

	char *tmp_buffer = (char*)calloc(sizeof(char), size + 1);

	content = (char*)calloc(sizeof(char), (size * 2));

	if ((file = open(full_path.c_str(), O_RDONLY)) < 0){
		throw Exception("Error while opening file");
	}

	lseek(file, 0, SEEK_SET);

	if (read(file, tmp_buffer, size) < 0){
		throw Exception("Reading error");
	}

	close(file);

	for (index_in_buffer = 0 , index_in_content = 0 ; index_in_buffer < size ; index_in_buffer++, index_in_content++){

		if (index_in_buffer && tmp_buffer[index_in_buffer] == '\n' && tmp_buffer[index_in_buffer-1] != '\r'){
			content[index_in_content++] = '\r';
			content[index_in_content] = tmp_buffer[index_in_buffer];
			if (tmp_buffer[index_in_buffer+1] == '.'){
				content[++index_in_content] = '.';
				//content[++index_in_content] = tmp_buffer[index_in_buffer++];
				dots++;
			}
			continue;
		}

		content[index_in_content] = tmp_buffer[index_in_buffer];
	}

	if (content[index_in_content-1] != '\n'){
		content[index_in_content++] = '\r';
		content[index_in_content++] = '\n';
	}

	b_readed = true;
	size = index_in_content;
	free(tmp_buffer);
}

char * MailMsg::get_content() {
    if (!content) read_content();
    return content;
}

MailMsg::MailMsg(std::string path, int id) {
    this->full_path = path;
    this->id = id;
    b_delete = false;
    b_readed = false;
	content = NULL;

    std::regex rgx(R"((.*\/)(cur\/|new\/)(.*))");
    std::smatch match;

    if (std::regex_search(path, match, rgx)){
        this->path      =   match[1].str();
        this->folder    =   match[2].str();
        this->msg_name  =   match[3].str();
    } else {
        throw Exception(4, "Regex not match");
    }

    std::ifstream file(path, std::ios::ate);
    size = file.tellg();
    file.close();

	read_content();

}

void MailMsg::move(std::string destinatin_name) {
    std::string old_name = path + folder + msg_name;
    std::string new_name = path + destinatin_name + msg_name;
    folder = destinatin_name;

    if (rename(old_name.c_str(), new_name.c_str())) {
        throw Exception("Error while renaming file");
    }

	MailDirLogger::add_log(old_name, new_name);

}

MailMsg::~MailMsg() {
	if (content) free(content);

    if (b_delete){
        std::string filename = path + folder + msg_name;

		echo("Deleting file " + filename);
        if (remove(filename.c_str())) {
			echo("Error while deleting");
        }
    }
}

const std::string &MailMsg::get_msg_name() const {
    return msg_name;
}

bool MailMsg::is_delete() const {
    return b_delete;
}

void MailMsg::set_delete() {
    MailMsg::b_delete = true;
}

long MailMsg::getSize() const {
    return size - dots;
}

int MailMsg::getId() const {
    return id;
}

void MailMsg::unset_delete() {
    b_delete = false;
}

std::string MailMsg::get_uidl() {
    return msg_name; //TODO return correct uidl
}

std::string MailMsg::get_folder() {
	return folder;
}

int MailMsg::getEngPosition() {
	return size;
}

