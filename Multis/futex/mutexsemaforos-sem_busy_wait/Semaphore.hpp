//*****************************************************************
// File:   Semaphore.hpp
// Author: PSCD-Unizar
// Date:   octubre 2016
// Coms:   Especificación de una clase de semáforos, extendida, con operaciones
//         "wait(nat)" y "signal(nat)"
//         El valor transmitido en la inicialización ha de ser
//         no negativo. Si fuera negativo, da error y aborta la ejecución.
//         La semántica que implementa es la que en
//         M. Ben-Ari, "Principles of Concurrent and Distributed Programming", Addison-Wesley, 2006
//         denomina "Busy-wait" (ver pág. 120)
//         Para los casos en que el uso del semáforo va a ser exclusivamente como "mutex", Cpp
//         suministra la clase "std::mutex", cuya especificación se encuentra en 
//         http://en.cppreference.com/w/cpp/thread/mutex
//*****************************************************************

#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP


#include <assert.h>
#include <pfs.hpp>

class Semaphore {
private:
    mutex mtx;          
	int count;                    //natural asociado al semáforo  
    bool initialized;             //para manejar dos constructores distintos

public:
	//------------------------- constructores
    //Pre:
    //Post: NOT initialized
    Semaphore();

    //Pre: n>=0
    //Post: count=n AND initialized
    Semaphore(int n);

    //------------------------- destructor
    //se invoca automáticamente al cerrar el bloque
    //en que se declara el objeto
    ~Semaphore();

    //Pre: n>=0 AND NOT initialized
    //Post: initialized AND count=n
    void setInitValue(int n, char &c);
    //------------------------- operaciones estándar
    //Pre: initialized
    //Post: <count++>
    void signal();

    //Pre: initialized
    //Post: <await count>0 
    //          count-- 
    //      >
    void wait();

    //------------------------- operaciones extendidas
    //Pre: n>0 AND initialized
    //Post: <count=count+n>
    void signal(int n);

    //Pre: n>0 AND initialized
    //Post: <await count>=n 
    //          count = count-n
    //      >
    void wait(int n);
};

#endif 
