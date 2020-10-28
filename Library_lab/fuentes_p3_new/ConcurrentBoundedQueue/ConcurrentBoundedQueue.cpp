//*****************************************************************
// File:   ConcurrentConcurrentBoundedQueue.cpp
// Author: PSCD-Unizar
// Date:   Octubre 2018
// Coms:   A Learning Experience Toward the Understanding of Abstraction-Level Interactions in Parallel Application
//	   PSCD lab session
//*****************************************************************

#include "ConcurrentBoundedQueue.hpp"
#include <cassert>


//-----------------------------------------------------
template <class T>
ConcurrentBoundedQueue<T>::ConcurrentBoundedQueue(const int N, char &c) {

    this->N = N;
    bq = new BoundedQueue<T>(N);
    mutex.setInitValue(1, c); //,"MUTEX_BQ_MONITOR");
}

template <class T>
ConcurrentBoundedQueue<T>::~ConcurrentBoundedQueue() {

    delete bq;
}


//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::enqueue(int i) {
    bq->enqueue(i);
}

template <class T>
void ConcurrentBoundedQueue<T>::firstR(T &f, bool &no_vacia, int * p_vector_comprobacion) {

    mutex.wait();
    ADD_EVENT("firstR,BEGIN_FUNC_PROC,"+to_string(bq->length()));
    if ((bq->length() == 0)) {
        if (no_vacia == true) {
            no_vacia = false;
        }
        mutex.signal();
        //std::this_thread::~thread();
    }
    else {
        f = bq->first();
        p_vector_comprobacion[f]++;
        bq->dequeue();
        ADD_EVENT("firstR,END_FUNC_PROC,"+to_string(bq->length()));
        AVISAR();
    }

}

template <class T>
void ConcurrentBoundedQueue<T>::AVISAR() {

    mutex.signal();

}