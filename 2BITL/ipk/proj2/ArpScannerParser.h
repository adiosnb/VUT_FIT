#ifndef ARP_SCANNER_PARSER
#define ARP_SCANNER_PARSER

#include <iostream>

namespace Parser{

    typedef struct {
        std::string interface   =   "";
        std::string out_file    =   "";
    } arg_t;

    arg_t parse_arguments(int argc, char **argv);

    void check_arguments(arg_t arguments);

}


#endif //ARP_SCANNER_PARSER