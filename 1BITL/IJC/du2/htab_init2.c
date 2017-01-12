/**************************************
 *   Program :               htab_init2.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               07.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC_DU2
 *
 *   Notes :         konstruktor
 *
 ***************************************/
/**
 * @file htab_init2.c
 * @author xtomas32
 * @date 7 Apr 2016
 * @brief konstruktor
 */

#include <stdlib.h>

#include "libhtable.h"

htab_t *htab_init2(unsigned size, unsigned (*hash_func)(const char *str, unsigned htab_size)){
	htab_t *table = htab_init(size);
	table->hash_fun_ptr = hash_func;
	return table;
}
