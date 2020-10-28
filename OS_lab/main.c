/*--------------------------------------------------------------------------------------*/
// Name: main.c								*/
// Authors: Emanuel A. Georgescu, Alejandro Valero, Ruben Gran-Tejero
// Copyright: Universidad de Zaragoza
// Date: 28/10/2020
// Description: A Learning Experience Toward the Understanding of Abstraction-Level Interactions in Parallel Applications.
// 		Operating Systems lab.
//*--------------------------------------------------------------------------------------*/


#include <pthread.h>
#include <stdio.h>
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

long long int MAX_SUM = 10000000LL;
int NUM_THREADS = 240;
char MUTEX_IMPL = 's'; //s: spin (SL), b: basic (BS), d: drepper (AS)

struct args {
    void *ptr; 
    int max_rep;
    double * p_v_trig;
};

struct tdata {
	int tid;
};

typedef char mypthread_mutex_t;

// mutex variable
mypthread_mutex_t my_mutex;

// functions to be implemented by the students
int mypthread_spin_init	(mypthread_mutex_t *lock);
int mypthread_spin_destroy	(mypthread_mutex_t *lock);
int mypthread_spin_lock	(mypthread_mutex_t *lock, char impl, int numThreads, int indThread);
int mypthread_spin_unlock	(mypthread_mutex_t *lock, char impl, int numThreads, int indThread);

int mypthread_basic_init	(mypthread_mutex_t *lock);
int mypthread_basic_destroy	(mypthread_mutex_t *lock);
int mypthread_basic_lock	(mypthread_mutex_t *lock, char impl, int numThreads, int indThread);
int mypthread_basic_unlock	(mypthread_mutex_t *lock, char impl, int numThreads, int indThread);

int mypthread_drepper_init	(mypthread_mutex_t *lock);
int mypthread_drepper_destroy	(mypthread_mutex_t *lock);
int mypthread_drepper_lock	(mypthread_mutex_t *lock, char impl, int numThreads, int indThread);
int mypthread_drepper_unlock	(mypthread_mutex_t *lock, char impl, int numThreads, int indThread);

int mypthread_spin_init	(mypthread_mutex_t *lock){
	*lock = 0;
    	return 0;
}

int mypthread_spin_destroy	(mypthread_mutex_t *lock){
    *lock = 0;
        return 0;
}

int mypthread_spin_lock	(mypthread_mutex_t *lock, char impl, int numThreads, int indThread){

    while(__atomic_test_and_set(lock, __ATOMIC_SEQ_CST)){}

}

int mypthread_spin_unlock	(mypthread_mutex_t *lock, char impl, int numThreads, int indThread){

    __atomic_store_n(lock, 0, __ATOMIC_SEQ_CST);
}

int mypthread_basic_init	(mypthread_mutex_t *lock){

	*lock = 0;
        return 0;
}

int mypthread_basic_destroy	(mypthread_mutex_t *lock){
	*lock = 0;
        return 0;
}

int mypthread_basic_lock	(mypthread_mutex_t *lock, char impl, int numThreads, int indThread){

    int sleep = -1;
    int c = __atomic_test_and_set(lock, __ATOMIC_SEQ_CST);
    while (c==1) {
        sleep = syscall(__NR_futex, lock, FUTEX_WAIT, c, NULL, 0, 0);
        c = __atomic_test_and_set(lock, __ATOMIC_SEQ_CST);
    }

}

int mypthread_basic_unlock	(mypthread_mutex_t *lock, char impl, int numThreads, int indThread){

    __atomic_store_n(lock, 0, __ATOMIC_SEQ_CST);
    syscall(__NR_futex, lock, FUTEX_WAKE, 1, NULL, 0, 0);
}

int mypthread_drepper_init	(mypthread_mutex_t *lock){
    *lock = 0;
        return 0;
}

int mypthread_drepper_destroy	(mypthread_mutex_t *lock){
    *lock = 0;
        return 0;
}

int mypthread_drepper_lock(mypthread_mutex_t *lock, char impl, int numThreads, int indThread){
    int c = 0;


    __atomic_compare_exchange_n( lock, &c, 1, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);

    if (c == 0) {
        return 0; // No contention
    }

    if (c != 2) { // Assign 2 if it was 1 before
        c = __atomic_exchange_n( lock, 2, __ATOMIC_SEQ_CST);
    }

    while (c != 0) { // Wait until is unlocked
        if (-1 == syscall(__NR_futex, lock, FUTEX_WAIT, 2, NULL, 0, 0)){
            // perror("lock");
        }
        c = __atomic_exchange_n( lock, 2, __ATOMIC_SEQ_CST);
    }
    
    return 0;

}

