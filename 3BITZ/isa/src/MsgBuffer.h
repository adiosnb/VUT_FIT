//
// Created by atomasov on 10/21/17.
//

#ifndef ISA_MSG_BUFFER_H
#define ISA_MSG_BUFFER_H

#define BUFFER_SIZE 1024

#include <iostream>
#include "Exception.h"

/**
 * Class MsgBuffer - handles recv() and send() and append to internal std::string types queues.
 *
 */
class  MsgBuffer {
private:
	std::string inner_q;	/**< receive queue */
	std::string out_q;		/**< send queue */
	int socket;				/**< host socket fd */

public:
	/**
	 * Constructor of MsgBuffer
	 * @param socket fd of host socket
	 */
	MsgBuffer(int socket);

	/**
	 * Call send_msg() and check if CRLF is included in inner_q.
	 * @return True if whole line is received or false if just part of msg is received.
	 */
	int is_readed();

	/**
	 * Get one line from inner_q. CRLR is used as separator of lines.
	 * @return Substring of inner_q of intervall <0, first CRLF>.
	 */
	std::string get_line();

	/**
	 * Delete inner_q
	 */
	void flush();

	/**
	 * Try ot receive whole msg into buffer then append into inner_q.
	 */
	void recv_msg();

	/**
	 * Send whole msg from outer_q.
	 */
	void send_msg();

	/**
	 * Set string to outer_q
	 * @param msg Msg which might be send.
	 */
	void set_q(std::string msg);

	/**
	 * Send data from external buffer.
	 * @param buffer ptr to external buffer
	 * @param size num of bytes, which will be send
	 */
	void direct_send(SendMailMsg &exc);

	void direct_send_lines(SendMailMsgLines &exc);
};


#endif //ISA_MSG_BUFFER_H
