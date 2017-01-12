/**************************************
 *   Program :               ppm.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               07.03.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC-DU1
 *
 *   Notes :        Hlavickovy subor modulu Erathostenos.
 *
 ***************************************/

#include <stdio.h>
#include <stdlib.h>

#include "ppm.h"
#include "error.h"

struct ppm *ppm_read(const char *filename) {
    struct ppm *PPM;
    unsigned int x_size, y_size;
    unsigned long num_of_pixels = 0;

    FILE *input;
    input = fopen(filename, "rb");
    if (input == NULL) {
        warning_msg("Neda sa otvorit vstupny subor : %s\n", filename);
        return NULL;
    }

    if (fscanf(input, "P6\n%u %u\n255\n", &x_size, &y_size) != 2) {
        warning_msg("Chyba vstupu\nZly subor na vstupe : %s\n", filename);
        fclose(input);
        return NULL;
    }

    if (x_size * y_size > 5000 * 5000) {
        warning_msg("Prekrocenie maximalnej velkosti obrazka\n");
        fclose(input);
        return NULL;
    }

    PPM = malloc(sizeof(struct ppm) + (sizeof(char) * 3 * x_size * y_size));
    if (PPM == NULL) {
        warning_msg("Malloc nealokoval miesto\n");
        fclose(input);
        return NULL;
    }

    PPM->ysize = y_size;
    PPM->xsize = x_size;


    num_of_pixels = y_size * x_size * 3;

    if (fread(&PPM->data, sizeof(char), num_of_pixels * sizeof(char), input) != num_of_pixels) {
        warning_msg("Zly pocet nacitanych pixelov\n");
        fclose(input);
        free(PPM);
        exit(-1);
    }

    fclose(input);
    return PPM;
}

int ppm_write(struct ppm *p, const char *filename) {

    FILE *output;
    unsigned long num_of_pixels;

    output = fopen(filename, "wb");
    if (output == NULL) {
        warning_msg("Subor sa nepodarilo otvorit");
        return -1;
    }

    if (fprintf(output, "P6\n%d %d\n255\n", p->xsize, p->ysize) < 1) {
        warning_msg("Nepodarilo sa zapisat do suboru %s", filename);
        fclose(output);
        return -1;
    }

    num_of_pixels = p->xsize * p->ysize * 3;
    if (fwrite(p->data, sizeof(char), num_of_pixels, output) != num_of_pixels) {
        warning_msg("NEpodarila sa zapisat do suboru : %s", filename);
        fclose(output);
        return -1;
    }

    fclose(output);
    return 0;
}
