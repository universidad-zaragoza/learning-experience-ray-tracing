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


#include <Semaphore.hpp>


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
    //nothing to do

}
//----------------------------------------------------------
void Semaphore::to_sleep(int ve){
//	suspends execution if ve does not match with count
	syscall(__NR_futex, &(count), FUTEX_WAIT, ve, NULL, 0, 0);
}
//----------------------------------------------------------
void Semaphore::wakeup(){
// wake up all suspended threads
	syscall(__NR_futex, &(count), FUTEX_WAKE, INT_MAX, NULL, 0, 0);
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
	
	wakeup();

	mtx.unlock();
}
//----------------------------------------------------------
void Semaphore::wait() {
	
	mtx.lock();

    assert(initialized);

    while(count == 0) {
		int vr = count;		
		mtx.unlock();
		to_sleep(vr);
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
		int vr = count; 
		mtx.unlock();
		to_sleep(vr);
		mtx.lock();
    }
    count = count-n;

	mtx.unlock();
}
