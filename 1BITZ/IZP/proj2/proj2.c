#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

/***********************************************************************************************************************
*   Program :               proj2.c                                                                                    *
*   Author :                Adrian Tomasov                                                                             *
*   Login :                 xtomas32                                                                                   *
*   Skupina :               39 1BIB 2015                                                                               *
*   Datum odovzdania :      29.11.2015                                                                                 *
*   Projekt :               Projekt 2 - Iterační výpočty                                                               *
*                                                                                                                      *
*   Poznamka :              Implementovana bisekce                                                                     *
*                                                                                                                      *
*   Popis :                 Program je urceny na pocitanie prirodzenych logaritmov. Na vstupe zadate bud --log X N, kde*
*                           X je cislo > 0, z ktoreho chceme dostat logaritmus a N znaci pocet iteracii v vypoctu.     *
*                           Cast --iter MIN MAX EPS hlada pocet iteracii pre vypocet logaritmu v danom intervale. EPS  *
*                           odchylka vypoctu. Program hlada pocet iteracii kym rozdiel dvoch po sebe nasledujucich     *
*                           vypoctov nie je mensi alebo rovny EPS.                                                     *
*                                                                                                                      *
***********************************************************************************************************************/


/**
 * chybove hlasenia
 */
#define OK 99  //nie je 0, pretoze 0 som pouzil pri E_OUT_OF_RANGE, ktore nemoze nadobudnut hodnotu vacsiu ako 0, inak by mohlo dojst k chybe
#define E_WRONG_ARG 10
#define E_OUT_OF_RANGE 0

/**
 * Konstanty pouzite v programe
 */
#define EPS_MAX 1e-12
#define UP_CORNER INT_MAX
#define DOWN_CORNER 0
#define DIVISOR_INT_MAX 4096
#define ACCEPTABLE_DIFFERENCE 0.3

#define LOG 100
#define ITER 110

/**
 * implementacia taylorovho polynomu
 * na vstupe cislo, z ktoreho chceme logaritmus a pocet iteracii
 * vracia vypocitanu hodnotu
 */
double taylor_log(double x, unsigned int n);

/**
 * implementacia zretazenho zlomku
 *  na vstupe cislo, z ktoreho chceme logaritmus a pocet iteracii
 *  vracia vypocitanu hodnotu
 */
double cfrac_log(double x, unsigned int n);

/**
 * vetva pre vypis logaritmov pri znamom pocte iteracii
 */
void log_part(double x,unsigned int n);

/**
 * vetva pre hladanie a vypis poctu iteracii podla zadannej odchylky vypoctu
 */
int iter_part(double min,double max, double eps);

//vypis chyboveho hlasenia
void print_error(int error);

/**
 * funckia pre vypocet poctu potrebnych iteracii
 * na vstupe ma cislo z ktoreho pocita logaritmus, ukazatel na funkciu pomocou ktorej bude pocitat, a odchylku vypoctu
 * na vystupe je pocet potrebnych iteracii
 */
unsigned int num_of_iterations(double input_num,double (*log_func)(double,unsigned int), double eps);

/**
 * funkcia pre nacitanie vstupnych hodnot
 * na vstupe ma ukazatele na cisla ktory priradi hodnoty
 * na vystupe ma ma bud hodnotu z ktorej vie ze ma pocitat bud cast log_part alebo iter_part a v pripade nespravneho
 * vstupu navracia chybu
 */
short int get_input(int *argc,char **argv,double *cislo_1,double *cislo_2, double *eps, unsigned int *n);

int main(int argc,char ** argv) {

    double cislo_1 = 0;     // pri --log je to cislo X, pri --iter to je MIN
    double cislo_2 = 0;     //cislo MAX
    double eps = 0;         //EPS zadana na vstupe
    int unsigned n;         //pocet iteracii
    short int return_value;

    return_value = get_input(&argc,argv,&cislo_1,&cislo_2,&eps,&n);

    if ( return_value == E_WRONG_ARG) {
        print_error(E_WRONG_ARG);
        return E_WRONG_ARG;
    }
    else{
        if (return_value == LOG){
            log_part(cislo_1,n);
        }
        else{
            if (return_value == ITER){
                if ((iter_part(cislo_1,cislo_2,eps)) == E_OUT_OF_RANGE) {
                    return E_OUT_OF_RANGE;
                }
            }
        }
    }

    return OK;
}

