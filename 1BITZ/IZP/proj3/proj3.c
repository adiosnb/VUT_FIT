/***********************************************************************************************************************
*   Program :               proj3.c                                                                                    *
*   Author :                Adrian Tomasov                                                                             *
*   Login :                 xtomas32                                                                                   *
*   Skupina :               39 1BIB 2015                                                                               *
*   Datum odovzdania :      13.12.2015                                                                                 *
*   Projekt :               Projekt 3 - Zhlukova analyza                                                               *
***********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h> // strcmp()
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

//konstany
#define MAX_INPUT_STRING 256
#define EXPECTED_NUM_OF_INPUT 3
#define MAX_NUM_STRING  32

//error
#define E_OK 0
#define E_CANNOT_OPEN_FILE -10
#define E_WRONG_ARG -20
#define E_CANNOT_ALLOC -30
#define E_TOO_MANY_CLUSTERS -40
#define E_MISTAKE_IN_INPUT_FILE -50

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct objt_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};


/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    if (cap > 0) {
        c->obj = (struct obj_t *)malloc(sizeof(struct obj_t)*cap);
        c->capacity = cap;
        c->size = 0;

        if (c->obj == NULL){
            c->capacity = 0;
            c->size = 0;
        }
    }
    else {
        c->capacity = 0;
        c->size = 0;
        c->obj = NULL;
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    free(c->obj);
    c->obj=NULL;
    c->capacity=0;
    c->size=0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    struct obj_t *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if (c->size < c->capacity){
        c->obj[c->size].id = obj.id;
        c->obj[c->size].x = obj.x;
        c->obj[c->size].y = obj.y;
        c->size++;
    }

    else{
        if (resize_cluster(c,c->capacity + CLUSTER_CHUNK) == NULL){
            return;
        }
        c->obj[c->size].id = obj.id;
        c->obj[c->size].x = obj.x;
        c->obj[c->size].y = obj.y;
        c->size++;
    }
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    int previous_size;

    for (int i = 0 ; i < c2->size ; i++){
        previous_size = c1->size;
        append_cluster(c1,c2->obj[i]);
        if (previous_size == c1->size){
            free(c2->obj);
            return;
        }
    }
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    free(carr[idx].obj);

    for (int i = idx; i < narr - 1 ; i++){
        carr[i].size = carr[i+1].size;
        carr[i].capacity = carr[i+1].capacity;
        carr[i].obj = carr[i+1].obj;
    }

    return narr -1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    return sqrtf(powf(o1->x - o2->x,2) + powf(o1->y - o2->y,2));
}

/*
 Pocita vzdalenost dvou shluku. Vzdalenost je vypoctena na zaklade nejblizsiho
 souseda.\
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float min_dist = obj_distance(&c1->obj[0],&c2->obj[0]);
    float current_dist;

    for (int i = 0; i < c1->size ; i++){
        for (int j = 0; j < c2->size;j++){
            current_dist = obj_distance(&c1->obj[i],&c2->obj[j]);
            if (current_dist < min_dist){
                min_dist = current_dist;
            }
        }
    }
    return min_dist;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky (podle nejblizsiho souseda). Nalezene shluky
 identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do
 pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
*/
void  find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    *c1 = 1;
    *c2 = 0;
    for (int i = 1;i < narr;i++){
        for (int j = 0; j < i; j++) {
            if (cluster_distance(&carr[*c1],&carr[*c2]) > cluster_distance(&carr[i],&carr[j])){
                *c1 = i;
                *c2 = j;
            }

        }
    }

    if (*c1 > *c2){
        *c1 = *c1 + *c2;
        *c2 = *c1 - *c2;
        *c1 = *c1 - *c2;
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    int count = 0;
    int return_value = 0;
    int num_of_alocated = 0;
    long int id_obj = 0;
    struct obj_t obj;
    FILE *input_file_d;
    char input_string_line[MAX_INPUT_STRING];
    char id_str[MAX_NUM_STRING];
    char x_str[MAX_NUM_STRING];
    char y_str[MAX_NUM_STRING];
    char *rest= NULL;

    input_file_d = fopen(filename,"r");
    if (input_file_d == NULL) {
        return E_CANNOT_OPEN_FILE;
    }

    fgets(input_string_line, MAX_INPUT_STRING, input_file_d);
    if (sscanf(input_string_line, "count=%d", &count) == EOF){
        fclose(input_file_d);
        return E_MISTAKE_IN_INPUT_FILE;
    }

    *arr = (struct cluster_t *) malloc (count*sizeof(struct cluster_t));

    if (*arr == NULL){
        fclose(input_file_d);
        return E_CANNOT_ALLOC;
    }


    for (int i = 0; i < count ; i++){
        fgets(input_string_line, MAX_INPUT_STRING, input_file_d);
        if (sscanf(input_string_line, "%s %s %s", id_str,x_str,y_str) != EXPECTED_NUM_OF_INPUT){
            return_value=  E_MISTAKE_IN_INPUT_FILE;
            break;
        }

        errno = 0;
        id_obj = strtol(id_str,&rest,10);
        if (id_obj > INT_MAX || errno == ERANGE || strcmp("",rest)){
            return_value = E_MISTAKE_IN_INPUT_FILE;
            break;
        }
        else {
            obj.id = (int) id_obj;
        }

        errno = 0;
        obj.x = strtof(x_str,&rest);
        if( obj.x > 1000 || errno == ERANGE || strcmp("",rest)){
            return_value = E_MISTAKE_IN_INPUT_FILE;
            break;
        }

        errno = 0;
        obj.y = strtof(y_str,&rest);
        if( obj.y > 1000 || errno == ERANGE || strcmp("",rest)){
            return_value = E_MISTAKE_IN_INPUT_FILE;
            break;
        }


        init_cluster(&arr[0][i],CLUSTER_CHUNK);
        append_cluster(&arr[0][i],obj);
        return_value++;
        num_of_alocated = return_value;
    }

    if (return_value < 0){
        for (int i = 0; i < num_of_alocated; i++) {
            clear_cluster(&arr[0][i]);
        }
    }

    fclose(input_file_d);

    return return_value;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}


