/**************************************
 *   Program :               io.h
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               08.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC_DU2
 *
 *   Notes :         nacitanie zo subora
 *
 ***************************************/
/**
 * @file io.h
 * @author xtomas32
 * @date 8 Apr 2016
 * @brief nacitanie zo subora
 */

#ifndef IJC_DU2_IO_H
#define IJC_DU2_IO_H

#include <stdio.h>

/**
 * @brief Nacita slovo zo stdin, oddelovac je medzera
 * @param[out] *s ukazatel na pole kam zapise slovo
 * @param[in] *f  vstupny subor
 * @return pocet pismen v slove
 */
int get_word(char *s, int max, FILE *f);

#endif //IJC_DU2_IO_H
