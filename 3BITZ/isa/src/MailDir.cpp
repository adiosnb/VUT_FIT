/**************************************
 *   File :                  MailDir.cpp
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Created :               15.11.2017
 *   Compiled:               g++ 7.2.1
 *   Project :               popser
 ***************************************/


//
// Created by atomasov on 10/10/17.
//

#include <dirent.h>
#include <cstring>
#include <algorithm>

#include "MailDir.h"
#include "Exception.h"

MailDir::~MailDir() {
    MailMsg *p_tmp;
    while (!msgs.empty()){
        p_tmp = msgs[msgs.size()-1];
        msgs.pop_back();
        delete p_tmp;
    }
}

void MailDir::read_msgs(const char *path, std::vector<MailMsg*> &msg_q) {
	std::vector<std::string> files;

    DIR *dp;
    struct dirent *dirp;

    if((dp  = opendir(path)) == NULL) {
        if (errno == ENOTDIR) Exception("Not a directory.");
        else throw  Exception("Directory not found");
    }

    while ((dirp = readdir(dp)) != NULL) {
        if (
                strcmp(dirp->d_name, ".") ==  0   ||
                strcmp(dirp->d_name, "..")  ==  0) {
            continue;
        }
		files.push_back(path+std::string(dirp->d_name));
    }
    closedir(dp);

	std::sort(files.begin(), files.end());


	for (auto &file:files){
		msg_q.push_back(new MailMsg(file,id++));
	}

	while (!files.empty()) files.pop_back();
}

void MailDir::read_dirs() {
	std::string path_2_cur = this->path + "cur/";
	read_msgs(path_2_cur.c_str(), msgs);

    std::string path_2_new = this->path + "new/";
    read_msgs(path_2_new.c_str(), msgs);
	for (auto &item:msgs){
		if (item->get_folder() == "new/") {
			item->move("cur/");
		}
	}
}

MailMsg *MailDir::get_msg_from_cur_by_id(int id) {
    return msgs[id - 1];
}
