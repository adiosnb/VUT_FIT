/**************************************
 *   Program :               htab_foreach.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               07.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC_DU2
 *
 *   Notes :         kazdy prvok pola preda funkcii
 *
 ***************************************/
/**
 * @file htab_foreach.c
 * @author xtomas32
 * @date 7 Apr 2016
 * @brief kazdy prvok pola preda funkcii
 */

#include "libhtable.h"
#include <stdlib.h>

int htab_foreach(htab_t *t, void (*func)(const char *, unsigned)){

	for (unsigned i = 0; i < t->htab_size; i++){
		struct htab_listitem * item;
		for (item = t->ptr[i]; item != NULL; item = item->next){
			func(item->key,item->data);
		}
	}

	return 0;
}
