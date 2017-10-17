#include <iostream>
#include <csignal>
#include "ArpScannerParser.h"
#include "Scanner.h"

using namespace std;

Scanner *glob_ref_scanner;

void server_interupt_handler(int sig_num);

int main(int argc, char **argv) {
    signal(SIGINT, server_interupt_handler);
    Parser::arg_t arguments;
    glob_ref_scanner = NULL;

    try {
        arguments = Parser::parse_arguments(argc, argv);
        Parser::check_arguments(arguments);


        Scanner scanner = Scanner();
        glob_ref_scanner = &scanner;
        scanner.set_out_file(arguments.out_file);
        scanner.set_interface_name(arguments.interface);
        scanner.get_ip_interface_data();

        scanner.scan();

        scanner.write_xml_to_file();
    }
    catch (const char *error){
        cerr << error << endl;
    }

    return 0;
}

void server_interupt_handler(int sig_num){
    //glob_ref_scanner->write_xml_to_cout();
    glob_ref_scanner->write_xml_to_file();
    exit(0);
}
