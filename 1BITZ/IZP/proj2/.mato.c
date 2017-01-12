///Projekt 2 - Iteracne vypocty logaritmu
///---------------------------------------
///Vyhotovil: Martin Uhliar (xuhlia03)
///Datum: 2015-11-11
///Verzia: 4.16
/*
* Poznamka: implementovana bisekce
*/

#include <stdio.h>  //Zaklady
#include <stdlib.h> //EXIT_FAILURE a SUCCESS konstanty
#include <math.h>   //log() funkcia

///Definicia booleanovskej premennej
typedef int bool;//Definovanie noveho datoveho typu zalozeneho na int
enum { false, true };//Vymenovanie a priradenie prhodnot prvkom (false - 0, true - 1)


///Enumeracie pre chybove hlasenia
enum
{
    ZIADNE_ARGUMENTY = 1,//Chybove hlasenie: Neboli zadane ziadne argumenty
    MALO_ARGUMENTOV,//Chybove hlasenie: Bolo zadanych malo argumentov
    VELA_ARGUMENTOV,//Chybove hlasenie: Bolo zadanych vela argumentov
    VELA_ARGUMENTOV_PRE_LOG,//Chybove hlasenie: Bolo zadanych vela argumentov pre --log
    VELA_ARGUMENTOV_PRE_ITER,//Chybove hlasenie: Bolo zadanych vela argumentov pre --iter
    MALO_ARGUMENTOV_PRE_LOG,//Chybove hlasenie: Bolo zadanych malo argumentov pre --log
    MALO_ARGUMENTOV_PRE_ITER,//Chybove hlasenie: Bolo zadanych malo argumentov --iter
    CHYBA_NA_PRVOM_ARGUMENTE,//Chybove hlasenie: Prvy argument nie je v spravnom tvare
    CHYBA_NA_DRUHOM_ARGUMENTE,//Chybove hlasenie: Druhy argument nie je v spravnom tvare
    CHYBA_NA_TRETOM_ARGUMENTE,//Chybove hlasenie: Treti argument nie je v spravnom tvare
    CHYBA_NA_STVRTOM_ARGUMENTE//Chybove hlasenie: Stvrty argument nie je v spravnom tvare
};


///Prototypy funkcii
double taylor_log();//Pocita hodnotu pre prirodzeny logaritmus s danym poctom iteracii cez Taylorov polynom
double cfrac_log();//Pocita hodnotu pre prirodzeny logaritmus s danym poctom iteracii cez zretazene zlomky
float absolutnaHodnota();//Vrati absolutnu hodnotu z cisla
int pocetIteracii();//Vrati pocet potrebnych iteracii pre dosiahnutie zadanej odchylky (EPS)
bool kontrolaVstupu();//Kontroluje zadane argumenty na vstupe
bool porovnanieRetazcov();//Porovnava dva retazce medzi sebou
void vypis();//Vypisuje vysledky programu
void help();//V pripade chyby vypisuje chybove hlasenia


///Deklaracie funkcii
int main(int argc, char **argv)//Nacitava argumenty a ich pocet
{
    short unsigned int chybaVstupu;//Premenna pre hodnotu chyboveo hlasenia

    chybaVstupu = kontrolaVstupu(argc, argv);//Pokial sa na vstupe nachadza neplatny argument funkcia kontrolaVstupu vrati jeho chybovy kod. Ak je vsetko v poriadku funkcia vrati hodnotu "0"

    if (chybaVstupu == 0)//Ak sa v premennej nachadza hodnota "0", je jasne ze argumenty su v poriadku
        vypis(argv);//Z argumentov sa urobi vypis

    else //Ak argumenty nie su v poriadku
    {
        help(chybaVstupu);//Vypise sa kod chyby, jej popis a kratka napoveda
        return EXIT_FAILURE;//Program sa ukonci
    }
    return EXIT_SUCCESS;//Ak vsetko prebehlo v poriadku program sa uspesne ukonci
}

