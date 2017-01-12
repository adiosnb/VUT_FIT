/** \file
  *  Header file to proj3.
  *  It is about cluster's analysis.
  *
  * @author   Adrian Tomasov
  * @login	  xtomas32
  * @date     15.12.2015
  * @brief    Header file of proj3.c using doxygen documentation
  */

/**
 * @defgroup print Printing cluster functions
 * @defgroup calc  Calculations on cluster and between them
 * @defgroup oper  Operations with clusters
 */

/**
 * @brief      Structure of object in cluster, it is used in struct cluster_t where is array of objects
 */
struct obj_t {
    int id;		/**<  Unique ID of object */
    float x;	/**<  Value X of object */
    float y;	/**<  Value Y of object*/
};

/**
 * @brief      Structure of cluster
 */
struct cluster_t {
    int size;			/**< Number of objects in Cluster*/
    int capacity;		/**< Number of allocated objects */
    struct obj_t *obj;	/**< Pointer to array of objects*/
};

/**
 * @ingroup   oper
 * @brief      This function allocs memory for cap objects and set defualt values
 * @pre      c!=NULL,cap>=0
 * @post     Memory for cap objects should be alocated, if not pointer to array of objects is NULL
 * @param      c     Pointer to array o clusters
 * @param[in]  cap   Num of objects to create
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @ingroup    oper
 * @brief      Function delete every object in cluster and free the memory where they was saved
 * @param      c     Pointer to cluster, where the objects might be cleared
 */
void clear_cluster(struct cluster_t *c);

/**
 *  Chunk of cluster objects. Value recommended for reallocation
 */
extern const int CLUSTER_CHUNK;

/**
 * @ingroup     oper 
 * @brief       Function resize cluster to size of new_cap
 * 				If new_cap <=c->cap do nothing, else realloc array of objects a set new value of c->capacity
 * @pre       new_cap >= 0,c->capacity>=0,c!=NULL
 * @post      Realloc memory in c->obj to size of new_cap, if not return NULL
 * @param      c        Pointer to cluster to resize
 * @param[in]  new_cap  New capacity of the cluster
 *
 * @return     Pointer to a new resized cluster
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @ingroup   oper
 * @brief      This function append object obj to the end of cluster
 * @pre      c!=NULL
 * @post     Object is added to cluster
 * @param      c     Pointer to cluster, where obj will be apended
 * @param[in]  obj   Object, which will be apended to cluster
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @ingroup   oper
 * @brief      	Add objects from cluster c2 to cluster c1
 *				If cluster c1 is not large enough, c1 will be resized. Every objects from c2->obj will be copied to c1->obj. c2 stay untouched.
 *				After copy cluster c1 will be sorted.
 * @pre      c1!=NULL,c2!=NULL
 * @post     { description_of_the_postcondition }
 * @param      c1    Pointer to cluster, where data will be copied.
 * @param      c2    Pointer to cluster, from where data will be copied.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @ingroup   oper
 * @brief      	Remove cluster from array of cluster
 * 				Remove cluster at idx and free memory of alocated obj in that cluster and return narr - 1
 * 			
 * @pre      narr>idx,narr>0
 * @post     Cluster is removed
 * @param      carr  Pointer to array of clusters
 * @param[in]  narr  Number of clusters in array
 * @param[in]  idx   ID of cluster to delete
 *
 * @return     Number of clusters in array after delete
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @ingroup    calc
 * @brief      Calculate Euclidean distance of two objects
 * @pre        o1!=NULL,o2!=NULL
 * @param      o1    Pointer to first object
 * @param      o2    Pointer to second object
 *
 * @return     Distance between two objects
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @ingroup    calc
 * @brief      Calulate distance between two clusters
 * 			   Calculate the smallest distance between two clusters, because we used the nearest neightbour algorythm
 *
 * @pre        c1!=NULL,c2!=NULL,c1->size > 0 , c1->size > 0
 * @param      c1    Pointer to first cluster 
 * @param      c2    Pointer to second cluster
 *
 * @return     The smallest distance between custers
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @ingroup  oper
 * @brief      Find the nearest clusters
 *			   Function find 2 the nearest neightbour based on their distance in cluster array carr with num of cluster narr
 *			   Founded indexex write to pointer to integer c1 and c2.
 * @pre      carr!=NULL,narr > 0
 * @param      		carr  Pointer to array of clusters
 * @param[in]  		narr  Number of clusters
 * @param[out]      c1    ID of cluster
 * @param[out]      c2    ID of cluster
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @ingroup  oper
 * @brief      Function sort cluster based on id if objects in array o objects c->obj 
 * @pre      c!=NULL
 * @param      c     Pointer to cluster
 * @post     Sorted cluster 

 */
void sort_cluster(struct cluster_t *c);

/**
 * @ingroup  print
 * @brief      Function print value of cluster to stdout
 * @pre      c!=NULL
 * @param      c     Poiner to cluster
 */
void print_cluster(struct cluster_t *c);

/**
 * @ingroup   oper
 * @brief       Loading clusters from file to memory
 * 				
 *
 * @pre      arr!=NULL
 * @param      filename  Pointer to array o string, there is name of file
 * @param      arr       Pointer to Pointer to clusters
 *
 * @return     Number of loaded clusters
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @ingroup  print
 * @brief      Prints array of clusters
 *
 * @param      carr  Pointer to array of clusters
 * @param[in]  narr  Number of clusters
 */
void print_clusters(struct cluster_t *carr, int narr);
