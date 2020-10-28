//*****************************************************************
// File:   Semaphore.cpp
// Author: PSCD-Unizar
// Date:   octubre 2016
// Coms:   Ver Semaphore.h
//         La implementación se entenderá cuando veamos variables
//         condición y monitores
//*****************************************************************

//Infoŕmación útil para entender la implementación en
//http://en.cppreference.com/w/cpp/thread/condition_variable
//La estudiaremos con más detalle cuando tratemos en la asignatura la parte de "monitores"

//"The condition_variable class is a synchronization primitive that can be used to block a thread,
//or multiple threads at the same time, until:
//a notification is received from another thread
//a timeout expires, or
//a spurious wakeup occurs
//Any thread that intends to wait on std::condition_variable has to acquire a std::unique_lock first.
//The wait operations atomically release the mutex and suspend the execution of the thread. When the
//condition variable is notified, the thread is awakened, and the mutex is reacquired.
//Condition variables permit concurrent invocation of the wait, wait_for, wait_until, notify_one and
//notify_all member functions."


#include <Semaphore.hpp>
#include <iostream> //añadido para cout


//----------------------------------------------------------
Semaphore::Semaphore(int n) {
    assert(n >= 0);           //aborta si se viola la Pre
    count = n;
    initialized = true;       //ya está inicializad
	
}
//----------------------------------------------------------
Semaphore::Semaphore() {
    initialized = false;      //habrá que inicializarlo
}
//----------------------------------------------------------
Semaphore::~Semaphore() {
    //nada que hacer

}
//----------------------------------------------------------
void Semaphore::setInitValue(int n, char &c) {
    c='m';

    if (print == 1) {
        fprintf(stderr, "%c\t", c);
        print = 0;
    }

    std::unique_lock<std::mutex> lck(mtx); //std::mutex para atomicidad de la función

    assert(!initialized && n >= 0);

    count = n;
    initialized = true;
    //std::cout << "setInitValue master \n";


}
//----------------------------------------------------------
void Semaphore::signal() {
    std::unique_lock<std::mutex> lck(mtx);

    assert(initialized);

    count++;
    cv.notify_all(); //podemos cambiar la semántica con cv.notify_one()
    //std::cout << "signal master \n";

}
//----------------------------------------------------------
void Semaphore::wait() {
    std::unique_lock<std::mutex> lck(mtx);

    assert(initialized);

    while(count == 0) {
        cv.wait(lck);
    }
    count--;
    //std::cout << "wait master \n";
}
//----------------------------------------------------------
void Semaphore::signal(int n) {
    std::unique_lock<std::mutex> lck(mtx);

    assert(initialized && n>0);

    count = count+n;
    cv.notify_all(); //podemos cambiar la semántica con cv.notify_one()

}
//----------------------------------------------------------
void Semaphore::wait(int n) {
    std::unique_lock<std::mutex> lck(mtx);

    assert(initialized && n>0);

    while(count < n) {
        cv.wait(lck);
    }
    count = count-n;

}
