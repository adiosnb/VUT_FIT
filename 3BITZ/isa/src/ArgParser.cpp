/**************************************
 *   File :                  ArgParser.cpp
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Created :               15.11.2017
 *   Compiled:               g++ 7.2.1
 *   Project :               popser
 ***************************************/


//
// Created by atomasov on 10/5/17.
//

#include <getopt.h>
#include <iostream>
#include <fstream>
#include "ArgParser.h"
#include "Exception.h"

void ArgParser::parse() {

    int c;
    //opterr = 0;

    while ((c = getopt(argc, argv, "ha:cp:d:r")) != -1){
        switch (c){
            case 'h':
                help++;
                break;

            case 'a':
                auth_file = optarg;
                break;

            case 'c':
                clear_pass++;
                break;

            case 'p':
                port = std::stoi(optarg);
                break;

            case 'd':
                directory = optarg;
                break;

            case 'r':
                reset++;
                break;

            case '?':
                throw Exception("Get opt found error");
                break;

            default:
                break;
        }
    }

	if (directory.back() != '/' && directory.size()){
		directory += '/';
	}
}

int ArgParser::getHelp() const {
    return help;
}

const std::string &ArgParser::getAuth_file() const {
    return auth_file;
}

int ArgParser::getClear_pass() const {
    return clear_pass;
}

int ArgParser::getPort() const {
    return port;
}

const std::string &ArgParser::getDirectory() const {
    return directory;
}

int ArgParser::getReset() const {
    return reset;
}

void ArgParser::check() {

	// too many args
    if (
        help    >   1   ||
        reset   >   1   ||
        clear_pass  >   1
            )   {
        throw Exception(E_PARSER_CHECK, "Parser check found error");
    }

	if (help){
		throw HelpException();
	}

	if (
		reset 	== 	true	&&
		port	== -1		&&
		auth_file	==	""	&&
		clear_pass	== false &&
		directory	== ""
			) {
		throw ResetAndExit();
	}

	if (
		auth_file 	==	""	||
		port	== -1		||
		directory	==	""
			){
		throw Exception("Mandatory argument is missing!!!");
	}


	std::ifstream infile(auth_file);
	if (!infile.good()) {
		throw Exception("Bad auth file");
	}
	infile.close();

	if (reset){
		throw ResetAndStart();
	}

}

void ArgParser::print_help(){
	std::cout << "-h (help) - voliteľný parameter, pri jeho zadaní sa vypíše nápoveda a program sa ukončí\n"
			"-a (auth file) - cesta k súboru s prihlasovacími údajmi\n"
			"-c (clear pass) - voliteľný parameter, pri zadaní server akceptuje autentizačnú metódu, ktorá prenáša heslo v nešifrovanej podobe (inak prijíma iba heslá v šifrovanej podobe - hash)\n"
			"-p (port) - číslo portu na ktorom bude bežať server\n"
			"-d (directory) - cesta do zložky Maildir (napr. ~/Maildir/)\n"
			"-r (reset) - voliteľný parameter, server vymaže všetky svoje pomocné súbory a emaily z Maildir adresárovej štruktúry vráti do stavu, ako keby proces popser nebol nikdy spustený (netýka sa časových pečiatok, iba názvov a umiestnení súborov) (týka sa to len emailov, ktoré sa v adresárovej štruktúre nachádzajú)."
			  << std::endl;
}

ArgParser::ArgParser(int argc, char **argv) {
    this->argc = argc;
    this->argv = argv;

    this->help = false;
	this->auth_file = "";
	this->clear_pass = false;
	this->port = -1;
	this->directory = "";
	this->reset = false;
}

