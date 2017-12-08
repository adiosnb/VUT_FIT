//
// Created by atomasov on 10/10/17.
//

#ifndef ISA_MAILMSG_H
#define ISA_MAILMSG_H

#include <iostream>

/**
 * class MailMsg - abstract object which corresponding to mail in system. It stores path to mail, folder in mail dir and
 * once mail is read it is stored in spacialized variable.
 */
class MailMsg {

private:
	char* content;				///< ptr to read file in char array. File is read only once, then is read from this var.
	std::string full_path;		///< full path on file system to mail file.
    std::string path;			///< path to mail dir
    std::string folder;			///< folder in mail dir
    std::string msg_name;		///< name of mail

	int dots;					///< additional dots added
    int id;						///< ID of mail in pop3server
    long size;					///< size of mail file on file system
    bool b_delete;				///< flag indicating if file should be deleted
    bool b_readed;				///< flag indicating if file is readed in content ptr

	/**
	 * Read mail file from file system to memory.
	 */
    void read_content();

public:

    int getId() const;

    long getSize() const;

    bool is_delete() const;

    void set_delete();

    void unset_delete();

    std::string get_uidl();

	std::string get_folder();

	int getEngPosition();

    const std::string &get_msg_name() const;

	/**
	 * Constructor of MailMsg. User regex to recognize in what kind of directory is mail saved. It splits path using
	 * match group and sotore into specific attributes.
	 * @param path Path in file system to mail.
	 * @param id ID of mail assigned by MailDir object.
	 */
    MailMsg(std::string path, int id);

    ~MailMsg();

    char * get_content();

	/**
	 * Move mail from actual dir in mail dir to another dir specified byt destination_name
	 * @param destinatin_name
	 */
    void move(std::string destinatin_name);

};


#endif //ISA_MAILMSG_H
