//*****************************************************************
// File:   BoundedQueue.hpp
// Author: PSCD-Unizar
// Date:   septiembre 2018
// Coms:   Especificación de una cola FIFO acotada.
//         Una cola será denotada como "< d_1 ... d_n >", de manera que "d_1"
//         hace referencia al primero de la cola (el dato más antiguo) y "d_n"
//         al último. "n>0" implicará que la cola es no vacía. "this" se referirá a la propia cola.
//         Denotaremos mediante "#this" el número máximo de elementos que puede contener.
//*****************************************************************

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
    //Com:  muestra la cola por la salida estándar, separando los elementos mediante comas, y salta de línea
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
// Para poder trabajar con tipos genéricos, declaración y código deben estar juntos
#include "BoundedQueue.cpp"
#endif