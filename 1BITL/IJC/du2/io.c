/**************************************
 *   Program :               io.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               08.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC_DU2
 *
 *   Notes :         nacitanie zo stdin
 *
 ***************************************/
/**
 * @file io.c
 * @author xtomas32
 * @date 8 Apr 2016
 * @brief nacitanie zo stdin
 */

#include <stdio.h>
#include <ctype.h>

#include "io.h"

int get_word(char *s, int max, FILE *f) {

	int c;
	int num_of_chars = 0;


	c = getc(f);
	while (isspace(c)) c = getc(f);

	do {
		*s++ = (char) c;
		num_of_chars++;
		if (num_of_chars >= max) break;
		c = getc(f);
		if (feof(f)) return EOF;
	} while (!isspace(c));
	*s='\0';



	return num_of_chars;
}