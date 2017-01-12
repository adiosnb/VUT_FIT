/**************************************
 *   Program :               main.c
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               03.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               proj2
 *
 *   Notes :        zaciatok programu
 *
 ***************************************/
/**
 * @file main.c
 * @author xtomas32
 * @date 3 Apr 2016
 * @brief Main code of program
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <errno.h>
#include <limits.h>
#include <semaphore.h>
#include "error.h"
#include "globals.h"
#include "main.h"


/************************************************************
 * Globalne premenne
 */

control_variable_t 	*pControl_variales	=	NULL;
semaphores_t 		*pSemaphores		=	NULL;

input_param_t input_param;

FILE * output_file = NULL;

int control_var_id 	= 	-1;
int semaphore_id 	= 	-1;




int main(int argc,char **argv) {

	srand((unsigned)(time(NULL)));

	init(argc,argv);

	pid_t return_value_fork;

	//vytvorenie procesu pre vozik
	return_value_fork = fork();
	if (return_value_fork == SYS_ERROR){
		system_error(E_FORK_ERROR);
	}
	if(return_value_fork == FORK_CHILD_PROCESS){
		car();
		destroy();
		exit(EXIT_SUCCESS);
	}

	//vytvorenie pasazierov do vozika
	return_value_fork = fork();
	if (return_value_fork == SYS_ERROR){
		system_error(E_FORK_ERROR);
	}
	if(return_value_fork == FORK_CHILD_PROCESS){
		for (int i = 0; i < input_param.num_of_passnegers; i++) {
			//cas za ktory je generovany novy proces pasaziera
			if (input_param.passenger_load_max_time) {
				unsigned time = (unsigned)(rand() % input_param.passenger_load_max_time);
				//printf("%d\n",time);
				usleep(time*1000);
			}
			return_value_fork = fork();
			if (return_value_fork == SYS_ERROR){
				system_error(E_FORK_ERROR);
			}
			if(return_value_fork == FORK_CHILD_PROCESS){
				passenger(i+1);
				destroy();
				exit(EXIT_SUCCESS);
			}
		}
		sem_wait(&pSemaphores->finish);
		destroy();
		exit(EXIT_SUCCESS);
	}

	sem_wait(&pSemaphores->finish);
	destroy();
	return 0;
}


input_param_t parse_arguments_to_struct(int argc,char **argv){
    input_param_t tmp_param;
    long int input_tmp_num;
    char *end_tmp = NULL;

    if (argc != 5){     //ak je zly pocet argumentov ukonci
        arg_error(E_WRONG_NUM_OF_ARGS);
    }

    //nacita prvu hodnotu z argumentov
    errno = 0;
    input_tmp_num = strtol(argv[1],&end_tmp,10);        //nacitanie prveho stringu
    if (    (errno == ERANGE) ||                        //ak je mimo rozsah
            (end_tmp != NULL && end_tmp[0] != '\0' ) || //ak za nacitanym cislom nieco nasledovalo
            (input_tmp_num <= 0) ||                     //ak je pocet pasazierov mensi ako nula
            (input_tmp_num > INT_MAX)){                 //ak je cislo vacsie ako int max
        arg_error(E_WRONG_ARG);             //ukonci program
    }
    tmp_param.num_of_passnegers = (int)input_tmp_num;    //nastavi pocet pasazierov

    //nacita druhu hodnotu z argumentov
    errno = 0;
    input_tmp_num = strtol(argv[2],&end_tmp,10);        //nacitanie druheho stringu
    if (    (errno == ERANGE) ||                        //ak je mimo rozsah
            (end_tmp != NULL && end_tmp[0] != '\0') ||  //ak za nacitanym cislom nieco nasledovalo
            (input_tmp_num <= 0) ||                     //ak je pocet pasazierov mensi ako nula
            (input_tmp_num > INT_MAX)||                 //ak je cislo vacsie ako int max
            (tmp_param.num_of_passnegers%input_tmp_num != 0)){ //ak pocet pasazierov nie je nasobkom kapacity vozika
        arg_error(E_WRONG_ARG);             //ukonci program
    }
    tmp_param.car_max_load = (int)input_tmp_num;    //nastavi kapacitu vozika

    //nacita tretiu hodnotu z argumentov
    errno = 0;
    input_tmp_num = strtol(argv[3],&end_tmp,10);        //nacitanie tretieho stringu
    if (    (errno == ERANGE) ||                        //ak je mimo rozsah
            (end_tmp != NULL && end_tmp[0] != '\0') ||  //ak za nacitanym cislom nieco nasledovalo
            (input_tmp_num < 0) ||                      //ak je pocet pasazierov mensi ako nula
            (input_tmp_num > INT_MAX)){                 //ak je cislo vacsie ako int max
        arg_error(E_WRONG_ARG);             //ukonci program
    }
    if( input_tmp_num > 5000){      //skontroluje rozsah hodnot
        arg_error(E_ARG_OUT_OF_RANGE);
    }
    tmp_param.passenger_load_max_time = (int)input_tmp_num;    //nastavi hodnotu za aku je generovany novy proces

    //nacita stvrtu hodnotu z argumentov
    errno = 0;
    input_tmp_num = strtol(argv[4],&end_tmp,10);        //nacitanie stvrteho stringu
    if (    (errno == ERANGE) ||                        //ak je mimo rozsah
            (end_tmp != NULL && end_tmp[0] != '\0') ||  //ak za nacitanym cislom nieco nasledovalo
            (input_tmp_num < 0) ||                      //ak je pocet pasazierov mensi ako nula
            (input_tmp_num > INT_MAX)){                 //ak je cislo vacsie ako int max
        arg_error(E_WRONG_ARG);             //ukonci program
    }
    if(input_tmp_num > 5000){      //skontroluje rozsah hodnot
        arg_error(E_ARG_OUT_OF_RANGE);
    }
    tmp_param.car_run_max_time = (int)input_tmp_num;    //nastavi hodnotu za aku prejde vozik drahu

    return tmp_param;
}//input_param_t parse_arguments_to_struct(int argc,char **argv);end

