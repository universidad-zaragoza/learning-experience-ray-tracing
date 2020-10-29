/*--------------------------------------------------------------------------------------*/
// Name:          BoundedQueue.hpp								
// Authors:       Pedro Alvarez, Joaquin Ezpeleta, Emanuel A. Georgescu, Alejandro Valero,
//                Ruben Gran-Tejero
// Copyright:     Universidad de Zaragoza
// Date:          28/10/2020
// Description:   A Learning Experience Toward the Understanding of Abstraction-Level 
//                Interactions in Parallel Applications. Library lab.
//                Specification of a bounded FIFO queue. A queue is denoted as "< d_1 ... d_n >",
//                so that "d_1" refers to the first elemente of the queue (the oldest one) and
//                "d_n" to the last one. The condition "n>0" implies that the queue is not empty.
//                "this" refers to the queue itself. We denote as "#this", the maximum number 
//                of elements that the queue can contain.
//*--------------------------------------------------------------------------------------*/

#ifndef BOUNDED_QUEUE
#define BOUNDED_QUEUE

#include <iostream>
#include <string>

using namespace std;

template <class T>
class BoundedQueue {
public:
    //-----------------------------------------------------
    //Pre:  0<N
    //Post: this=< > AND #this=N
    //Com:  constructor
    BoundedQueue(const int N);
    //-----------------------------------------------------
    //Pre:
    //Post:
    //Com:  destructor
    ~BoundedQueue();
    //-----------------------------------------------------
    //Pre:
    //Post: this=<>
    void empty();
    //-----------------------------------------------------
    //Pre:  this=< d_1 ... d_n > AND n<#this
    //Post: this=< d_1 ... d_n d > 
    void enqueue(const T d);
    //-----------------------------------------------------
    //Pre:  this=< d_1 ... d_n > AND 0<n
    //Post: this=< d_2 ... d_n > 
    void dequeue();
    //-----------------------------------------------------
    //Pre:  this=< d_1 ... d_n > AND 0<n
    //Post: first()=d_1
    T first();
    //-----------------------------------------------------
    //Pre:  this=< d_1 ... d_n >
    //Post: length()=n
    int length();
    //-----------------------------------------------------
    //Pre:  
    //Post: size()=#this
    int size();
    //-----------------------------------------------------
    //Pre:
    //Post:
    //Com:  dumps the elements of the queue to the standard output
    void print();
    //-----------------------------------------------------
    //Pre:
    //Post: copy=this
    //Com:  Clonación de la cola
    void clone(BoundedQueue<T> &copy);
    //----------------------------------------------------- 
private:
    int N;
    int nEl;
    T *data;
    int pri, sig;
};
#include "BoundedQueue.cpp"
#endif
