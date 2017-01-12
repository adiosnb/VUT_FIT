/**************************************
 *   Program :               htab_init.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               07.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC_DU2
 *
 *   Notes :         knihovna pre pracu s tabullkou
 *
 ***************************************/
/**
 * @file htab_init.c
 * @author xtomas32
 * @date 7 Apr 2016
 * @brief knihovna pre pracu s tabullkou
 */

#include <stdlib.h>

#include "libhtable.h"

htab_t *htab_init(unsigned size){
	htab_t *t;

	t = malloc(sizeof(htab_t) + (size * sizeof(struct htab_listitem *)));
	if (t == NULL){
		return NULL;
	}

	t->n = 0;
	t->htab_size = size;
	for (unsigned i = 0 ; i < t->htab_size; i++){
		t->ptr[i] = NULL;
	}
	t->hash_fun_ptr = hash_function;

	return t;
}
