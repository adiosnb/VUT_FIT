//
// Created by atomasov on 10/10/17.
//

#ifndef ISA_MAILDIR_H
#define ISA_MAILDIR_H

#include <iostream>
#include <vector>
#include "MailMsg.h"

/**
 * MailDir is responsible to operation inside maildir. After creation
 */
class MailDir {

public:

	std::string path; ///< path to maildir in filesystem

	unsigned int id; ///< counter of created MailMsg object

	std::vector<MailMsg*> msgs; ///< List of all readed MailMsgs



	/**
	 * Constructor
	 * @param path Path to maildir in filesystem
	 */
	MailDir(std::string const &path) : path(path), id(1) {};
	MailDir(std::string const &path, unsigned int id) : path(path), id(id) {};

	/**
	 * Clear memory - delete all dynamic created object
	 */
	~MailDir();

	/**
	 * Read whole mail dir
	 */
    void read_dirs();

	/**
	 * Read only dir specified by *path. This funciton is called by read_dirs().
	 * @param path foder to read
	 * @param msg_q destination list to append new MailMsgs
	 */
    void read_msgs(const char *path, std::vector<MailMsg*> &msg_q);

	/**
	 * Get ptr to requested MailMsg
	 * @param id index to msgs list.
	 * @return ptr to MailMsg
	 */
    MailMsg *get_msg_from_cur_by_id(int id);

};


#endif //ISA_MAILDIR_H
