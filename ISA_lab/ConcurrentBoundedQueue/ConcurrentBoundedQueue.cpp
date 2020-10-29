/*--------------------------------------------------------------------------------------*/
// Name:          BoundedQueue.cpp								
// Authors:       Pedro Alvarez, Joaquin Ezpeleta, Emanuel A. Georgescu, Alejandro Valero,
//                Ruben Gran-Tejero
// Copyright:     Universidad de Zaragoza
// Date:          28/10/2020
// Description:   A Learning Experience Toward the Understanding of Abstraction-Level 
//                Interactions in Parallel Applications. Library lab.
//                Class specification
//*--------------------------------------------------------------------------------------*/

#include "ConcurrentBoundedQueue.hpp"
#include <cassert>


//-----------------------------------------------------
template <class T>
ConcurrentBoundedQueue<T>::ConcurrentBoundedQueue(const int N, char c) {

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
void ConcurrentBoundedQueue<T>::firstR(T &f, bool &not_empty, int * p_check_array) {

    mutex.wait();
    ADD_EVENT("firstR,BEGIN_FUNC_PROC,"+to_string(bq->length()));
    if ((bq->length() == 0)) {
        if (not_empty == true) {
            not_empty = false;
        }
        mutex.signal();
        //std::this_thread::~thread();
    }
    else {
        f = bq->first();
        p_check_array[f]++;
        bq->dequeue();
        ADD_EVENT("firstR,END_FUNC_PROC,"+to_string(bq->length()));
        AVISAR();
    }

}

template <class T>
void ConcurrentBoundedQueue<T>::AVISAR() {

    mutex.signal();

}
