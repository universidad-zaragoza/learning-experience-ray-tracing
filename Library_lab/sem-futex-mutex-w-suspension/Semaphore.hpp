/*--------------------------------------------------------------------------------------*/
// Name:          Semaphore.hpp								
// Authors:       Pedro Alvarez, Joaquin Ezpeleta, Emanuel A. Georgescu, Alejandro Valero,
//                Ruben Gran-Tejero
// Copyright:     Universidad de Zaragoza
// Date:          28/10/2020
// Description:   A Learning Experience Toward the Understanding of Abstraction-Level 
//                Interactions in Parallel Applications. Library lab.
//                Specificaton a Semaphore class, extended with methods "wait(nat)" and 
//                "signal(nat)". The initialization value must be non negative. In such a 
//                case, an error is raised and the execution is aborted. The implemented
//                semantic for this semaphore is the one named as "busy-wait" in M. Ben-Ari, "Principles of Concurrent 
//                and Distributed Programming", Addison-Wesley, 2006 (page 120).
//                For thos cases in which the use of the semaphores is the same as a mutex
//                C++ offers the clas "std::mutex", whose specification can be found at
//                http://en.cppreference.com/w/cpp/thread/mutex
/*--------------------------------------------------------------------------------------*/

#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>
#include <pfs.hpp>

class Semaphore {
private:
    mutex mtx;          
    int count;                    //natural asociado al semáforo  
    bool initialized;             //para manejar dos constructores distintos

    void adormir (int ve);          //dormira si ve es igual a count
    void despertar ();
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
    void setInitValue(int n, char c='m');
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
