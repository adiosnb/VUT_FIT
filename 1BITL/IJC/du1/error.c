/**************************************
 *   Program :               error.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               06.03.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC-DU1
 *
 *   Notes :        Zdrojovy subor k modulu error. Obsahuje funkcie pre vypis na stderr pripadne
 *                  ukonci program po chybovom hlaseni
 *
 *
 ***************************************/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


void warning_msg(const char *fmt, ...) {
    va_list args;
    va_start(args,fmt);
    fprintf(stderr,"CHYBA: ");
    vfprintf(stderr,fmt,args);
    va_end(args);
}


void fatal_error(const char *fmt, ...) {
    va_list args;
    va_start(args,fmt);
    fprintf(stderr,"CHYBA: ");
    vfprintf(stderr,fmt,args);
    va_end(args);

    exit(1);
}