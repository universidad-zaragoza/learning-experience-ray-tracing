/*--------------------------------------------------------------------------------------*/
// Name:          Semaphore.cpp								
// Authors:       Pedro Alvarez, Joaquin Ezpeleta, Emanuel A. Georgescu, Alejandro Valero,
//                Ruben Gran-Tejero
// Copyright:     Universidad de Zaragoza
// Date:          28/10/2020
// Description:   A Learning Experience Toward the Understanding of Abstraction-Level 
//                Interactions in Parallel Applications. Library lab.
//                See Semaphore.hpp
//*--------------------------------------------------------------------------------------*/

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
#include <thread>
#include <iostream>

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
void Semaphore::setInitValue(int n, char c) {
    mtx.setMutexType(c);

    mtx.lock();

    assert(!initialized && n >= 0);

    count = n;
    initialized = true;

    mtx.unlock();
}
//----------------------------------------------------------
void Semaphore::signal() {

	mtx.lock();

    assert(initialized);

    count++;

	mtx.unlock();
}
//----------------------------------------------------------
void Semaphore::wait() {

	mtx.lock();

    assert(initialized);

    while(count == 0) {
		mtx.unlock();
		mtx.lock();
    }
    count--;

	mtx.unlock();
}
//----------------------------------------------------------
void Semaphore::signal(int n) {
	
	mtx.lock();

    assert(initialized && n>0);

    count = count+n;

	mtx.unlock();
}
//----------------------------------------------------------
void Semaphore::wait(int n) {
	
	mtx.lock();

    assert(initialized && n>0);

    while(count < n) {
		mtx.unlock();
		mtx.lock();
    }
    count = count-n;

	mtx.unlock();
}
