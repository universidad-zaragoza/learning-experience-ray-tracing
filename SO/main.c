/*--------------------------------------------------------------------------------------*/
// Name: main.c								*/
// Authorr: Alejandro Valero y Ruben Gran 
// Copyright: Universidad de Zaragoza
// Date: 12/12/2018
// Description: A Learning Experience Toward the Understanding of Abstraction-Level Interactions in Parallel Applications.
// 		Operating Systems lab.
//*--------------------------------------------------------------------------------------*/


#include <pthread.h>
#include <stdio.h> // fprintf, setbuf()
#include <stdlib.h>
#include <sys/types.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>

#include <sys/time.h>   // for gettimeofday() 
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_COUNT_EACH 100000LL
#define TRUE 1
#define FALSE 0
#define BUFSIZE 209715200

long long int MAX_SUMA = 10000000LL;
int NUM_THREADS = 240;
char MUTEX_IMPLE = 's'; //s: spin, b: basic, K: Kdrepper
int imprimir_traza = 0;

struct args {
    void *ptr; 
    int max_rep;
    double * p_v_trig;
};

struct tdata{
	int tid;
};

typedef char mipthread_mutex_t;

// mutex variable
mipthread_mutex_t mi_mutex;

// Funciones a implementar
int mipthread_spin_init		(mipthread_mutex_t *lock);
int mipthread_spin_destroy	(mipthread_mutex_t *lock);
int mipthread_spin_lock		(mipthread_mutex_t *lock, char imple, int numThreads, int indThread);
int mipthread_spin_unlock	(mipthread_mutex_t *lock, char imple, int numThreads, int indThread);

int mipthread_basic_init	(mipthread_mutex_t *lock);
int mipthread_basic_destroy	(mipthread_mutex_t *lock);
int mipthread_basic_lock	(mipthread_mutex_t *lock, char imple, int numThreads, int indThread);
int mipthread_basic_unlock	(mipthread_mutex_t *lock, char imple, int numThreads, int indThread);

int mipthread_Kdrepper_init	(mipthread_mutex_t *lock);
int mipthread_Kdrepper_destroy	(mipthread_mutex_t *lock);
int mipthread_Kdrepper_lock	 (mipthread_mutex_t *lock, char imple, int numThreads, int indThread);
int mipthread_Kdrepper_unlock(mipthread_mutex_t *lock, char imple, int numThreads, int indThread);

int mipthread_spin_init		(mipthread_mutex_t *lock){
	*lock = 0;
    	return 0;
}

int mipthread_spin_destroy	(mipthread_mutex_t *lock){
    *lock = 0;
        return 0;
}

int mipthread_spin_lock		(mipthread_mutex_t *lock, char imple, int numThreads, int indThread){

    while(__atomic_test_and_set(lock, __ATOMIC_SEQ_CST)){}

}

int mipthread_spin_unlock	(mipthread_mutex_t *lock, char imple, int numThreads, int indThread){

    __atomic_store_n(lock, 0, __ATOMIC_SEQ_CST);

    //other way:
    //*lock = 0;

}

int mipthread_basic_init	(mipthread_mutex_t *lock){

	*lock = 0;
        return 0;
}

int mipthread_basic_destroy	(mipthread_mutex_t *lock){
	*lock = 0;
        return 0;
}

int mipthread_basic_lock	(mipthread_mutex_t *lock, char imple, int numThreads, int indThread){

    int duerme = -1;
    int c = __atomic_test_and_set(lock, __ATOMIC_SEQ_CST);
    while (c==1) {
        duerme = syscall(__NR_futex, lock, FUTEX_WAIT, c, NULL, 0, 0);
        c = __atomic_test_and_set(lock, __ATOMIC_SEQ_CST);
    }

}

int mipthread_basic_unlock	(mipthread_mutex_t *lock, char imple, int numThreads, int indThread){

    __atomic_store_n(lock, 0, __ATOMIC_SEQ_CST);
    syscall(__NR_futex, lock, FUTEX_WAKE, 1, NULL, 0, 0);
}

int mipthread_Kdrepper_init	(mipthread_mutex_t *lock){
    *lock = 0;
        return 0;
}

int mipthread_Kdrepper_destroy	(mipthread_mutex_t *lock){
    *lock = 0;
        return 0;
}

int mipthread_Kdrepper_lock(mipthread_mutex_t *lock, char imple, int numThreads, int indThread){
    int c = 0;


    __atomic_compare_exchange_n( lock, &c, 1, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);

    if (c == 0) {
        return 0; // No contention
    }

    if (c != 2) { // Assign 2 if it was 1 before
        c = __atomic_exchange_n( lock, 2, __ATOMIC_SEQ_CST);
    }

    while (c != 0) { // Wait until is unlocked
        if (imprimir_traza) fprintf(stderr, "%c\t%d\tespera\t%d\t\n", MUTEX_IMPLE, NUM_THREADS, indThread);
        if (-1 == syscall(__NR_futex, lock, FUTEX_WAIT, 2, NULL, 0, 0)){
            // perror("lock");
        }
        c = __atomic_exchange_n( lock, 2, __ATOMIC_SEQ_CST);
    }
    
    return 0;

}

int mipthread_Kdrepper_unlock   (mipthread_mutex_t *lock, char imple, int numThreads, int indThread){

    if (__atomic_fetch_sub( lock, 1, __ATOMIC_SEQ_CST) != 1) {
        // There are waiters, wake one
        *lock = 0;
        syscall(__NR_futex, (int *)lock, FUTEX_WAKE, 1, NULL, 0, 0);
    }

}


