//*****************************************************************
// File:   ConcurrentConcurrentBoundedQueue.cpp
// Author: PSCD-Unizar
// Date:   Octubre 2018
// Coms:   Especificación de una clase cd cola limitada con gestión de acceso concurrente
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

//-----------------------------------------------------
// template <class T>
// void ConcurrentBoundedQueue<T>::empty() {

//     mutex.wait();
//     bq.empty();
//     AVISAR();
// }
//-----------------------------------------------------
//-----------------------------------------------------
// template <class T>
// void ConcurrentBoundedQueue<T>::dequeue() {

//     mutex.wait();
//     ADD_EVENT("dequeue,BEGIN_FUNC_PROC,"+to_string(bq->length()));
//     if ((bq->length() == 0)) {
//         d_hay_dato++;
//         mutex.signal();
//         b_hay_dato.wait();
//     }
//     bq->dequeue();
//     ADD_EVENT("dequeue,END_FUNC_PROC,"+to_string(bq->length()));
//     AVISAR();
// }
//-----------------------------------------------------
// template <class T>
// void ConcurrentBoundedQueue<T>::first(T &f) {

//     mutex.wait();
//     ADD_EVENT("first,BEGIN_FUNC_PROC,"+to_string(bq->length()));
//     if ((bq->length() == 0)) {
//         d_hay_dato++;
//         mutex.signal();
//         b_hay_dato.wait();
//     }
//     f = bq->first();
//     ADD_EVENT("first,END_FUNC_PROC,"+to_string(bq->length()));
//     AVISAR();
// }
//-----------------------------------------------------
//-----------------------------------------------------
// template <class T>
// void ConcurrentBoundedQueue<T>::length(int &l) {
    
//     mutex.wait();
//     ADD_EVENT("length,BEGIN_FUNC_PROC,"+to_string(bq->length()));
//     l = bq->length();
//     ADD_EVENT("length,END_FUNC_PROC,"+to_string(bq->length()));
//     AVISAR();
// }
// //-----------------------------------------------------
// template <class T>
// void ConcurrentBoundedQueue<T>::size(int &s) {
    
//     mutex.wait();
//     ADD_EVENT("size,BEGIN_FUNC_PROC,"+to_string(bq->length()));
//     s = bq->size();
//     ADD_EVENT("size,END_FUNC_PROC,"+to_string(bq->length()));
//     AVISAR();
// }
// //-----------------------------------------------------
// template <class T>
// void ConcurrentBoundedQueue<T>::print() {

//     mutex.wait();
//     ADD_EVENT("print,BEGIN_FUNC_PROC,"+to_string(bq->length()));
//     bq->print();
//     ADD_EVENT("print,END_FUNC_PROC,"+to_string(bq->length()));
//     AVISAR();
// }
