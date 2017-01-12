/**************************************
 *   Program :               htab_clear.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               07.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC_DU2
 *
 *   Notes :         vymaze vsetky prvky z tabulky
 *
 ***************************************/
/**
 * @file htab_clear.c
 * @author xtomas32
 * @date 7 Apr 2016
 * @brief vymaze vsetky prvky z tabulky
 */

#include <stdlib.h>
#include "libhtable.h"

void htab_clear(htab_t *t){
	for (unsigned i = 0 ; i < t->htab_size; i++){
		struct htab_listitem * item , *item_next;
		item = t->ptr[i];
		while (item != NULL){
			item_next = item->next;
			free(item->key);
			free(item);
			item = item_next;
		}
	}
}