void init(int argc,char **argv){
	key_t control_var_key;
	key_t semaphore_key;

	//ziskanie parametrov zo vstupu
	input_param = parse_arguments_to_struct(argc,argv);


	//otvorenie vystupneho subora
	output_file= fopen("proj2.out","w");
	if (output_file == NULL){
		system_error(E_FOPEN_ERROR);
	}

	//generovanie kluca pre zdielanu pamat
	control_var_key = ftok("proj2.out",1);
	if (control_var_key == SYS_ERROR){
		system_error(E_FTOK_ERROR);
	}
	semaphore_key = ftok("proj2.out",2);
	if (semaphore_key == SYS_ERROR){
		system_error(E_FTOK_ERROR);
	}

	//inicializacia zdielanej pamate
	control_var_id = shmget(control_var_key,sizeof(control_variable_t),IPC_CREAT | 0666);
	if (control_var_id == SYS_ERROR){
		system_error(E_SHMGET_ERROR);
	}
	semaphore_id = shmget(semaphore_key,sizeof(semaphores_t),IPC_CREAT | 0666);
	if (semaphore_id == SYS_ERROR){
		system_error(E_SHMGET_ERROR);
	}

	//ziskanie ukazatela na zdielanu pamat
	pControl_variales= shmat(control_var_id,0,0);
	if (pControl_variales == (void *) -1) {
		system_error(E_SHMAT_ERROR);
	}
	pSemaphores = shmat(semaphore_id,0,0);
	if (pSemaphores == (void *) -1) {
		system_error(E_SHMAT_ERROR);
	}

	//inicializacia riadiacih parametrov na pociatocne hodnoty
	pControl_variales->num_of_action		= 	0;
	pControl_variales->boarded				= 	0;
	pControl_variales->counter 				= 	0;
	pControl_variales->finished				=	0;


	//inicializacia semaforov

	if (sem_init(&pSemaphores->write_to_file,1,1) 	== 	SYS_ERROR){
		system_error(E_SEM_INIT);
	}
	if (sem_init(&pSemaphores->board,1,0) 	== 	SYS_ERROR){
		system_error(E_SEM_INIT);
	}
	if (sem_init(&pSemaphores->unboard,1,0) 	== 	SYS_ERROR){
		system_error(E_SEM_INIT);
	}
	if (sem_init(&pSemaphores->loaded,1,0) 	== 	SYS_ERROR){
		system_error(E_SEM_INIT);
	}
	if (sem_init(&pSemaphores->unloaded,1,0) 	== 	SYS_ERROR){
		system_error(E_SEM_INIT);
	}
	if (sem_init(&pSemaphores->finish,1,0) 	== 	SYS_ERROR){
		system_error(E_SEM_INIT);
	}

}


void destroy(){
	//zavrie subor ak je otvoreny
	if (output_file != NULL) {
		fclose(output_file);
	}

	//zavrie zdieanu pamat
	if (pControl_variales != NULL) {
		shmdt(pControl_variales);
	}
	if (pSemaphores != NULL) {
		shmdt(pSemaphores);
	}

	//vymaze zdielanu pamat
	if (control_var_id != -1) {
		shmctl(control_var_id, IPC_RMID, NULL);
	}
	if (semaphore_id != -1) {
		shmctl(semaphore_id, IPC_RMID, NULL);
	}
}