int main(int argc, char *argv[])
{
    struct cluster_t *clusters = NULL;
    int num_of_cluster = 1;
    int num_of_chunks = 0;
    long input_num_from_argv;
    char *error_check_strtol = NULL;

    int index_1 = 0;
    int index_2 = 0;

    if (!(argc<4 && argc>1)){
        fprintf(stderr,"Zle argumenty na vstupe !\n");
        return E_WRONG_ARG;
    }


    errno = 0;
    if(argc == 3) {
        input_num_from_argv = strtol(argv[2],&error_check_strtol,10);
        if (errno == ERANGE || input_num_from_argv > INT_MAX || strcmp(error_check_strtol,"")){
            fprintf(stderr,"Zadali ste zly pocet konecnych zhlukov !\n");
            fprintf(stderr,"Zhlukov je viac ako objektov alebo zhukov je menej alebo rovne 0!\n");
            return E_TOO_MANY_CLUSTERS;
        }
        else{
            num_of_cluster = (int)input_num_from_argv;
        }
    }

    num_of_chunks = load_clusters(argv[1],&clusters);

    if (num_of_chunks < 0 ) {
        switch (num_of_chunks) {
            case E_CANNOT_OPEN_FILE:
                fprintf(stderr, "Chyba pri otvarani suboru !\n");
                break;
            case E_CANNOT_ALLOC:
                fprintf(stderr, "Problem pri alokaci miesta !\n");
                break;
            case E_MISTAKE_IN_INPUT_FILE:
                fprintf(stderr, "Chyba vo vstupnom subore !\n");
                break;
            default:
                break;
        }
        for (int i = 0; i < num_of_chunks; i++) {
            clear_cluster(&clusters[i]);
        }
        free(clusters);

        return num_of_chunks;
    }

    if(num_of_cluster > num_of_chunks || num_of_cluster <=0 ){
        for (int i = 0; i < num_of_chunks; i++){
            clear_cluster(&clusters[i]);
        }
        free(clusters);
        fprintf(stderr,"Zadali ste zly pocet konecnych zhlukov !\n");
        fprintf(stderr,"Zhlukov je viac ako objektov alebo zhukov je menej alebo rovne 0!\n");
        return E_TOO_MANY_CLUSTERS;
    }

    for (int i = num_of_chunks - num_of_cluster; i > 0; i--){
        find_neighbours(clusters,num_of_chunks,&index_1,&index_2);
        merge_clusters(&clusters[index_1],&clusters[index_2]);
        if (&clusters[index_2] == NULL){
            for (int j = 0; j < num_of_chunks; j++){
                clear_cluster(&clusters[j]);
            }
            free(clusters);
            fprintf(stderr,"Problem pri alokaci miesta !\n");
            return E_CANNOT_ALLOC;
        }
        remove_cluster(clusters,num_of_chunks,index_2);
        num_of_chunks--;
    }

    print_clusters(clusters,num_of_cluster);

    for (int i = 0; i < num_of_chunks; i++){
        clear_cluster(&clusters[i]);
    }
    free(clusters);
    return E_OK;
}
