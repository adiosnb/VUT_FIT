/**************************************
 *   Program :               htab_remove.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               07.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC_DU2
 *
 *   Notes :         odstrani prvok z tabulky
 *
 ***************************************/
/**
 * @file htab_remove.c
 * @author xtomas32
 * @date 7 Apr 2016
 * @brief odstrani prvok z tabulky
 */

#include <stdlib.h>
#include <string.h>
#include "libhtable.h"

int htab_remove(htab_t *t, const char *key){
	unsigned id = t->hash_fun_ptr(key,t->htab_size);
	struct htab_listitem *item = t->ptr[id];
	struct htab_listitem *last = NULL;

	if (item == NULL){
		return -1;
	}

	while (item != NULL) {

		if (!strcmp(key,item->key)){
			if (last != NULL) {
				last->next = item->next;
			}
			free(item);

			return 1;
		}

		last = item;
		item=item->next;
	}

	return 0;
}
