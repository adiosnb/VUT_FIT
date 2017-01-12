/**************************************
 *   Program :               ppm.h
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               07.03.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               IJC-DU1
 *
 *   Notes :        Hlavickovy subor modulu ppm.
 *                  Je tu definovana struktura pre obrazok ppm a operacie pre nacitanie a zapis obrazka
 *
 ***************************************/
#ifndef IJC_1DU_1_PPM_H
#define IJC_1DU_1_PPM_H

struct ppm {
    unsigned xsize;
    unsigned ysize;
    char data[];    // RGB bajty, celkem 3*xsize*ysize
};


struct ppm *ppm_read(const char *filename);


int ppm_write(struct ppm *p, const char *filename);


#endif //IJC_1DU_1_PPM_H