int kontrolaVstupu(int pocetArgumentov, char **argument)//Funkcia kontroluje spravnost zadanych argumentov na vstupe
{
    char *znakyRetazca;//Deklaracia premenej pointeru na znak
    znakyRetazca = NULL;//Vynulovanie premennej

    if (pocetArgumentov == 1)//Ak neboli zadane ziadne argumenty
        return ZIADNE_ARGUMENTY;//funkcia vrati chybove lasenie

    if (pocetArgumentov < 4 && porovnanieRetazcov( argument[1], "--log" ) == 0)//Ak bolo zadanych malo argumentov
        return MALO_ARGUMENTOV;//funkcia vrati chybove lasenie

    if (pocetArgumentov > 5 && porovnanieRetazcov( argument[1], "--iter" ) == 0 )//Ak bolo zadanych vela argumentov
        return VELA_ARGUMENTOV;//funkcia vrati chybove lasenie

    if ( porovnanieRetazcov( argument[1], "--log" ) )//Zistovanie spravnosti argumentov pre argument "--log"

    {
        if ( pocetArgumentov < 4)//Ak bolo zadanych malo argumentov
            return MALO_ARGUMENTOV_PRE_LOG;//funkcia vrati chybove lasenie

        if ( pocetArgumentov > 4)//Ak bolo zadanych vela argumentov
            return VELA_ARGUMENTOV_PRE_LOG;//funkcia vrati chybove lasenie

            strtod(argument[2], &znakyRetazca);
        if ( *znakyRetazca != '\0' )//Zistuje, ci sa druhy argument nerovna nule, ak boli na vstupe nejake neplatne znaky su ulozene do "znakyRetazca". V druhej casti sa kontroluje, ci je tato premenna zmenena.
            return CHYBA_NA_DRUHOM_ARGUMENTE;//funkcia vrati chybove lasenie

        if ( (strtol(argument[3], &znakyRetazca, 0) <= 0 ) || *znakyRetazca != '\0' )//Zistuje, ci sa treti argument nerovna nule, ak boli na vstupe nejake neplatne znaky su ulozene do "znakyRetazca". V druhej casti sa kontroluje, ci je tato premenna zmenena.
            return CHYBA_NA_TRETOM_ARGUMENTE;//funkcia vrati chybove lasenie
    }

    else if ( porovnanieRetazcov( argument[1], "--iter" ) )//Zistovanie spravnosti argumentov pre argument "--iter"

    {
        if ( pocetArgumentov < 5)//Ak bolo zadanych malo argumentov
            return MALO_ARGUMENTOV_PRE_ITER;//funkcia vrati chybove lasenie

        if ( pocetArgumentov > 5)//Ak bolo zadanych vela argumentov
            return VELA_ARGUMENTOV_PRE_ITER;//funkcia vrati chybove lasenie

            strtod(argument[2], &znakyRetazca);
        if ( *znakyRetazca != '\0')//Zistuje, ci sa druhy argument nerovna nule, ak boli na vstupe nejake neplatne znaky su ulozene do "znakyRetazca". V druhej casti sa kontroluje, ci je tato premenna zmenena.
            return CHYBA_NA_DRUHOM_ARGUMENTE;//funkcia vrati chybove lasenie

            strtod(argument[3], &znakyRetazca);
        if ( *znakyRetazca != '\0')//Zistuje, ci sa treti argument nerovna nule, ak boli na vstupe nejake neplatne znaky su ulozene do "znakyRetazca". V druhej casti sa kontroluje, ci je tato premenna zmenena.
            return CHYBA_NA_TRETOM_ARGUMENTE;//funkcia vrati chybove lasenie

            strtod(argument[4], &znakyRetazca);
        if ( *znakyRetazca != '\0')//Zistuje, ci sa stvrty argument nerovna nule, ak boli na vstupe nejake neplatne znaky su ulozene do "znakyRetazca". V druhej casti sa kontroluje, ci je tato premenna zmenena.
            return CHYBA_NA_STVRTOM_ARGUMENTE;//funkcia vrati chybove lasenie
    }

    else
        return CHYBA_NA_PRVOM_ARGUMENTE;//funkcia vrati chybove lasenie

    return 0;//Ak nebola splnena ani jedna podmienka je jasne, ze argumenty su v poriadku. Funkcia odosle hodnotu "0"
}

