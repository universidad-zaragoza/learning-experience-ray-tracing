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
    //nada que hacer

}
//----------------------------------------------------------
void Semaphore::adormir(int ve){
//	suspende la ejecución del hilo si el valor de ve no coincide con el de count
	syscall(__NR_futex, &(count), FUTEX_WAIT, ve, NULL, 0, 0);
}
//----------------------------------------------------------
void Semaphore::despertar(){
// despierta a todos los hilos para los que fue suspendida su ejecución hasta el momento
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
	
	despertar();

	mtx.unlock();
}
//----------------------------------------------------------
void Semaphore::wait() {
	
	mtx.lock();

    assert(initialized);

    while(count == 0) {
		int vr = count;		
		mtx.unlock();
		adormir(vr);
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
		adormir(vr);
		mtx.lock();
    }
    count = count-n;

	mtx.unlock();
}