// Shared variable 
long long int shared_counter = 0;
char nofin = TRUE;
int participa_th [240];

struct timespec tdormir;

double trigFunc(double entrada);

void *contar(void *input) {

    int thid = (*((struct tdata *)((struct args*)input)->ptr)).tid;

    participa_th[thid] = 0;

    int rand_int;
    double result = 0;
    int max_rep = ((struct args*)input)->max_rep;
    double * p_v_trig = ((struct args*)input)->p_v_trig;

	while (nofin){

		if (MUTEX_IMPLE == 's') mipthread_spin_lock(&mi_mutex, MUTEX_IMPLE, NUM_THREADS, thid);
		if (MUTEX_IMPLE == 'b') mipthread_basic_lock(&mi_mutex, MUTEX_IMPLE, NUM_THREADS, thid);
		if (MUTEX_IMPLE == 'K') mipthread_Kdrepper_lock(&mi_mutex, MUTEX_IMPLE, NUM_THREADS, thid);

       
		if(shared_counter >= MAX_SUMA){

			nofin = FALSE;

		}else{

	            participa_th[(*((struct tdata *)((struct args*)input)->ptr)).tid] ++;
        	    shared_counter = shared_counter + 1;

		}

		if (MUTEX_IMPLE == 's') mipthread_spin_unlock(&mi_mutex, MUTEX_IMPLE, NUM_THREADS, thid);
		if (MUTEX_IMPLE == 'b') mipthread_basic_unlock(&mi_mutex, MUTEX_IMPLE, NUM_THREADS, thid);
		if (MUTEX_IMPLE == 'K') mipthread_Kdrepper_unlock(&mi_mutex, MUTEX_IMPLE, NUM_THREADS, thid);

        if(max_rep > 0) {
            rand_int = rand();
            for (int i=0; i < rand_int % max_rep; i++) {
                if (i==0) result = trigFunc((double)rand_int);
                else result=trigFunc(result);
            }
            p_v_trig[thid] += result;
        }

    }

}

double trigFunc(double entrada) {
    return sin(entrada*786.12);
}

char buf[BUFSIZE];

int main (int argc, char *argv[]) {

    if ((argc != 5)||((argv[1][0]!='s')&&(argv[1][0]!='b')&&(argv[1][0]!='K'))||(atoi(argv[2])>240)||(atoi(argv[2])<=0)||(atoi(argv[4])<0)) {
        printf ("USO: ./main [spin|basic|Kdrepper] #threads(1<=th<=240) #MAX_SUMA max_rep(>=0)\n");
        exit(1);
    }

    //input arguments
    MUTEX_IMPLE = argv[1][0]; // primer carácter
    NUM_THREADS = atoi(argv[2]);
    MAX_SUMA = atol(argv[3]);
    int max_rep = atoi(argv[4]);

    struct args *argumentos = (struct args *)malloc(sizeof(struct args)); 
    srand(5);

    struct timeval startt, endd; //gettimeofday
    tdormir.tv_sec = 0;

    pthread_t threads[NUM_THREADS];
    int rc, i;
    struct tdata id[NUM_THREADS];
    
    double v_trig[NUM_THREADS];
    for (int i = 0; i<NUM_THREADS; i++) {
        v_trig[i] = 0; 
    }
    double * p_v_trig = v_trig;

    gettimeofday(&startt, NULL); 

    if (MUTEX_IMPLE == 's') mipthread_spin_init(&mi_mutex);
    if (MUTEX_IMPLE == 'b') mipthread_basic_init(&mi_mutex);
    if (MUTEX_IMPLE == 'K') mipthread_Kdrepper_init(&mi_mutex);

    argumentos -> max_rep = max_rep;
    argumentos -> p_v_trig = p_v_trig;
    for(i=0; i<NUM_THREADS; i++) {
        id[i].tid = i;
        argumentos -> ptr = (void *) &id[i];
        rc = pthread_create(&threads[i], NULL, contar, (void *) argumentos);
        if (rc != 0) printf("error al crear un hilo \n");
    }

    for(i=0; i<NUM_THREADS; i++) {
        rc = pthread_join(threads[i], NULL);
        if (rc != 0) printf("error al crear un hilo \n");
    }    

    gettimeofday(&endd, NULL); 
    long long seconds = (long long)(endd.tv_sec - startt.tv_sec); 
    long long micros = (long long)((seconds * 1000000) + endd.tv_usec) - (startt.tv_usec); 
    double milisF = ((double)micros)/((double)1000); 
    

    if (contador_compartido == MAX_SUMA) {

        printf("%c\t%d\t%lf\n", MUTEX_IMPLE, NUM_THREADS, milisF); // AÑADIDO

        // export intermediate results 
        FILE * fp;
        fp = fopen ("temp_results.txt", "w+");
        for (int i=0; i<NUM_THREADS; i++) {
            fprintf(fp, "%f\t", v_trig[i]);
        }
        fprintf(fp, "\n");
        fflush(fp);
        fclose(fp);

    }
    else {
        printf("ERROR\n"); 
        fprintf(stderr, "ERROR\n");
    }

    if (MUTEX_IMPLE == 's') mipthread_spin_destroy(&mi_mutex);
    if (MUTEX_IMPLE == 'b') mipthread_basic_destroy(&mi_mutex);
    if (MUTEX_IMPLE == 'K') mipthread_Kdrepper_destroy(&mi_mutex);

    return 0;

}