double taylor_log(double x, unsigned int n) {
    double vysledok = 0.0;
    double menovatel = 1;

    if (x == 0) {
        return -INFINITY;
    }

    if (x < 0){
        return NAN;

    }
    if( x == INFINITY ){
        return INFINITY;
    }

    if (x > 1) {
        x = (x-1)/x;
        for (unsigned int i = 1 ; i<=n;i++) {
            menovatel *= x;
            vysledok += menovatel/i;
        }
    }

    else {
        x = 1-x;
        for (unsigned int i = 1 ; i <= n ; i++) {
            menovatel *= x;
            vysledok-= menovatel/i;
        }
    }

    return vysledok;
}

double cfrac_log(double x, unsigned int n) {
    double z = (x-1)/(x+1);
    n--;
    double vysledok = 2*(double)(n)+1;
    double  pom;

    if(x == 0) {
        return -INFINITY;
    }
    if(x < 0) {
        return NAN;
    }

    if( x == INFINITY ){
        return INFINITY;
    }

    for (unsigned int i = n  ; i > 0 ; i--){
        pom = (double)(i)*(double)(i)*z*z;
        vysledok = (2*(double)i-1)-(pom / vysledok);
    }
    vysledok = (2*z/vysledok);

    return vysledok;
}

void print_error(int error){

    switch (error) {
        case E_WRONG_ARG:
            fprintf(stderr, "\n\n"
                    "******************************\n"
                    "* Zle argumenty na vstupe!!! *\n"
                    "******************************\n\n");
            break;

        case E_OUT_OF_RANGE:
            fprintf(stderr,"\n\n"
                    "***********************************************************\n"
                    "*Prilis mnoho iteracii a doslo k preteceniu premennej !!! *\n"
                    "***********************************************************\n\n");
            break;

        default:
            fprintf(stderr,"\n\n"
                    "*******************\n"
                    "*Neznama chyba !!!*\n"
                    "*******************\n\n");
            break;
        }
    return;
}

void log_part(double x, unsigned int n) {
    printf("        log(%g) = %.12g\n", x, log(x));
    printf("     cf_log(%g) = %.12g\n", x, cfrac_log(x, n));
    printf(" taylor_log(%g) = %.12g\n", x, taylor_log(x, n));
}

int iter_part(double min, double max, double eps){

    unsigned int cf_num_iter_min = 0;
    unsigned int cf_num_iter_max = 0;
    unsigned int taylor_num_iter_min = 0;
    unsigned int taylor_num_iter_max = 0;

    if (
        ((cf_num_iter_min = num_of_iterations(min,&cfrac_log,eps)) == E_OUT_OF_RANGE)       ||
        ((cf_num_iter_max = num_of_iterations(max,&cfrac_log,eps)) == E_OUT_OF_RANGE)       ||
        ((taylor_num_iter_min = num_of_iterations(min,&taylor_log,eps)) == E_OUT_OF_RANGE)  ||
        ((taylor_num_iter_max = num_of_iterations(max,&taylor_log,eps)) == E_OUT_OF_RANGE)
    ){
        return E_OUT_OF_RANGE;
    }

    cf_num_iter_max = cf_num_iter_max > cf_num_iter_min ? cf_num_iter_max : cf_num_iter_min;
    taylor_num_iter_max = taylor_num_iter_max > taylor_num_iter_min ? taylor_num_iter_max : taylor_num_iter_min;

    //Vypis logarytmov s vypocitanym poctom iteracii
    printf("       log(%g) = %.12g\n",min,log(min));
    printf("       log(%g) = %.12g\n",max,log(max));
    printf("continued fraction iterations = %u\n",cf_num_iter_max);
    printf("    cf_log(%g) = %.12g\n",min,cfrac_log(min,cf_num_iter_max));
    printf("    cf_log(%g) = %.12g\n",max,cfrac_log(max,cf_num_iter_max));
    printf("taylor polynomial iterations = %u\n",taylor_num_iter_max);
    printf("taylor_log(%g) = %.12g\n",min,taylor_log(min,taylor_num_iter_max));
    printf("taylor_log(%g) = %.12g\n",max,taylor_log(max,taylor_num_iter_max));

    return OK;
}

