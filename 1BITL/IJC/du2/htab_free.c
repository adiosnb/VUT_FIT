/**************************************
 *   Program :               htab_free.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               07.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC_DU2
 *
 *   Notes :         uvolnenie tabulky
 *
 ***************************************/
/**
 * @file htab_free.c
 * @author xtomas32
 * @date 7 Apr 2016
 * @brief uvolnenie tabulky
 */

#include <stdlib.h>
#include "libhtable.h"


void htab_free(htab_t *t) {
	htab_clear(t);
	free(t);
}
