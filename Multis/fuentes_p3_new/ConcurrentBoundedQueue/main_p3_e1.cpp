//*****************************************************************
// File:   main_p3_e1.cpp (esqueleto)
// Author: PSCD-Unizar
// Date:   octubre 2018
// Coms:   Parte del práctica 1 de PSCD
//         Compilar mediante el makefile asociado
//         Invocación: main_p3_e1
//*****************************************************************
 
#include <iostream>
#include <thread>
#include <string>
#include "ConcurrentBoundedQueue.hpp"

// imprimir double con máxima precisión
#include <iomanip>
#include <cmath>
#include <limits>

#include <sys/time.h>   // for gettimeofday() 

#include <atomic>

using namespace std;
//-----------------------------------------------------
//  Compilación condicional: ¿quiero hacer "logging"?
#ifdef LOGGING_MODE
    #include <Logger.hpp>
    Logger logger("_log_.log");
    // Logger logger("_log_.log, clog"); // si queremos echo de eventos en stdout
    #define ADD_EVENT(e) {logger.addMessage(e);} //generar evento
#else
    #define ADD_EVENT(e)   // nada
#endif
//-----------------------------------------------------   
double trigFunc(double entrada);

template <class T>
void insertar(ConcurrentBoundedQueue<T> &cbq, int TAM_COLA){
    for (int i = 0; i<TAM_COLA; i++) cbq.enqueue(i);
}

template <class T>
void extraer(ConcurrentBoundedQueue<T> &cbq, bool &no_vacia, int * p_vector_comprbacion, int max_rep, int ind_thread, double * p_v_trig){
    int rand_int;
    double result = 0;
    while(no_vacia){
        T v;
        cbq.firstR(v, no_vacia, p_vector_comprbacion);

        if(max_rep > 0) {
            rand_int = rand();
            for (int i=0; i < rand_int % max_rep; i++) {
                if (i==0) result = trigFunc((double)rand_int);
                else result=trigFunc(result);
            }
            p_v_trig[ind_thread] += result;
        }
    }
}

double trigFunc(double entrada) {
    return sin(entrada*786.12);
}

int main(int argc, char* argv[]) {
    srand(5);
    if (argc != 5 || ((argv[3][0]!='s')&&(argv[3][0]!='n')&&(argv[3][0]!='K')) ) {
        fprintf (stderr, "USO: ./main_p3_e1 TAM_COLA N_LECTORES mutex_type(s,n,K) max_rep\n");
        exit(1);
    }
    //argv[0] es nombre programa  
    int TAM_COLA = atoi(argv[1]); 
    int N_LECTORES = atoi(argv[2]);
    char mutex_type = argv[3][0];
    int max_rep = atoi(argv[4]);

    // variables compartidas
    ConcurrentBoundedQueue<int> cbq(TAM_COLA, mutex_type);
    bool no_vacia = true;
    int vector_comprobacion[TAM_COLA] = {0};
    int * p_vector_comprbacion = vector_comprobacion;
    double v_trig[N_LECTORES] = {0}; 
    double * p_v_trig = v_trig;

    // variables tiempo
    struct timespec tdormir;
    struct timeval start, end;
    tdormir.tv_sec = 0;

    thread pIns[1];
    thread pExt[N_LECTORES];
    
    pIns[0] = thread (&insertar<int>, ref(cbq), TAM_COLA);
    pIns[0].join();

    gettimeofday(&start, NULL);

    for (int ind_thread=0; ind_thread<N_LECTORES; ind_thread++){
        pExt[ind_thread] = thread (&extraer<int>, ref(cbq), ref(no_vacia), p_vector_comprbacion, max_rep, ind_thread, p_v_trig);
    }

    for (int i=0; i<N_LECTORES; i++){
        pExt[i].join();
    }
    gettimeofday(&end, NULL); 
    long long seconds = (long long)(end.tv_sec - start.tv_sec); 
    long long micros = (long long)((seconds * 1000000) + end.tv_usec) - (start.tv_usec); 
    double milisF = ((double)micros)/((double)1000);

    // COMPROBAR CORRECCIÓN RESULTADOS
    bool error = false;
    for (int i = 0; i<TAM_COLA; i++) {
        if ((vector_comprobacion[i] > 1) || (vector_comprobacion[i] == 0)) {
            error = true;
        }
    }

    // EXPORTAR RESULTADOS
    if (error == true) {
        fprintf(stdout, "%c\t%d\t%lf\t%d\t%d ERROR\n", mutex_type, N_LECTORES, milisF, TAM_COLA, max_rep);
        fprintf(stderr, "ERROR \n");
    }
    else {
        fprintf(stdout, "%c\t%d\t%lf\t%d\t%d\n", mutex_type, N_LECTORES, milisF, TAM_COLA, max_rep);

        // Para que el compilador lo vea como info útil
        fprintf(stderr, "TODO PERFECTO \t");
        for (int i=0; i<N_LECTORES; i++) {
            fprintf(stderr, "%f\t", v_trig[i]);
        }
        fprintf(stderr, "\n");
    }
    
    return 0;
}