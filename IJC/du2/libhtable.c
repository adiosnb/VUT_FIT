/**************************************
 *   Program :               libtable.c
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
 * @file libtable.c
 * @author xtomas32
 * @date 7 Apr 2016
 * @brief knihovna pre pracu s tabullkou
 */

#include <stdlib.h>
#include <string.h>
#include "libhtable.h"

unsigned int hash_function_default(const char *str, unsigned htab_size) {
	unsigned int h = 0;
	const unsigned char *p;
	for (p = (const unsigned char *) str; *p != '\0'; p++)
		h = 65599 * h + *p;
	return h % htab_size;
}

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
	t->hash_fun_ptr = &hash_function_default;

	return t;
}

htab_t *htab_init2(unsigned size, unsigned (*hash_func)(const char *str, unsigned htab_size)) {
	htab_t *table = htab_init(size);
	table->hash_fun_ptr = hash_func;
}

struct htab_listitem *htab_lookup_add(htab_t *t, const char *key){
	unsigned id = t->hash_fun_ptr(key,t->htab_size);
	struct htab_listitem *item = t->ptr[id];
	struct htab_listitem *last;

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

int htab_foreach(htab_t *t, void (*func)(const char *, unsigned)){

	for (unsigned i = 0; i < t->htab_size; i++){
		struct htab_listitem * item;
		for (item = t->ptr[i]; item != NULL; item = item->next){
			func(item->key,item->data);
		}
	}

	return 0;
}

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

void htab_free(htab_t *t) {
	htab_clear(t);
	free(t);
}