/**************************************
 *   Program :               steg-decode.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               07.03.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC-DU1
 *
 *   Notes :        Dekodovanie obrazka. Na pixeloch s indexom , ktore je prvocislo
 *                  zoberie najmenej vyznamny bit a zapise ho do buffera na znak,
 *                  ktory po nacitani 8 bit vypise.
 *
 ***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "error.h"
#include "ppm.h"
#include "bit_array.h"
#include "eratosthenes.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        warning_msg("Zle argumenty na vstupe");
        return 1;
    }

    int one_char = 0;
    char bit = 0;
    int bit_count = 0;

    ba_create(primes_arr, 5000 * 5000);
    Eratosthenes(primes_arr);

    struct ppm *image;
    image = ppm_read(argv[1]);
    if (image == NULL) {
        warning_msg("Obrazok sa nenacital\n");
        return 1;
    }

    for (long int i = 2; i < ba_size(primes_arr); i++) {
        if (!(ba_get_bit(primes_arr, i))) {
            bit = (image->data[i]) & (char) 1;
            if (bit) {
                one_char |= 1 << bit_count;
            }
            bit_count++;
            if (bit_count == 8) {
                bit_count = 0;
                if (one_char == '\0') {
                    break;
                }
                if (isprint(one_char)) {
                    printf("%c", one_char);
                }
                else {
                    free(image);
                    fatal_error("Nevypisatelny znak\n Pravdepodobne chyba nacitania\n");
                }
                one_char ^= one_char;
            }
        }
    }

    free(image);
    return 0;
}