double taylor_log(double x, unsigned int n)//Pocita hodnotu pre prirodzeny logaritmus s danym poctom iteracii cez Taylorov polynom
{
    double mocnina;//Deklaracia premennej "mocnina" do ktorej bude ulozena hodnota citatela zlomku zo vzorca
    double y;//Deklaracia premennej "y" do ktorej bude ulozena priebezna hodnota
    unsigned short int i;//Index pre cyklus

    if ( x == 0.0 ) return (-1)*INFINITY;
    if ( x == INFINITY ) return INFINITY;
    if ( x < 0.0  ) return NAN;

    y = 0;//Vynulovanie hodnoty premennej "y"

    //------------------------------"x" patri (0,1)-----------------------------------------------
    if ( x > 0 && x < 1 )//Ak sa hodnota "x" patri (0,1) pouzije sa nasledujuci kod
    {
        x = 1 - x;//"x" upravene podla vzorca
        mocnina = x;//Do citatela prirad "x"
        for ( i = 1; i < n+1; i++ )//Kym "i" nie je rovne "n". "i" sa inkrementuje o 1
        {
            y -= mocnina / i;//Do "y" sa priradi posledna hodnota vyrazu na pravo
            mocnina *= x;//Umocnenie citatela
        }
    }
    //---------------------------------------------------------------------------------------------

    //------------------------------"x" patri <1,DOUBLE_MAX)---------------------------------------
    else
    {
        mocnina = 1;//Prvota hodnota citatela
        for ( i = 1; i < n+1; i++ )//Kym "i" nie je rovne "n". "i" sa inkrementuje o 1
        {
            y += (mocnina * ((x-1)/x))/i;//Do "y" sa priradi posledna hodnota vyrazu na pravo
            mocnina *= (x-1)/x;//Umocnenie citatela vyrazom, ktory vyplyva zo vzorca
        }

    }
    //---------------------------------------------------------------------------------------------
    return y;//Funkcia vrati poslednu hodnotu "y"
}

double cfrac_log(double x, unsigned int n)//Pocita hodnotu pre prirodzeny logaritmus s danym poctom iteracii cez zretazene zlomky
{
    double d;
    double men;

    if ( x == 0.0 ) return (-INFINITY);
    else if ( x == 1.0) return 0.0;//Logaritmus pre 1 sa rovna 0
    else if ( x == INFINITY ) return INFINITY;
    else if ( x < 0.0  ) return NAN;

    x =  (x-1)/(x+1);//odvodenie zo vzorca
    d = ((n*2)-1);//Menovatel zlomku
    n--;
    d = ((((n)*(n))*(x*x)) / d );//odvodenie zo vzorca

    while (n > 1 )//Kym je "n" vacsie od "1"
    {
        men = (n-1)*x;
        d =  (men * men) / (((n * 2)-1)-d);
        n--;//Dekrementacia poctu iteracii/indexu
    }

    d = (2 * x ) / (1 - d);

    return d;//Nakoniec "d" deli "(2 * x )"

}

