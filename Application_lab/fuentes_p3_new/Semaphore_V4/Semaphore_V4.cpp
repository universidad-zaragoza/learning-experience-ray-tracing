//*****************************************************************
// File:   Semaphore_V4.cpp
// Author: PSCD-Unizar
// Date:   octubre 2016
// Coms:   Ver Semaphore_V4.hpp
//         La implementación se entenderá cuando veamos variables
//         condición y monitores
//*****************************************************************

//Infoŕmación útil para entender la implementación en
//http://en.cppreference.com/w/cpp/thread/condition_variable
//La estudiaremos con más detalle cuando tratemos en la asignatura la parte de "monitores"

#include <Semaphore_V4.hpp>

//----------------------------------------------------------
Semaphore::Semaphore(const int n, const string info) {
    assert(n >= 0);           //aborta si se viola la Pre

    count = n;
    this->info = info;
    initialized = true;       //ya está inicializado
}
//----------------------------------------------------------
Semaphore::Semaphore(const string info) {
    initialized = false;      //habrá que inicializarlo
    this->info = info;
}
//----------------------------------------------------------
Semaphore::~Semaphore() {
}
//----------------------------------------------------------
void Semaphore::setInitValue(const int n, const string info) {
    unique_lock<mutex_type> lck(mtx); //mutex para atomicidad de la función

    assert(!initialized && n >= 0);

    count = n;
    this->info = info;
    initialized = true;
}
//----------------------------------------------------------
void Semaphore::wait() {
    unique_lock<mutex_type> lck(mtx);

    assert(initialized);

    while(count == 0) {
        cv.wait(lck);
    }
    count--;

    ADD_EVENT(info + "," + "WAIT," + to_string(count));
}
//----------------------------------------------------------
void Semaphore::signal() {
    unique_lock<mutex_type> lck(mtx);

    assert(initialized);

    count++;
    cv.notify_all(); //podemos cambiar la semántica con cv.notify_one()

    ADD_EVENT(info + "," + "SIGNAL," + to_string(count));
}
//----------------------------------------------------------
void Semaphore::signal(const int n) {
    unique_lock<mutex_type> lck(mtx);

    assert(initialized && n>0);

    count = count+n;
    cv.notify_all();

    ADD_EVENT(info + "," + "SIGNAL," + to_string(count));
}
//----------------------------------------------------------
void Semaphore::wait(const int n) {
    unique_lock<mutex_type> lck(mtx);

    assert(initialized && n>0);

    while(count < n) {
        cv.wait(lck);
    }
    count = count-n;

    ADD_EVENT(info + "," + "WAIT," + to_string(count));
}
