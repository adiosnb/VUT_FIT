/**************************************
 *   Program :               htab_lookup_add.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               07.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC_DU2
 *
 *   Notes :         pripocitanie vyskytu prvku
 *
 ***************************************/
/**
 * @file htab_lookup_add.c
 * @author xtomas32
 * @date 7 Apr 2016
 * @brief pripocitanie vyskytu prvku
 */

#include <stdlib.h>
#include <string.h>
#include "libhtable.h"

struct htab_listitem *htab_lookup_add(htab_t *t, const char *key){
	unsigned id = t->hash_fun_ptr(key,t->htab_size);
	struct htab_listitem *item = t->ptr[id];
	struct htab_listitem *last = NULL;

	if (item == NULL){
		item = malloc(sizeof(struct htab_listitem));
		if (item == NULL)
			return NULL;
		t->n++;
		item->data=0;
		item->next=NULL;
		item->key = malloc(sizeof(char)*strlen(key)+1);
		strcpy(item->key,key);
		t->ptr[id] = item;
	}
	else {
		while(item != NULL){
			if (!strcmp(key,item->key)){
				break;
			}
			last = item;
			item = item->next;
		}

		if (item == NULL){
			item = malloc(sizeof(struct htab_listitem));
			if (item == NULL)
				return NULL;
			t->n++;
			item->data=0;
			item->next=NULL;
			item->key = malloc(sizeof(char)*strlen(key)+1);
			strcpy(item->key,key);

			last->next = item;
		}
	}

	return item;
}
