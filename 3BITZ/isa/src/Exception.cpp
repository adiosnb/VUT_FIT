/**************************************
 *   File :                  Exception.cpp
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Created :               15.11.2017
 *   Compiled:               g++ 7.2.1
 *   Project :               popser
 ***************************************/


//
// Created by atomasov on 10/5/17.
//

#include <iostream>
#include "Exception.h"

Exception::Exception(int num, const std::string &msg) : error_num(num), error_msg(msg) {}

Exception::Exception(const std::string &msg) : error_num(1), error_msg(msg) {}

void Exception::print_error() {
    std::cerr << error_msg << std::endl;
}

void Exception::print_and_exit() {
    print_error();
    exit(error_num);
}

const char *Exception::what() const throw (){
    return error_msg.c_str();
}
