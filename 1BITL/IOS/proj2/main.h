/**************************************
 *   Program :               main.h
 *   Author :                Adrián Tomašov, FIT VUT
 *   Login :                 xtomas32
 *   Skupina :               1BIB(2015/2016)
 *   Created :               19.04.2016
 *   Compiled:               gcc 4.8.4
 *   Project :               proj2
 *
 *   Notes :        Prototypy funkcii z main
 *
 ***************************************/
/**
 * @file main.h
 * @author xtomas32
 * @date 19 Apr 2016
 * @brief Prototypy funkcii z main
 */
#ifndef IOS_PROJ2_MAIN_H
#define IOS_PROJ2_MAIN_H

#include "globals.h"

/**
 * @brief Inicializuje vstupnu strukturu na zaklade vstupu (argv).
 *
 * @param argc      Pocet argumentov.
 * @param **argv    Pole ukazatelov na retazce parametrov.
 * @return          Struktura input_param_t s inicializovanymi premennymi z argv.
 */
input_param_t parse_arguments_to_struct(int argc,char **argv);

/**
 * @brief 			Inicualizuje globalne premenne (vystupny subor, zdielana pamat).
 * @param	argc	Pocet argumentov.
 * @param	**argv	Pole ukazatelov na retazce parametrov.
 */
void init(int argc,char **argv);

/**
 * @brief			Tato funkcia popisuje co ma robit vozik.
 * @param 	*var	Ukzatel na zdielanu pamat riadiacich premenych.
 * @param	*sem	Ukzatel na zdielanu pamat semaforov.
 */
void car();

/**
 * @brief			Tato funkcia popisuje co ma robit pasazier.
 * @param 	*var	Ukzatel na zdielanu pamat riadiacich premenych.
 * @param	*sem	Ukzatel na zdielanu pamat semaforov.
 */
void passenger(int i);

/**
 * @brief			Funkcia uzavrie subor, uzavrie zdieanu pamat a nakoniec vymaze zdielanu pamat.
 */
void destroy();


#endif //IOS_PROJ2_MAIN_H
