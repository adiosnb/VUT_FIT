/**************************************
 *   Program :               tail2.cc
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               03.04.2016
 *   Compiled:               g++ 4.8.4
 *   Project :               proj2
 *
 *   Notes :         ze zadaného vstupního souboru vytiskne posledních  10 řádků. Není-li
 *                   zadán vstupní soubor, čte ze stdin. Je-li programu zadán
 *                   parametr -n číslo, bude se tisknout tolik posledních řádků,
 *                   kolik je zadáno parametrem 'číslo'.
 *
 ***************************************/
/**
 * @file tail2.cc
 * @author xtomas32
 * @date 3 Apr 2016
 * @brief ze zadaného vstupního souboru vytiskne posledních  10 řádků. Není-li zadán vstupní soubor, čte ze stdin. Je-li programu zadán parametr -n číslo, bude se tisknout tolik posledních řádků, kolik je zadáno parametrem 'číslo'.
 */

#include <iostream>
#include <queue>
#include <string>
#include <fstream>

using namespace std;

int get_args(int argc, char **argv, string *filename, unsigned long *num_of_lines);

int main(int argc, char **argv) {

	string line;
	string filename = "";
	unsigned long num_of_lines = 10;
	unsigned i = 0;
	ifstream input_file;
	queue<string> lines;


	ios_base::sync_with_stdio(false);

	if (get_args(argc, argv, &filename, &num_of_lines)) {
		cerr << "Zly vstup" << endl;
		return 1;
	}

	if (filename != "") {
		input_file.open(filename);
		if (!input_file.is_open()) {
			cerr << "Chyba pri otvarani suboru" << endl;
			return 1;
		}
		cin.rdbuf(input_file.rdbuf());
		cin.tie();
	}


	while ((getline(cin, line)) && (!cin.eof())) {
		if (lines.size() < num_of_lines) {
			lines.push(line);
		}
		else {
			lines.pop();
			lines.push(line);
		}
	}

	num_of_lines = lines.size();
	for (i = 0; i < num_of_lines; i++) {
		cout << lines.front() << endl;
		lines.pop();
	}


	input_file.close();
	return 0;
}

int get_args(int argc, char **argv, string *filename, unsigned long *num_of_lines) {
	if (argc != 1) {
		bool n_flag = false;
		bool filename_flag = false;

		for (int i = 1; i < argc; i++) {
			string current = argv[i];
			if (current == "-n") {
				if (!n_flag) {
					n_flag = true;
					i++;
					if (i < argc) {
						current = argv[i];
						string::size_type num_of_digit;
						try {
							*num_of_lines = stoul(current, &num_of_digit);
						}
						catch (...) {
							return 1;
						}
						if (current.length() != num_of_digit) {
							return 1;
						}
					}
					else {
						return 1;
					}
				}
				else {
					return 1;
				}
			}
			else {
				if (!filename_flag) {
					filename_flag = true;
					*filename = current;
				}
				else {
					return 1;
				}
			}
		}//for
	}//if(argc!=1)
	return 0;
}