/**************************************
 *   Program :               error.h
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               03.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               proj2
 *
 *   Notes :        pomocne konstanty pre vypis chyb
 *
 ***************************************/
/**
 * @file error.h
 * @author xtomas32
 * @date 3 Apr 2016
 * @brief pomocne konstanty pre vypis chyb
 */

#ifndef IOS_PROJ2_ERROR_H
#define IOS_PROJ2_ERROR_H

#define E_WRONG_ARG             0
#define E_WRONG_NUM_OF_ARGS     1
#define E_ARG_OUT_OF_RANGE      2
#define E_FORK_ERROR			3
#define E_FOPEN_ERROR			4
#define E_FTOK_ERROR			5
#define E_SHMGET_ERROR			6
#define E_SHMAT_ERROR			7
#define E_SEM_INIT				8

/**
 * @brief                   Vypise chybove hlasenie na stderr.
 * @param error_num         Chybove cislo.
 */
void print_error_msg(int error_num);

/**
 * @brief                   Vypise chybove hlasenie a ukonci programm s navratovou hodnotou 1.
 * @param error_num         Chybove cislo.
 */
void arg_error(int error_num);

/**
 * @brief                   Vypise chybove hlasenie a ukonci programm s navratovou hodnotou 2.
 * @param error_num         Chybove cislo.
 */
void system_error(int error_num);

#endif //IOS_PROJ2_ERROR_H
