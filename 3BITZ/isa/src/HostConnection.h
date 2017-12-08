//
// Created by atomasov on 10/22/17.
//

#ifndef ISA_HOSTCONNECTION_H
#define ISA_HOSTCONNECTION_H

#include <netinet/in.h>
#include "MsgBuffer.h"

/**
 * class HostConnection - stores information about connected host
 */
class HostConnection {
public:

	/**
	 * Destructor of this object. Calling close() on host host socket.
	 */
	~HostConnection();

	int connection_fd;				/**< socket fd of host */
	struct sockaddr_in host_addr;	/**< host address */
	socklen_t socklen;

	MsgBuffer *buffer;				/**< Object which handles recv() and send() ops*/

};


#endif //ISA_HOSTCONNECTION_H
