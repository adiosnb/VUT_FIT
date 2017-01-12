#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>


/***********************************************************************************************************************
*   Program :               proj1.c                                                                                    *
*   Author :                Adrian Tomasov                                                                             *
*   Skupina :               39 1BIB 2015                                                                               *
*   Datum odovzdania :                                                                                                 *
*   Projekt :               IZP Projekt 1 - prace s textem                                                             *
*   Popis :                 Tento program nacitava vstup z stdin a nasledne urci co za pole znakov dostal na vstupe.   *
*                           Vstup je rieseny pomocou scanf() s obmedzenim na 100 znakov. V pripade vacsieho poctu sa   *
*                           rozdeli na casti s <= 100 znakoch, aby nenastal buffer overflow. Navratovu hodnotu zo      *
*                           scanf() uklada do ret_scan, aby zistil kedy je koniec suboru. Dalej pomocou funkcii        *
*                           is_prime(),is_digit(),is_palindrome(),is_date()zisti ci vstup je prvocislo,cislo,palindrome*
*                           alebo datum a vypise co to je. Maximalne prvocislo, ktore dokaze uverit je INT_MAX.        *
*                                                                                                                      *
*                                                                                                                      *
***********************************************************************************************************************/

#define MAX_INPUT 101
#define MAX_MON 12
#define MAX_DAYS 31 

//chyby ktore mozu nastat pocas programu
#define CANNOT_GET_TIME 10
#define CANNOT_RUN_MKTIME 20
#define SSCANF_ERROR_WORNG_INPUT 30
#define NOTPRINTABLE_CHAR 40


/**
 * Tato funkcia ma na vstupe cele cislo, a dalej funkcia skusa najst nejakeho delitala okrem 1 a cisla sameho.
 * Pokial nejaekho delitala najde, ihned ukonci beh funkcie a navrati False, inak True.
 */
bool is_prime(int number);


/**
 * Tato funkcie prejde znak po znaku v retazci, ktory ma na vstupe. Pokial nejaky znak nebude cislo tak ihned
 * navrati False. Ak budu vsetko cisla navrati True.
 */
bool is_digit(char *number);


/**
 *Funckai kontroluje ci sa posledny znak zhoduje s prvy vo vstupnom retazci a pokracuje symetricky az ku stredu slova.
 * Ak najde nezhodu ukonci sa a navrati False inak True.
 */
bool is_palindrome(char *string);


/**
 *Funkcia pre vypis chyboveho hlasenia podla vsupneho oznacenia hlasenia. Hlasenai su definovane na zaciatku programu.
 * Po vypise hlasenia sa program ukonci pomocou exit().
 */
void print_error(int error);


/**
 * Funckia ktora zo vstupneho streamu sa snazi extrahovat rok,mesiac a den. Kontroluje aj spravnost vstupneho retazca
 * a pri zlom formate navrati False. Ak je vstupny retazec vporiadku  tak eztrahuje data a snazi sa ich zapisat do
 * struktury tm, ktora je na vstupe ako ukazatel an strukturu. Nakoniec sa pokusi vypocitat den pomocou mktime().
 * ak vsetko prebehne v poriadku navrati True. Ak by nastala chyba alebo by bol vstupny datum mimo rozsah  navrati false.
 */
bool is_date(char * stream,struct tm * ptime);


/**
 * Funcka ma na vstupe nejaky retazec a na vystupe ma dlzku tohto retazca
 */
int str_len(char *stream);


/**
 * Tato procedura zisti ci su vsetky znaky v retazci na vstupe pisatelne
 * ak nie tak ukonci program
 */
 void input_error_detect(char *stream);


/*
 * Funkcia najde prvy nenunulovy znak vo vstupnom retazci a potom vrati jeho adresu
 */
char * zero_strip(char *stream);


/**
 pole v ktorom su ulozene dni v tyzdni
 *pole zacina !nedelou!, pretoze amerika d-_-b
 */
const char *days[] = {
        "Sun","Mon","Tue",
        "Wed","Thu","Fri","Sat",};


