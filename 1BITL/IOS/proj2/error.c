/**************************************
 *   Program :               error.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               03.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               proj2
 *
 *   Notes :        pomocne funkcie pre vypis chybovych hlaseni
 *
 ***************************************/
/**
 * @file error.c
 * @author xtomas32
 * @date 3 Apr 2016
 * @brief pomocne funkcie pre vypis chybovych hlaseni
 */


#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "error.h"
#include "main.h"

//chybove hlasenia indexovane od 0
/**
 * @brief	Pole ukazatelov na chybove hlasenia.
 */
const char *error_strings_array[] = {
        "Zle argumenty na vstupe",                      //0
        "Zly pocet argumentov",                         //1
        "Argumenty su mimo stanovany rozsah",           //2
        "Chyba pri volani fork()",						//3
		"Chyba pri otvarani subara",					//4
		"Chyba pri generovani kluca [ftok]",			//5
		"Chyba pri inicializacii pamate [shmget]",		//6
		"Chyba prisupe k pamati [shmat]",				//7
		"Chyba pri inicializacii semaforov",			//8

};


void print_error_msg(int error_num){
    fprintf(stderr,"%d : %s\n",error_num,error_strings_array[error_num]);
}

void arg_error(int error_num){
    print_error_msg(error_num);
	destroy();
    exit(1);
}

void system_error(int error_num){
    print_error_msg(error_num);

	//aby sa ukoncili vsetky cakajuce procesy
	pControl_variales->counter = input_param.num_of_passnegers;
	for (int i = 0; i < input_param.num_of_passnegers + 3; ++i) {
		sem_post(&pSemaphores->finish);
		sem_post(&pSemaphores->loaded);
		sem_post(&pSemaphores->unloaded);
		sem_post(&pSemaphores->board);
		sem_post(&pSemaphores->unboard);
	}

	//zavrie vsetky alokovane subory
	destroy();

    exit(2);
}