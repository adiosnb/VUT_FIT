/**************************************
 *   Program :               wordcount.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               07.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC_DU2
 *
 *   Notes :         pocitanie slov v subore
 *
 ***************************************/
/**
 * @file wordcount.c
 * @author xtomas32
 * @date 7 Apr 2016
 * @brief knihovna pre pracu s tabullkou
 */

#define MAX_LEN_WORD 127

#include <stdio.h>

#include "libhtable.h"
#include "io.h"

void print_item(const char * str, unsigned data);

int main() {

	char word[MAX_LEN_WORD+1];
	htab_t * table = htab_init(10000);
	struct htab_listitem * list_item = NULL;

	while (get_word(word,MAX_LEN_WORD,stdin) != EOF){
		list_item = htab_lookup_add(table,word);
		if (list_item == NULL){
			fprintf(stderr,"Chyba pri vytvarani prvku\n");
			htab_free(table);
			return 1;
		}
		else {
			list_item->data++;
		}
	}

	htab_foreach(table,&print_item);



	htab_free(table);
	return 0;
}


void print_item(const char * str, unsigned data){
	printf("%s\t%d\n",str,data);
}