//hlavne telo programu
int main(int argc, char ** argv) {

    (void) argv;
    struct tm * my_time;
    time_t now;
    char buffer[MAX_INPUT];
    char * p_buffer;
    long int my_number;

    if (argc != 1) {        //testuje ci je na vstupe nejaky argument, ak ano vypise tento help a ukonci program
        printf("\n!!!\nToto je aplikacia pre filtrovanie textu. Cita vstup zo stdin pomocou scanf a nasladne zisti ci "
                       "vstupny string je [palindrome,word, number, prime, date]. v pripade datumu [date] zavola"
                       "funkciu mktime() z kniznice time.h. V programe nemam implementacia chybneho datumu takze "
                       "pri nemzyslenom datumu mktime vycaruje den ;)\n!!!\n ");
        exit(EXIT_SUCCESS);
    }

    else {                  //zistuje co je zadany vstup zac
        if (time(&now) == -1) {     //nacitanie systemoveho casu
            print_error(CANNOT_GET_TIME);
        }
        my_time = localtime(&now);  //prevod systemoveho casu do struktury my_time

        //hlavny cyklus programu ktory nacita a testuje vstup az do konca suboru potom uspesne ukonci program
        //vnutri cyklu sa odvolava na funcie definovane na konci programu ktore zistuju akeho typu je vstup
        while (scanf("%100s", buffer) != EOF) {

            input_error_detect(buffer);
            errno = 0;

            if (is_digit(buffer)) {
                p_buffer = zero_strip(buffer);
                if ((my_number = strtol(p_buffer,NULL,10)) != -1    &&      //pokusi sa premenitretazec na long int
                        my_number <= INT_MAX                        &&      //okontrolovanie ci je cislo mensie ako INT_MAX
                        errno != ERANGE                             &&      //kontrola systemovej chybovej premennej errno
                        is_prime(my_number))                                //volanie funckie na kontrolu prvocisla
                                                printf("number: %ld (prime)\n", my_number);

                else                            printf("number: %s\n", p_buffer);
            }

            else {
                if (is_date(buffer, my_time))   printf("date: %s %s\n", days[my_time->tm_wday],buffer);
                else {
                    if (is_palindrome(buffer))  printf("word: %s (palindrome)\n", buffer);
                    else                        printf("word: %s\n", buffer);
                }
            }

        }
    }
    exit(EXIT_SUCCESS);
}


bool is_prime(int number) {

    if (number%2 == 0 && number != 2) return false;
    if (number == 1 ) return 1;

    for (int i = 3;i<number/2; i++)
        if (number %i == 0 ) return false;

    return true;
}


bool is_digit(char *stream) {

    if (stream[0] == '-' || !isdigit(stream[0])) return false;
    for (int i = 0; i < str_len(stream); i++) {
        if (!isdigit(stream[i])) return false;
    }
    return true;
}


bool is_palindrome(char *stream) {

    int len = str_len(stream);

    for (int i = 0 ; i < len / 2 ;i++) {
        if (stream[i] != stream[len - i - 1]) return false;
    }

    return true;
}


bool is_date(char *stream, struct tm * ptime) {
    int len = str_len(stream) ;
    int day,mon,year;

    if (len != 10) return false;

    for (int i = 0; i < len; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(stream[i])) return false;
    }

    if (sscanf(stream,"%d-%d-%d", &year,&mon,&day) == EOF) {
        print_error(SSCANF_ERROR_WORNG_INPUT);
    }

    if (mon > MAX_MON || day > MAX_DAYS)
        return false;

    ptime->tm_year = year-1900;
    ptime->tm_mon  = mon - 1;
    ptime->tm_mday = day;

    if (!mktime(ptime)) {
        print_error(CANNOT_RUN_MKTIME);
    }

    return true;
}


void print_error(int error) {
    switch (error) {
        case CANNOT_GET_TIME:
            fprintf(stderr,"\n\n***\nNedokazem precitat cas zo systemu pomocou time()\n***\n\n");
            break;

        case CANNOT_RUN_MKTIME:
            fprintf(stderr,"\n\n***\nNastala chyba pri funkcii mktime\n***\n\n");
            break;

        case SSCANF_ERROR_WORNG_INPUT:
            fprintf(stderr,"\n\n***\nNastala chyba pri citani vstupu z buferu pomocou "
                        "sscanf pri extrahovani udajou");
            break;
        case NOTPRINTABLE_CHAR:
            fprintf(stderr,"\n\n***\nNa vstupe bol nevypisatelny znak.\nProgram bol ukonceny, pretoze by nastali chyby "
                    "pri urcovani typu slova.\n***\n\n");
            break;
        default:
            fprintf(stderr,"\n\n***\nNeznama chyba\n***\n\n");
            break;
        }

    exit(error);
}


int str_len(char *stream) {
    int len = 0;
    while (*stream != '\0'){
        stream++;
        len++;
    }
    return len;
}


void input_error_detect(char *stream) {
    int len = str_len(stream);
    for (int i = 0; i < len; i++){
        if (!isprint(stream[i]))
            print_error(NOTPRINTABLE_CHAR);
    }
    return;
}


char *zero_strip(char *stream) {
    int i = 0;
    while (stream[i] == '0' && stream[i+1] != '\0')
        i++;

    return &stream[i];
}