void car(){
	//proces vozika zacal
	sem_wait(&pSemaphores->write_to_file);
	fprintf(output_file,"%d\t: C 1\t: started\n",++pControl_variales->num_of_action);
	fflush(output_file);
	sem_post(&pSemaphores->write_to_file);

	//cyklus, v ktorom sa vsetky procesy odvezu vo voziku
	while ( pControl_variales->counter < input_param.num_of_passnegers ){
		sem_wait(&pSemaphores->write_to_file);
		fprintf(output_file,"%d\t: C 1\t: load\n",++pControl_variales->num_of_action);
		fflush(output_file);
		sem_post(&pSemaphores->write_to_file);

		//zaciatok nastupovania
		for (int i = 0; i <input_param.car_max_load ; ++i) sem_post(&pSemaphores->board);
		//caka kym nebudu vsetci nastupia
		sem_wait(&pSemaphores->loaded);

		sem_wait(&pSemaphores->write_to_file);
		fprintf(output_file,"%d\t: C 1\t: run\n",++pControl_variales->num_of_action);
		fflush(output_file);
		sem_post(&pSemaphores->write_to_file);

		//prechod vozika drahou
		if (input_param.car_run_max_time){
			usleep((unsigned)(rand() % input_param.car_run_max_time)*1000);
		}


		sem_wait(&pSemaphores->write_to_file);
		fprintf(output_file,"%d\t: C 1\t: unload\n",++pControl_variales->num_of_action);
		fflush(output_file);
		sem_post(&pSemaphores->write_to_file);

		//zaciatok vystupovania
		for (int i = 0; i <input_param.car_max_load ; ++i) sem_post(&pSemaphores->unboard);
		//caka kym vsetci vystupia
		sem_wait(&pSemaphores->unloaded);
	}

	//cakanie na synchronne ukoncenie vsetkych procesov
	sem_wait(&pSemaphores->finish);

	sem_wait(&pSemaphores->write_to_file);
	fprintf(output_file,"%d\t: C 1\t: finished\n",++pControl_variales->num_of_action);
	fflush(output_file);
	sem_post(&pSemaphores->write_to_file);


	return;
}

void passenger(int i) {
	sem_wait(&pSemaphores->write_to_file);
	//setbuf()
	fprintf(output_file,"%d\t: P %d\t: started\n",++pControl_variales->num_of_action,i);
	fflush(output_file);
	sem_post(&pSemaphores->write_to_file);

	//caka na signal k nastupeniu
	sem_wait(&pSemaphores->board);

	//nastupi do vozika
	sem_wait(&pSemaphores->write_to_file);
	pControl_variales->counter++;
	fprintf(output_file,"%d\t: P %d\t: board\n",++pControl_variales->num_of_action,i);
	fflush(output_file);
	sem_post(&pSemaphores->write_to_file);

	//napise poradie v ktorom nastupil
	sem_wait(&pSemaphores->write_to_file);
	pControl_variales->boarded += 1;
	if (pControl_variales->boarded >= input_param.car_max_load ){
		fprintf(output_file,"%d\t: P %d\t: board last\n",++pControl_variales->num_of_action,i);
		fflush(output_file);
		sem_post(&pSemaphores->loaded);
	}
	else {
		fprintf(output_file,"%d\t: P %d\t: board order %d\n",++pControl_variales->num_of_action,i,pControl_variales->boarded);
		fflush(output_file);
	}
	sem_post(&pSemaphores->write_to_file);


	sem_wait(&pSemaphores->unboard);

	//vystupi
	sem_wait(&pSemaphores->write_to_file);
	fprintf(output_file,"%d\t: P %d\t: unboard\n",++pControl_variales->num_of_action,i);
	fflush(output_file);
	sem_post(&pSemaphores->write_to_file);

	//vypise poradie v ktorom vystupil
	sem_wait(&pSemaphores->write_to_file);
	pControl_variales->boarded -= 1;
	if (pControl_variales->boarded > 0 ){
		fprintf(output_file,"%d\t: P %d\t: unboard order %d\n",++pControl_variales->num_of_action,i,input_param.car_max_load - pControl_variales->boarded);
		fflush(output_file);
	}
	else {
		fprintf(output_file,"%d\t: P %d\t: unboard last\n",++pControl_variales->num_of_action,i);
		fflush(output_file);
		sem_post(&pSemaphores->unloaded);
	}
	sem_post(&pSemaphores->write_to_file);

	//zvysi pocet procesov cakajucich na ukoncenie
	sem_wait(&pSemaphores->write_to_file);
	int finished = pControl_variales->finished += 1;
	sem_post(&pSemaphores->write_to_file);

	//ak su ukoncene vsetky procesy pasazierov posle vsetkym siglal na ukoncenie
	//ak nie caka na signal
	if (finished < input_param.num_of_passnegers ){
		sem_wait(&pSemaphores->finish);
	}
	else {
		for (int i = 0 ; i < input_param.num_of_passnegers + 2; i++) sem_post(&pSemaphores->finish);
	}

	//proces konci
	sem_wait(&pSemaphores->write_to_file);
	fprintf(output_file,"%d\t: P %d\t: finished\n",++pControl_variales->num_of_action,i);
	fflush(output_file);
	sem_post(&pSemaphores->write_to_file);

}