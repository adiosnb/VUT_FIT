/**************************************
 *   File :                  HostConnection.cpp
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Created :               15.11.2017
 *   Compiled:               g++ 7.2.1
 *   Project :               popser
 ***************************************/


//
// Created by atomasov on 10/22/17.
//

#include "HostConnection.h"
#include <unistd.h>

HostConnection::~HostConnection() {
	delete buffer;
	close(connection_fd);
}
