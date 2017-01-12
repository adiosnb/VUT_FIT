/**************************************
 *   Program :               globals.h
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               015.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               proj2
 *
 *   Notes :        globalne definicie struktur a premennych, hlavne pre error
 *
 ***************************************/
/**
 * @file globals.h
 * @author xtomas32
 * @date 15 Apr 2016
 * @brief globalne definicie struktur a premennych, hlavne pre error
 */
#ifndef IOS_PROJ2_GLOBALS_H
#define IOS_PROJ2_GLOBALS_H

/********************************************************************************************
 * debugovacie makra
 */
#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#include <stdio.h>
#include <semaphore.h>

#define SYS_ERROR 			-1
#define FORK_CHILD_PROCESS 	 0

/**
 * @brief Struktura pre predanie vsupu z funkcie
 */
typedef struct {
	int num_of_passnegers;              /**<  pocet procesu reprezentujicich pasazera */
	int car_max_load ;                  /**<  kapacita voziku */
	int passenger_load_max_time;        /**<  maximalni hodnota doby (v milisekundach), po ktere je generovan novy proces pro pasazera */
	int car_run_max_time;               /**<  maximálni hodnota doby (v milisekundách) prujezdu trati */
} input_param_t;

/**
 * @brief Semafory pouzivane v programe
 */
typedef struct {
	sem_t board;						/**<  Semafaor signalizuje ci pasazieri mozu nastupovat. */
	sem_t unboard;						/**<  Semafor signalizuje, ze pasazieri mozu vystupovat.*/
	sem_t loaded;						/**<  Semafor signalizue, ze pasazieri nastpili do vozika. */
	sem_t unloaded;						/**<  Semafor signalizuje, ze vsetci pasazieri vystupili z vozika. */
	sem_t finish;						/**<  Semafor caka, kym vsetky procesy skoncia. */
	sem_t write_to_file;				/**<  Mutex pre pristup k datam. */
} semaphores_t;

/**
 * @brief Riadiace premenne nastupovania
 */
typedef struct {
	int boarded;						/**< Pocet pasazierov, ktori nastupili do vozika.*/
	int counter;						/**< Pocet vsetkcyh pasazierov, ktori uz niekdy nastupili.*/
	int num_of_action;					/**< Cislo riadka pri zapise do subora. */
	int finished;						/**< Pocet procesov, ktore skoncili svoju cinnost a cakaju na ukoncenie.*/
} control_variable_t;

/**
 * @brief Tento prototyp je to preto aby kompilator nekrical.
 */

int usleep(unsigned usec);

/********************************************************************
 * Globalne premenne
 */

extern int 					control_var_id;			/**< ID zdielanej pamate riadiacich premennych*/
extern int 					semaphore_id;			/**< ID zdielanej pamate semaforov.*/
extern FILE 				*output_file;			/**< Ukazatel na vystupny subor.*/
extern control_variable_t 	*pControl_variales;		/**< Globalna struktura pre zdielanu pamat riadiacich premennych.*/
extern semaphores_t 		*pSemaphores;			/**< Globalna struktura pre zdielanu pamat semaforov.*/
extern input_param_t 		input_param;			/**< Vstupne premenne v strukture*/


#endif //IOS_PROJ2_GLOBALS_H
