/**************************************
 *   Program :               hash_function.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               07.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC_DU2
 *
 *   Notes :         hashovacia funkcia
 *
 ***************************************/
/**
 * @file hash_function.c
 * @author xtomas32
 * @date 7 Apr 2016
 * @brief hashovacia funkcia
 */

#include "libhtable.h"

unsigned int hash_function(const char *str, unsigned htab_size) {
	unsigned int h = 0;
	const unsigned char *p;
	for (p = (const unsigned char *) str; *p != '\0'; p++)
		h = 65599 * h + *p;
	return h % htab_size;
}