int pocetIteracii( double x, double eps, double (*funkcia)(double , unsigned int ))//Vrati pocet potrebnych iteracii pre dosiahnutie zadanej odchylky (EPS) cez bisekciu/poolenie intervalu/binarneho vyhladavania
{//                                                ^^^^^
 //                                                |||||
 //                  Pointer na funkciu, pre ktoru chceme vypocitat pocet iteracii

    double y;//Clen aktualnej iteracie
    double yp;//Clen predchadzajucej iteracie
    unsigned short int i;//Index cyklu
    unsigned int lavyOkraj;//Lavy okraj pre bisekciu
    unsigned int pravyOkraj;//Pravy okraj pre bisekciu
    unsigned int median;//Median pre bisekciu

    i = 2;//Index sa nastavi na hodnotu 2
    yp = log(x);//<-----Upravene podla fora------>//predchadzajuci clen je staticky, hodnota "log(x)" je odcitavana od hodnoty v "y"

    do{// Kym absolutna hodnta rozdielu predchadzajuceho clena a aktualneho clena je vacsia od odchylky
        i *= 2;//Index sa umocnuje na druhu
        y = funkcia(x, i);//Do aktualneho clena sa ulozi hodnota prirodzeneho logaritmu pre cislo "x" pri "i" pocte iteracii pomocou funkcie, na ktoru ukazuje pointer z parametra :D
    }while( absolutnaHodnota(yp - y) > eps );

    //--------------------------------Binarne vyhladavanie poctu iteracii--------------------------
    pravyOkraj = i;
    lavyOkraj = 1;

    do {//                                                                       lavyOkraj                        median      hladana hodnota       pravyOkraj
        median = ((lavyOkraj + pravyOkraj)/2);// Median indexu     __________________|_______________________________|____________|_____________________|____________________________
        y = funkcia(x, median );
    //                                                                                                            || || ||
    //                                                                                                            \/ \/ \/
    //
        if ( absolutnaHodnota(y - yp) > eps )//                                                                  lavyOkraj  hladana hodnota         pravyOkraj
            lavyOkraj = median + 1;//                              __________________________________________________|_________|_______|________________|____________________________
        else//                                                                                                                         ^median
            pravyOkraj = median;//                                                                                || || ||
    //                                                                                                            \/ \/ \/
    //

    }while( lavyOkraj != median );//                                                                            lavyOkraj  hladana hodnota
    //                                                             __________________________________________________|_________|_______|__________________________________________
    if ( absolutnaHodnota(yp - y) > eps)//                                                                                             ^pravyOkraj
        return median + 1;//                                                                                      || || ||
    else////                                                                                                      \/ \/ \/
        return median;//                                                                         Az kym sa niektory z okrajov nerovna medianu
    //

    //----------------------------------------------------------------------------------------------
}

bool porovnanieRetazcov(char* s1, char* s2)//Porovnava dva retazce medzi sebou
{
    int i;//Index pre cyklus

    for (i = 0; s1[i] != '\0'; i++)//Opakuje sa pokym nie je v retazci "s1[i]" znak '\0'
        if ( s1[i] != s2[i])//Ak sa znaky narovnakych indexoch nerovnaju, funkcia vrati hodnotu "false"
            return false;
    return true;//Ak sa znaky rovnali, funkcia vrati hodnotu "true"
}

