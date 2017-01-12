/**************************************
 *   Program :               tail.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               03.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               proj2
 *
 *   Notes :         ze zadaného vstupního souboru vytiskne posledních  10 řádků. Není-li
 *                   zadán vstupní soubor, čte ze stdin. Je-li programu zadán
 *                   parametr -n číslo, bude se tisknout tolik posledních řádků,
 *                   kolik je zadáno parametrem 'číslo'.
 *
 ***************************************/
/**
 * @file tail.c
 * @author xtomas32
 * @date 3 Apr 2016
 * @brief ze zadaného vstupního souboru vytiskne posledních  10 řádků. Není-li zadán vstupní soubor, čte ze stdin. Je-li programu zadán parametr -n číslo, bude se tisknout tolik posledních řádků, kolik je zadáno parametrem 'číslo'.
 */

#define MAX_CHAR_PER_LINE 513		//skutocny pocet + 3
#define MAX_FILE_NAME 256

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

int get_args(int argc, char **argv, int *num_of_lines, char *file);


int main(int argc, char **argv) {
	FILE *input_files;
	char **lines;
	char line[MAX_CHAR_PER_LINE + 1] = {'\0'};
	int num_of_lines = 10;
	int counter = 0;
	char filename[MAX_FILE_NAME] = "";
	char char_from_file;

	short overflow_printed_flag = 0;

	if (get_args(argc, argv, &num_of_lines, filename)) {
		fprintf(stderr, "Zle argumenty na vstupe\n");
		return 1;
	}

	if (strcmp("", filename)) {
		if ((input_files = fopen(filename, "r")) == NULL) {
			fprintf(stderr, "Subor sa neda otvorit\n");
			return 1;
		}
	}
	else {
		if ((input_files = stdin) == NULL) {
			fprintf(stderr, "Subor sa neda otvorit\n");
			return 1;
		}
	}

	lines = malloc(num_of_lines * sizeof(char *));
	if (lines == NULL) {
		fprintf(stderr, "Malloc Error\n");
		return 1;
	}
	for (int i = 0; i < num_of_lines; i++) {
		lines[i] = malloc(sizeof(char) * (MAX_CHAR_PER_LINE - 1));
		if (lines[i] == NULL) {
			fprintf(stderr, "Malloc Error\n");
			return 1;
		}
	}

	while (fgets(line, MAX_CHAR_PER_LINE, input_files) != NULL) {

		//printf("%s",line);
		if (line[MAX_CHAR_PER_LINE-2] != '\0') {
			line[MAX_CHAR_PER_LINE-2] = '\0';
			line[MAX_CHAR_PER_LINE-3] = '\n';
			if (!overflow_printed_flag) {
				overflow_printed_flag=1;
				fprintf(stderr, "Prekroceny limit dlzky riadka\n");
			}
			while((char_from_file = getc(input_files)) != '\n');
		}

		strncpy(lines[counter], line, MAX_CHAR_PER_LINE - 2);

		counter++;
		counter %= num_of_lines;
	}

	for (int i = 0; i < num_of_lines; i++, counter++, counter %= num_of_lines) {
		printf("%s", lines[counter]);
	}

	for (int i = 0; i < num_of_lines; i++) {
		free(lines[i]);
	}

	free(lines);
	fclose(input_files);
	return 0;
}//main


int get_args(int argc, char **argv, int *num_of_lines, char *file) {
	if (argc != 1) {
		int state = 0;
		long int tmp;
		char *pEnd = NULL;

		for (int i = 1; i < argc; i++) {
			switch (state) {
				case 0:
					if (!strncmp(argv[i], "-n", 3)) {
						state = 1;
						if (argc == i + 1) return 1;
					} else {
						sscanf(argv[i], "%255s", file);
						state = 3;
					}
					break;

				case 1:
					errno = 0;
					tmp = strtol(argv[i], &pEnd, 10);
					if ((errno == ERANGE) || (tmp > INT_MAX) || (pEnd[0] != '\0')) {
						return 1;
					}
					*num_of_lines = (int) tmp;
					state = 2;
					break;

				case 2:
					if (argc != i + 1) {
						return 1;
					} else {
						sscanf(argv[i], "%255s", file);
					}
					break;

				case 3:
					if (!strncmp(argv[i], "-n", 3)) {
						state = 4;
						if (argc == i + 1) return 1;

					} else {
						return 1;
					}
					break;

				case 4:
					if (argc != i + 1) {
						return 1;
					}
					errno = 0;
					tmp = strtol(argv[i], &pEnd, 10);
					if ((errno == ERANGE) || (tmp > INT_MAX) || (pEnd[0] != '\0')) {
						return 1;
					}
					*num_of_lines = (int) tmp;

					break;
			}//switch
		}//for
	}//argc!=1


	return 0;
}//get_args
