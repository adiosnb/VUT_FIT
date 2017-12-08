/**************************************
 *   File :                  MailDirLogger.cpp
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Created :               15.11.2017
 *   Compiled:               g++ 7.2.1
 *   Project :               popser
 ***************************************/


#include <fstream>
#include <regex>

#include "MailDirLogger.h"
#include "debug.h"
#include "Exception.h"

std::string MailDirLogger::logger_path = ".popser_log";

void MailDirLogger::add_log(std::string source, std::string destination) {
	std::fstream file(logger_path, std::ios::app);

	file << source + "^^^" + destination << std::endl;

	file.close();
}

void MailDirLogger::reset() {
	echo("Reseting direcotry");

	std::regex rgx(R"((.*)\^\^\^(.*))");
	std::smatch smatch;

	std::string line, from, to;
	std::ifstream file(logger_path);

	if (file.is_open()) {
		while (std::getline(file, line)) {
			echo("Readed >> " + line);

			if (std::regex_search(line, smatch, rgx)) {
				from = smatch[1].str();
				to = smatch[2].str();

				echo(to + " >>> " + from);

				// file could be deleted so no error handling
				rename(to.c_str(), from.c_str());

			} else {
				throw Exception("Bad reading of logger file");
			}
		}
		// closing file after reading
		file.close();

		// -r flag shoudl delete file
		echo("Deleting logger file");
		if (remove(logger_path.c_str())){
			throw Exception("Error while deleting logger file");
		}
	} else {
		echo("Logger file waas NOT found");
	}
}