void vypis(char **argument)//Vypisuje vysledky programu
{
    //--------------------------------------------------------Vypis pre "--log"---------------------------------------------------------------------------------------------------

    if (porovnanieRetazcov(argument[1], "--log"))//Ak prvy argument je "--log", funkcia urobi vypis pre volbu "--log"
    {
        double x;//Cislo z argumentu
        unsigned int n;//pocet iteracii z argumentu

        x = strtod(argument[2], NULL);//Do "x" priradi Cislo z 2. argumentu
        n = (unsigned int)strtol(argument[3], NULL, 0);//Do "n" priradi pocet iteracii z argumentu
        printf("       log(%g) = %.12g\n", x, log(x) );//Vytlaci hodnotu pre "x" prirodzeneho logaritmu pouzitim funkcie "log()" z kniznice "math.h"
        printf("    cf_log(%g) = %.12g\n", x, cfrac_log(x, n) );//Vytlaci hodnotu prirodzeneho logaritmu pre "x" s poctom iteracii z 2. argumentu pomocou funkcie "cfrac_log"
        printf("taylor_log(%g) = %.12g\n", x, taylor_log(x, n) );//Vytlaci hodnotu prirodzeneho logaritmu pre "x" s poctom iteracii z 2. argumentu pomocou funkcie "taylor_log"
    }
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    else//Ak prvy argument je "--iter", funkcia urobi vypis pre volbu "--iter"

    //--------------------------------------------------------Vypis pre "--iter"---------------------------------------------------------------------------------------------------

    {
        double min;//deklaracia premennej "min", sluzi na ulozenie hodnoty pre spodnu hranicu intervalu
        double max;//deklaracia premennej "max", sluzi na ulozenie hodnoty pre hornu hranicu intervalu
        double eps;//deklaracia premennej "eps", sluzi na ulozenie hodnoty pre odchylku
        int iterMin;//deklaracia premennej "iterMin", sluzi na ulozenie hodnoty pre pocet potrebnych iteracii na dosihnutie zadanej odchylky z "min"
        int iterMax;//deklaracia premennej "iterMax", sluzi na ulozenie hodnoty pre pocet potrebnych iteracii na dosihnutie zadanej odchylky z "max"

        min = strtod(argument[2], NULL);//priradenie hodnot z 2. argumentu
        max = strtod(argument[3], NULL);//priradenie hodnot z 3. argumentu
        eps = strtod(argument[4], NULL);//priradenie hodnot z 4. argumentu

        //-----------------Kontroluje spravnost intervalu------------------
        if (min > max)// Ak je hodnota "min" vacsia ako hodnota "max"
        {
            max = min;// Do "max" sa priradi hodnota z "min", kotra je vacsia
            min = strtod(argument[3], NULL);// Do "min" sa ulozi povodna hodnota z "max"
        }
        //-----------------------------------------------------------------

        printf("       log(%g) = %.12g\n", min, log(min) );//Vytlaci hodnotu pre "min" prirodzeneho logaritmu pouzitim funkcie "log()" z kniznice "math.h"
        printf("       log(%g) = %.12g\n", max, log(max) );//Vytlaci hodnotu pre "max" prirodzeneho logaritmu pouzitim funkcie "log()" z kniznice "math.h"

        iterMin = pocetIteracii(min, eps, cfrac_log);//Do premennej "iterMin" sa priradi pocet iteracii potrebnych na vypocitanie logaritmu z "min" pre odchylku "eps"
        iterMax = pocetIteracii(max, eps, cfrac_log);//Do premennej "iterMax" sa priradi pocet iteracii potrebnych na vypocitanie logaritmu z "max"  pre odchylku "eps"

        iterMax = (iterMin > iterMax)? iterMin:iterMax;//Vacsi pocet iteracii prirad do "iterMax"
        printf("continued fraction iterations = %d\n", iterMax);//Vytlaci vacsi pocet iteracii potrebnych na vypocitanie logaritmu z "min" pre odchylku "eps" pomocou zretazenych zlomkov
        printf("    cf_log(%g) = %.12g\n", min, cfrac_log(min, iterMax) );//Vytlaci hodnotu prirodzeneho logaritmu pre "min" na skor zisteny pocet iteracii pomocou funkcie "cfrac_log"
        printf("    cf_log(%g) = %.12g\n", max, cfrac_log(max, iterMax) );//Vytlaci hodnotu prirodzeneho logaritmu pre "max" na skor zisteny pocet iteracii pomocou funkcie "cfrac_log"

        iterMin = pocetIteracii(min, eps, taylor_log);//Do premennej "iterMin" sa priradi pocet iteracii potrebnych na vypocitanie logaritmu z "min" pre odchylku "eps"
        iterMax = pocetIteracii(max, eps, taylor_log);//Do premennej "iterMax" sa priradi pocet iteracii potrebnych na vypocitanie logaritmu z "max"  pre odchylku "eps"
        iterMax=(iterMin>iterMax)?iterMin:iterMax;
        printf("taylor polynomial iterations = %d\n", iterMax);//Vytlaci vacsi pocet iteracii potrebnych na vypocitanie logaritmu z "min" pre odchylku "eps" pomocou Taylorovho polynomu
        printf("taylor_log(%g) = %.12g\n", min, taylor_log(min, iterMax) );//Vytlaci hodnotu prirodzeneho logaritmu pre "min" na skor zisteny pocet iteracii pomocou funkcie "taylor_log"
        printf("taylor_log(%g) = %.12g\n", max, taylor_log(max, iterMax) );//Vytlaci hodnotu prirodzeneho logaritmu pre "max" na skor zisteny pocet iteracii pomocou funkcie "taylor_log"
    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    }
}

float absolutnaHodnota(double x)//Vrati absolutnu hodnotu z cisla
{
    if ( x < 0 ) //Ak je parameter zaporny
        return -x;// je vynasobeny "(-1)" cim sa ziska jeho absoltna hodnota
    else return x;//Ak nie je zaporna, vrati sa povodna hodnota parametra
}


