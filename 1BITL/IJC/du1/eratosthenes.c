/**************************************
 *   Program :               eratosthenes.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               04.03.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC-DU1
 *
 *   Notes :        Zdrojovy subor k modulu Eratosthenes.
 *                  Implementacia eratosthenovho sita na bitovom poli.
 *                  Vstupny parameter ma strukturu bit_array_t.
 *
 ***************************************/

#include "eratosthenes.h"
#include <math.h>

void Eratosthenes(bit_array_t pole) {

    unsigned long sqrt_num_of_bit = round(sqrt(ba_size(pole)));
    for (unsigned long long i = 2; i < sqrt_num_of_bit;) {
        for (unsigned long long j = i + i; j < ba_size(pole); j += i) {
            ba_set_bit(pole, j, 1);
        }
        for (unsigned long long j = i + 1; i < sqrt_num_of_bit; j++) {
            if (!(ba_get_bit(pole, j))) {
                i = j;
                break;
            }
        }

    }
}

