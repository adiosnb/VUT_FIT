/**************************************
 *   Program :               libtable.h
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
 * @file libtable.h
 * @author xtomas32
 * @date 7 Apr 2016
 * @brief knihovna pre pracu s tabullkou
 */

#ifndef IJC_DU2_LIBHTABLE_H
#define IJC_DU2_LIBHTABLE_H


typedef struct htab_table {
	unsigned htab_size;									/**< velkost pola ukazatelov na struct htab_listitem */
	unsigned (*hash_fun_ptr)(const char *, unsigned);	/**< ukazatel na hash funkciu */
	unsigned n;											/**< aktualni pocet zaznamov*/
	struct htab_listitem *ptr[];
} htab_t;


struct htab_listitem {
	char *key;							/**<  ukazatel na dynamicky alokovaný řetězec */
	unsigned data; 							/**<  počet výskytů */
	struct htab_listitem *next;			/**<  ukazatel na další záznam */
};


unsigned int hash_function(const char *str, unsigned htab_size);

htab_t *htab_init(unsigned size);

htab_t *htab_init2(unsigned size, unsigned (*hash_func)(const char *str, unsigned htab_size));

struct htab_listitem *htab_lookup_add(htab_t *t, const char *key);

int htab_foreach(htab_t *t, void (*func)(const char *, unsigned));

int htab_remove(htab_t *t, const char *key);

void htab_clear(htab_t *t);

void htab_free(htab_t *t);

unsigned int hash_function(const char *str, unsigned htab_size);

#endif //IJC_DU2_LIBHTABLE_H
