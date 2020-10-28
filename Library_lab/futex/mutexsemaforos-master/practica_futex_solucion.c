/*--------------------------------------------------------------------------------------*/
// Name: practica_futex.c								*/
// Author: Alejandro Valero y Ruben Gran 
// Copyright: Universidad de Zaragoza
// Date: 12/12/2018
// Description: Mutex implementation and performance assesment
/*--------------------------------------------------------------------------------------*/


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

#define MAX_COUNT_EACH 100000LL
#define TRUE 1
#define FALSE 0

long long int MAXIMO = 10000000LL;
int NUM_THREADS = 240;
char MUTEX_IMPLE = 's'; //s: spin, n: naive, K: Kdrepper

struct tdata{
	int tid;
};

typedef char mipthread_mutex_t;

// Variable mutex
mipthread_mutex_t mi_mutex;

// Funciones a implementar
int mipthread_spin_init		(mipthread_mutex_t *lock);
int mipthread_spin_destroy	(mipthread_mutex_t *lock);
int mipthread_spin_lock		(mipthread_mutex_t *lock);
int mipthread_spin_unlock	(mipthread_mutex_t *lock);

int mipthread_naive_init	(mipthread_mutex_t *lock);
int mipthread_naive_destroy	(mipthread_mutex_t *lock);
int mipthread_naive_lock	(mipthread_mutex_t *lock);
int mipthread_naive_unlock	(mipthread_mutex_t *lock);

int mipthread_Kdrepper_init	(mipthread_mutex_t *lock);
int mipthread_Kdrepper_destroy	(mipthread_mutex_t *lock);
int mipthread_Kdrepper_lock	(mipthread_mutex_t *lock);
int mipthread_Kdrepper_unlock	(mipthread_mutex_t *lock);

int mipthread_spin_init		(mipthread_mutex_t *lock){

	*lock = 0;
    	return 0;

}

int mipthread_spin_destroy	(mipthread_mutex_t *lock){

	*lock = 0;
    	return 0;

}

int mipthread_spin_lock		(mipthread_mutex_t *lock){

	while (__atomic_test_and_set ( lock, __ATOMIC_SEQ_CST));	
    	return 0;

}

int mipthread_spin_unlock	(mipthread_mutex_t *lock){

	*lock = 0;
    	return 0;

}

int mipthread_naive_init	(mipthread_mutex_t *lock){

	*lock = 0;
    	return 0;

}

int mipthread_naive_destroy	(mipthread_mutex_t *lock){

	*lock = 0;
    	return 0;

}

int mipthread_naive_lock	(mipthread_mutex_t *lock){

	while (__atomic_test_and_set ( lock, __ATOMIC_SEQ_CST)){

        	syscall(__NR_futex, lock, FUTEX_WAIT, 1, NULL, 0, 0);

	}
    	return 0;

}

int mipthread_naive_unlock	(mipthread_mutex_t *lock){

	*lock = 0;
	
	syscall(__NR_futex, lock, FUTEX_WAKE, 1, NULL, 0, 0);
    	return 0;

}

int mipthread_Kdrepper_init	(mipthread_mutex_t *lock){

	*lock = 0;
    	return 0;

}

int mipthread_Kdrepper_destroy	(mipthread_mutex_t *lock){

	*lock = 0;
    	return 0;

}

int mipthread_Kdrepper_lock(mipthread_mutex_t *lock){
    int c = 0;


    __atomic_compare_exchange_n( lock, &c, 1, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);

    if (c == 0) return 0; // No contention

    if (c != 2) { // Assign 2 if it was 1 before
        c = __atomic_exchange_n( lock, 2, __ATOMIC_SEQ_CST);
    }

    while (c != 0) { // Wait until is unlocked
        if ( -1 == syscall(__NR_futex, lock, FUTEX_WAIT, 2, NULL, 0, 0)){
//	   perror("lock");
        }
        c = __atomic_exchange_n( lock, 2, __ATOMIC_SEQ_CST);
    }
	
    return 0;

}

int mipthread_Kdrepper_unlock	(mipthread_mutex_t *lock){

	if (__atomic_fetch_sub( lock, 1, __ATOMIC_SEQ_CST) != 1) {
		// There are waiters, wake one

		*lock = 0;
	
		syscall(__NR_futex, (int *)lock, FUTEX_WAKE, 1, NULL, 0, 0);
	}

}




// Variable compartida entre los threads
long long int contador_compartido = 0;
char nofin = TRUE;

void *contar(void *ptr) {
    long long int i;

	while (nofin){
	
		if (MUTEX_IMPLE == 's') mipthread_spin_lock(&mi_mutex);
		if (MUTEX_IMPLE == 'n') mipthread_naive_lock(&mi_mutex);
		if (MUTEX_IMPLE == 'K') mipthread_Kdrepper_lock(&mi_mutex);

		if(contador_compartido >= MAXIMO){

			nofin = FALSE;

		}else{

			contador_compartido = contador_compartido + 1;

		}

		if (MUTEX_IMPLE == 's') mipthread_spin_unlock(&mi_mutex);
		if (MUTEX_IMPLE == 'n') mipthread_naive_unlock(&mi_mutex);
		if (MUTEX_IMPLE == 'K') mipthread_Kdrepper_unlock(&mi_mutex);

	}

}

int main (int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int rc, i;
    struct tdata id[NUM_THREADS];

    if ((argc != 4)||((argv[1][0]!='s')&&(argv[1][0]!='n')&&(argv[1][0]!='K'))||(atoi(argv[2])>240)||(atoi(argv[2])<=0)) {
	printf ("USO: ./ejecutable [spin|naive|Kdrepper] #threads(<=240&&>0) #maximo\n");
	exit(1);
    }

    MAXIMO = atol(argv[3]);
    NUM_THREADS = atoi(argv[2]);
    MUTEX_IMPLE = argv[1][0];

    clock_t start = clock();

    if (MUTEX_IMPLE == 's') mipthread_spin_init(&mi_mutex);
    if (MUTEX_IMPLE == 'n') mipthread_naive_init(&mi_mutex);
    if (MUTEX_IMPLE == 'K') mipthread_Kdrepper_init(&mi_mutex);

    for(i=0; i<NUM_THREADS; i++) {
      id[i].tid = i;
      rc = pthread_create(&threads[i], NULL, contar, (void *) &id[i]);
    }

    for(i=0; i<NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

   printf("Tiempo transcurrido: %f segundos\n", ((double)clock() - start) / CLOCKS_PER_SEC);


    printf("Valor de contador compartido: %lld Esperado: %lld\n", contador_compartido, MAXIMO);
    if (contador_compartido == MAXIMO)
        printf("Los valores coinciden, continua!!!\n");
    else
        fprintf(stderr, "Los valores NOOOOO coinciden, deberías arreglarlo!\n");
    
    if (MUTEX_IMPLE == 's') mipthread_spin_destroy(&mi_mutex);
    if (MUTEX_IMPLE == 'n') mipthread_naive_destroy(&mi_mutex);
    if (MUTEX_IMPLE == 'K') mipthread_Kdrepper_destroy(&mi_mutex);

    return 0;
}