int mypthread_drepper_unlock   (mypthread_mutex_t *lock, char impl, int numThreads, int indThread){

    if (__atomic_fetch_sub( lock, 1, __ATOMIC_SEQ_CST) != 1) {
        // There are waiters, wake one
        *lock = 0;
        syscall(__NR_futex, (int *)lock, FUTEX_WAKE, 1, NULL, 0, 0);
    }
}


// Shared variable 
long long int shared_counter = 0;
char noend = TRUE;
int part_th [240];

double trigFunc(double input);

void *count(void *input) {

    int thid = (*((struct tdata *)((struct args*)input)->ptr)).tid;

    part_th[thid] = 0;

    int rand_int;
    double result = 0;
    int max_rep = ((struct args*)input)->max_rep;
    double * p_v_trig = ((struct args*)input)->p_v_trig;

	while (noend){

		if (MUTEX_IMPL == 's') mypthread_spin_lock(&my_mutex, MUTEX_IMPL, NUM_THREADS, thid);
		if (MUTEX_IMPL == 'b') mypthread_basic_lock(&my_mutex, MUTEX_IMPL, NUM_THREADS, thid);
		if (MUTEX_IMPL == 'd') mypthread_drepper_lock(&my_mutex, MUTEX_IMPL, NUM_THREADS, thid);

       
		if(shared_counter >= MAX_SUM){

			noend = FALSE;

		}else{

	            part_th[(*((struct tdata *)((struct args*)input)->ptr)).tid]++;
        	    shared_counter = shared_counter + 1;

		}

		if (MUTEX_IMPL == 's') mypthread_spin_unlock(&my_mutex, MUTEX_IMPL, NUM_THREADS, thid);
		if (MUTEX_IMPL == 'b') mypthread_basic_unlock(&my_mutex, MUTEX_IMPL, NUM_THREADS, thid);
		if (MUTEX_IMPL == 'd') mypthread_drepper_unlock(&my_mutex, MUTEX_IMPL, NUM_THREADS, thid);

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

double trigFunc(double input) {
    return sin(input*786.12);
}

int main (int argc, char *argv[]) {

    if ((argc != 5)||((argv[1][0]!='s')&&(argv[1][0]!='b')&&(argv[1][0]!='d'))||(atoi(argv[2])>240)||(atoi(argv[2])<=0)||(atoi(argv[4])<0)) {
        printf ("USAGE: ./main [spin|basic|drepper] #threads(1<=th<=240) #MAX_SUM max_rep(>=0)\n");
        exit(1);
    }

    //input arguments
    MUTEX_IMPL = argv[1][0]; // first char
    NUM_THREADS = atoi(argv[2]);
    MAX_SUM = atol(argv[3]);
    int max_rep = atoi(argv[4]);

    struct args *argus = (struct args *)malloc(sizeof(struct args)); 
    srand(5);

    struct timeval startt, endd; //gettimeofday

    pthread_t threads[NUM_THREADS];
    int rc, i;
    struct tdata id[NUM_THREADS];
    
    double v_trig[NUM_THREADS];
    for (int i = 0; i<NUM_THREADS; i++) {
        v_trig[i] = 0; 
    }
    double * p_v_trig = v_trig;

    gettimeofday(&startt, NULL); 

    if (MUTEX_IMPL == 's') mypthread_spin_init(&my_mutex);
    if (MUTEX_IMPL == 'b') mypthread_basic_init(&my_mutex);
    if (MUTEX_IMPL == 'd') mypthread_drepper_init(&my_mutex);

    argus -> max_rep = max_rep;
    argus -> p_v_trig = p_v_trig;
    for(i=0; i<NUM_THREADS; i++) {
        id[i].tid = i;
        argus -> ptr = (void *) &id[i];
        rc = pthread_create(&threads[i], NULL, count, (void *) argus);
        if (rc != 0) fprintf(stderr, "Error creating a thread\n");
    }

    for(i=0; i<NUM_THREADS; i++) {
        rc = pthread_join(threads[i], NULL);
        if (rc != 0) fprintf(stderr, "Error joining a thread\n");
    }    

    gettimeofday(&endd, NULL); 
    long long seconds = (long long)(endd.tv_sec - startt.tv_sec); 
    long long micros = (long long)((seconds * 1000000) + endd.tv_usec) - (startt.tv_usec); 
    double millisF = ((double)micros)/((double)1000); 
    

    if (shared_counter == MAX_SUM) {
        printf("%c\t%d\t%lf\n", MUTEX_IMPL, NUM_THREADS, millisF);

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
    else
        fprintf(stderr, "ERROR\n");

    if (MUTEX_IMPL == 's') mypthread_spin_destroy(&my_mutex);
    if (MUTEX_IMPL == 'b') mypthread_basic_destroy(&my_mutex);
    if (MUTEX_IMPL == 'd') mypthread_drepper_destroy(&my_mutex);

    return 0;

}