unsigned int num_of_iterations(double input_num,double (*log_func)(double,unsigned int), double eps) {
    /**
     * V tejto funkcii sa pri vyhladavani neberie cely interval (0,INT_MAX), ale tento interval sa rozdeli na
     * DIVISOR_INT_MAX intervalov. Nasledne vyhldava iba v tom mensom intervaly a ak sa tam nenachadza spravny pocet
     * iteracii prejde na nasledujuci interval. V pripade pretecenia navrati chybu.
     * Tuto metodu som pouziil pretoze funkcie a pocitanie logaritmov s hodnotou INT_MAX je velmi zdlhave.
     */
    unsigned int i = 0;
    double up_moved = UP_CORNER / DIVISOR_INT_MAX;      //horna hranica intervalu
    double down_moved = DOWN_CORNER;                    //dolna hranica intervalu
    double interval = up_moved;                         //v tomto intervale cisel riesi hladanie cisel pomocou polenia intervalu
    double middle = 0;                                  //premenna kde sa uklada stred intervau
    double math_log = log(input_num);                   //log vypocitany pomocou log() z math

    short int have_a_result = 0;                              //riadiaca premenna cyklu

    if (
            ((*log_func)(input_num,1) == NAN)       ||
            (*log_func)(input_num,1) == -INFINITY   ||
            (*log_func)(input_num,1) == INFINITY
            ){
        return 0;
    }

    while (!have_a_result){
        while (fabs(up_moved - down_moved) > ACCEPTABLE_DIFFERENCE){
            middle = (up_moved + down_moved) / 2;
            if((fabs((*log_func)(input_num,(unsigned)middle) - math_log)) > eps) down_moved = middle;
            else up_moved = middle;
        }
        if(middle+1 < interval*(i+1)){
            have_a_result = 1;
            continue;
        }
        else {
            i++;
            down_moved = interval*(double)i;
            up_moved = interval*(double)(i+1);
            middle = 0;
        }

        if (up_moved > INT_MAX)
            return E_OUT_OF_RANGE;

    }
    return (unsigned)middle;
}

short int get_input(int *argc, char **argv,double *cislo_1,double *cislo_2, double *eps, unsigned int *n){
    char *p_end;

    errno = 0;

    if (*argc == 4) {
        if ((!strcmp(argv[1], "--log"))) {
            *cislo_1 = strtod(argv[2], &p_end);
            *n = (unsigned)atoi(argv[3]);
            if (!((p_end != NULL)                               &&             //osetrenei vstupu
                  (*n > 0)                                      &&             //pocet itercii je vacsii je vacsii ako 0
                  (errno != ERANGE)                                            //skontroluje ci strtod nepretiekol
            )) {//ak neuspeje apson jedna podmienka program vypise chybovehlasenie a ukonci sa pretoze by nastala chyba za behu programu
                return E_WRONG_ARG;
            }
            else{
                return LOG;
            }
        }
    }

    else {
        if (*argc == 5) {
            if ((!strcmp(argv[1], "--iter"))) {
                *cislo_1 = strtod(argv[2], &p_end);
                *cislo_2 = strtod(argv[3], &p_end);
                *eps = strtod(argv[4], &p_end);
                if (!(((p_end != NULL) &&          //podmiekka vstupu
                       (*eps >= EPS_MAX) &&          //podmienka v zadanie pre eps
                       (errno != ERANGE) &&          //skontroluje ci strtod nepretiekol
                       (*cislo_1 <
                        *cislo_2)                                      //cislo_1 musi byt mensie ako cislo_2 inak by interval nemal zmysel
                ))) {//ak neuspeje apson jedna podmienka program vypise chybovehlasenie a ukonci sa pretoze by nastala chyba za behu programu
                    return E_WRONG_ARG;
                }
                else {
                    return ITER;
                }
            }
        }
        else {
            return E_WRONG_ARG;
        }

    }

    return E_WRONG_ARG;
}