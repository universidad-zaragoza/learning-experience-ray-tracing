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
template <class T>
void insertar(ConcurrentBoundedQueue<T> &cbq, const int nD){

    for(int i=0; i<nD; i++){ 
    	int num = rand() % 100;
        cbq.enqueue(num);
        this_thread::sleep_for(chrono::milliseconds(num));
    }
}

template <class T>
void extraer(ConcurrentBoundedQueue<T> &cbq, const int nD){

    for(int i=0; i<nD; i++){
    	T v;
        cbq.firstR(v);
        this_thread::sleep_for(chrono::milliseconds(rand() % 100));
    }
}

int main(int argc, char* argv[]) {
    const int N = 10; //tamaño de la cola
    const int N_DAT = 9;
    const int N_PROC = 5;
    srand(time(NULL));

    ConcurrentBoundedQueue<int> cbq(N);


    thread pIns[N_PROC];
    thread pExt[N_PROC];

    for (int i=0; i<N_PROC; i++){
        pIns[i] = thread (&insertar<int>, ref(cbq), N_DAT);
        pExt[i] = thread (&extraer<int>, ref(cbq), N_DAT-1);
    }

    for (int i=0; i<N_PROC; i++){
        pIns[i].join();
        pExt[i].join();
    }

    cbq.print();
    return 0;
}