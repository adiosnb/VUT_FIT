/**************************************
 *   Program :               error.h
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               06.03.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC-DU1
 *
 *   Notes :        Hlavickovy subor k modulu error. Su to deklarovane dve funckie tohto modulu
 *
 *
 ***************************************/

#ifndef IJC_1DU_1_ERROR_H
#define IJC_1DU_1_ERROR_H

void warning_msg(const char *fmt, ...);

void fatal_error(const char *fmt, ...);

#endif //IJC_1DU_1_ERROR_H