void help(int chyba)//Funkcia sluzi na vypisovanie napovedy a chybovych hlaseni
{
    fprintf(stderr,"\nKod chyby: %d\n\n",chyba );//Vytlaci cislo chyby

    if( chyba == ZIADNE_ARGUMENTY)// 1 - Chyba sa zobrazi ak neboli zadane argumenty
    {
        fprintf(stderr,"Neboli zadane ziadne argumenty.\n\n");//Vytlaci chybu
    }

    if( chyba == MALO_ARGUMENTOV)// 2 - Chyba sa zobrazi ak bolo zadanych malo argumentov
    {
        fprintf(stderr,"Bolo zadanych prilis malo argumentov.\n\n");//Vytlaci chybu
    }

    if( chyba == VELA_ARGUMENTOV)// 3 - Chyba sa zobrazi ak bolo zadanych vela argumentov
    {
        fprintf(stderr,"Bolo zadanych prilis vela argumentov.\n\n");//Vytlaci chybu
    }

    if( chyba == VELA_ARGUMENTOV_PRE_LOG)// 4 - Chyba sa zobrazi ak bolo zadanych vela argumentov pre volbu --log
    {
        fprintf(stderr,"Bolo zadanych prilis vela argumentov pre volbu \"--log\".\n\n");//Vytlaci chybu
    }

    if( chyba == VELA_ARGUMENTOV_PRE_ITER)// 5 - Chyba sa zobrazi ak bolo zadanych vela argumentov pre volbu --iter
    {
        fprintf(stderr,"Bolo zadanych prilis vela argumentov pre volbu \"--iter\".\n\n");//Vytlaci chybu
    }

    if( chyba == MALO_ARGUMENTOV_PRE_LOG)// 6 - Chyba sa zobrazi ak bolo zadanych malo argumentov pre volbu --log
    {
        fprintf(stderr,"Bolo zadanych prilis malo argumentov pre volbu \"--log\".\n\n");//Vytlaci chybu
    }

    if( chyba == MALO_ARGUMENTOV_PRE_ITER)// 7 - Chyba sa zobrazi ak bolo zadanych malo argumentov pre volbu --iter
    {
        fprintf(stderr,"Bolo zadanych prilis malo argumentov pre volbu \"--iter\".\n\n");//Vytlaci chybu
    }

    if( chyba == CHYBA_NA_PRVOM_ARGUMENTE)// 8 - Chyba sa zobrazi ak prvy argument nebol v spravnom tvare (obsahoval nieco ine ako "--iter", "--log")
    {
        fprintf(stderr,"Prvy argument nie je v spravnom tvare.\n\n");//Vytlaci chybu
    }

    if( chyba == CHYBA_NA_DRUHOM_ARGUMENTE)// 9 - Chyba sa zobrazi ak druhy argument nebol v spravnom tvare (obsahoval nieco ine ako cisla)
    {
        fprintf(stderr,"Druhy argument nie je v spravnom tvare.\n\n");//Vytlaci chybu
    }

    if( chyba == CHYBA_NA_TRETOM_ARGUMENTE)// 10 - Chyba sa zobrazi ak treti argument nebol v spravnom tvare (obsahoval nieco ine ako cisla)
    {
        fprintf(stderr,"Treti argument nie je v spravnom tvare.\n\n");//Vytlaci chybu
    }

    if( chyba == CHYBA_NA_STVRTOM_ARGUMENTE)// 11 - Chyba sa zobrazi ak stvrty argument nebol v spravnom tvare (obsahoval nieco ine ako cisla)
    {
        fprintf(stderr,"Stvrty argument nie je v spravnom tvare\n\n");//Vytlaci chybu
    }
    fprintf(stderr,"Skontrolujte vstup a spustite program podla nasledujucej struktury: \n"//Vypise strucnu napovedu
            "./proj2 --log X N\n"
            "alebo\n"
            "./proj2 --iter MIN MAX EPS\n\n"
            "Argumenty programu:\n"
            "  --log X N poziadavka pre vypocet prirozeného logaritmu z cisla X v N iteraciach (pomocou Taylorovho polynomu a zretazeneho zlomku).\n"
            "  --iter MIN MAX EPS poziadavka pre hladanie pozadovaneho poctu iteracii pre dostatocne presny (EPS >= 1e-12) vypocet logaritmu cisla z intervalu <MIN;MAX> (program automaticky prehodi zle zadany interval).\n\n"
            );
}

