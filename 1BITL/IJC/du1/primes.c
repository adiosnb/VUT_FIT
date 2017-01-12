/**************************************
 *   Program :               primes.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               04.03.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC-DU1
 *
 *   Notes :        Zdrojovy subor k modulu primes.
 *                  Tetno modul pocita prvocisla s pomocou eratosthenosovho sita.
 *                  Poslednych 10 prvocisel vypise na stdout.
 *
 ***************************************/

#include <stdio.h>

#include "bit_array.h"
#include "eratosthenes.h"

#define NUM_OF_BITS 202000000

int main() {
    unsigned long long primes[10];
    int pole_index = 9;

    ba_create(era_arr, NUM_OF_BITS);

    Eratosthenes(era_arr);

    for (unsigned long long i = ba_size(era_arr) - 1; i > 0; i--) {
        if (!(ba_get_bit(era_arr, i))) {
            primes[pole_index] = i;
            pole_index--;
            if (pole_index == -1) {
                break;
            }
        }
    }

    for (pole_index = 0; pole_index < 10; pole_index++) printf("%llu\n", primes[pole_index]);

    return 0;
}
