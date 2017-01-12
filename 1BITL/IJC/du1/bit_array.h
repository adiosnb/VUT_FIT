/**************************************
 *   Program :               bit_array.h
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               04.03.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC-DU1
 *
 *   Notes :        Hlavickovy subor pre strukturu bit_array_t a pre zakladne operacie na nej.
 *                  Na zaklade podmieneho prekladu su pouzite bud makra alebo inline funkcie.
 *
 ***************************************/

#ifndef IJC_1DU_1_BIT_ARRAY_H
#define IJC_1DU_1_BIT_ARRAY_H

#include "error.h"

typedef unsigned long bit_array_t[];

#define DU1_GET_BIT_(p, i) ((p[(i/(sizeof(long)*8))+1] & (unsigned long)1 << (i%(sizeof(long)*8))) ?1:0)
#define DU1_SET_BIT_(p, i, b) {                                                       \
        if(b)   p[(i/(sizeof(long)*8))+1] |= (unsigned long)1 << (i%(sizeof(long)*8));      \
        else    p[(i/(sizeof(long)*8))+1] &= ~( (unsigned long)1 << (i%(sizeof(long)*8)));  \
}

#define ba_create(jmeno_pole, velikost) unsigned long jmeno_pole[(velikost/(sizeof(long)*8))+2] = {velikost,0}

#ifndef USE_INLINE

#define ba_size(jmeno_pole) (jmeno_pole[0])

#define ba_set_bit(jmeno_pole, index, vyraz) {                                      \
    if(index < ba_size(jmeno_pole)){                                                \
        DU1_SET_BIT_(jmeno_pole,index,vyraz)                                        \
    }                                                                               \
    else {                                                                          \
        fatal_error("Setting bit out of range >>> index : %d !!!\n", index);        \
    };                                                                              \
}

#define ba_get_bit(jmeno_pole, index) (!(index < ba_size(jmeno_pole))) ? \
    fatal_error("Index %ld mimo rozsah 0..%ld", index, ba_size(jmeno_pole)),0 \
    :\
    (DU1_GET_BIT_(jmeno_pole, index))

#else

static inline long  ba_size(bit_array_t jmeno_pole)  {
        return jmeno_pole[0];
}

static inline void ba_set_bit(bit_array_t jmeno_pole, unsigned long index,short vyraz){
        if (index < jmeno_pole[0]) {
                DU1_SET_BIT_(jmeno_pole, index, vyraz);
        }
        else {
                fatal_error("Setting bit out of range >>> index : %d !!!\n", index);
        }
}


static inline int ba_get_bit(bit_array_t jmeno_pole, unsigned long index) {
        if (index < ba_size(jmeno_pole)) {
                return DU1_GET_BIT_(jmeno_pole, index);
        }
        else {
                fatal_error("Accessing bit out of range >>> index : %d !!!\n",index);
        }
        return 0;
}

#endif //USE_LINE
#endif //IJC_1DU_1_BIT_ARRAY_H
