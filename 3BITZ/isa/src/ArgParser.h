//
// Created by atomasov on 10/5/17.
//

#ifndef ISA_ARGPARSER_H
#define ISA_ARGPARSER_H

#include <string>

/**
 * class ArgParser - this class is used to read and parser aruments given in cmd line.
 */
class ArgParser {


private:
    int help;
    std::string auth_file;
    int clear_pass;
    int port;
    std::string directory;
    int reset;
    int argc;
    char **argv;

public:

    ArgParser(int argc, char **argv);

    virtual ~ArgParser() = default;

    void parse();

    void check();

    int getHelp() const;

    const std::string &getAuth_file() const;

    int getClear_pass() const;

    int getPort() const;

    const std::string &getDirectory() const;

    int getReset() const;

    void print_help();

};


#endif //ISA_ARGPARSER_H
