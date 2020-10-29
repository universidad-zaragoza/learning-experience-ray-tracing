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


#include <mutex>
#include <condition_variable>
#include <assert.h>
//#include <pfs.hpp>

//using namespace std; //mutex, condition_variable, etc.

class Semaphore {
private:
    std::mutex mtx;          
    std::condition_variable_any cv;
    int count;                    
    bool initialized;             
    int print = 1; 


public:
	//------------------------- constructors
    //Pre:
    //Post: NOT initialized
    Semaphore();

    //Pre: n>=0
    //Post: count=n AND initialized
    Semaphore(int n);

    //------------------------- destructor
    ~Semaphore();

    //Pre: n>=0 AND NOT initialized
    //Post: initialized AND count=n
    void setInitValue(int n, char c='m');
    //Pre: initialized
    //Post: <count++>
    void signal();

    //Pre: initialized
    //Post: <await count>0 
    //          count-- 
    //      >
    void wait();

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
