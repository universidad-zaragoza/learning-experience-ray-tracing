/*--------------------------------------------------------------------------------------*/
// Name:          ConcurrentBoundedQueue.hpp								
// Authors:       Pedro Alvarez, Joaquin Ezpeleta, Emanuel A. Georgescu, Alejandro Valero,
//                Ruben Gran-Tejero
// Copyright:     Universidad de Zaragoza
// Date:          28/10/2020
// Description:   A Learning Experience Toward the Understanding of Abstraction-Level 
//                Interactions in Parallel Applications. Library lab.
//*--------------------------------------------------------------------------------------*/

#ifndef CONCURRENT_BOUNDED_QUEUE
#define CONCURRENT_BOUNDED_QUEUE

//#include <Semaphore_V4.hpp>
#include <Semaphore.hpp>
#include <Logger.hpp>
#include <BoundedQueue.hpp>

using namespace std;

//-----------------------------------------------------
#ifdef LOGGING_MODE
    #include <Logger.hpp>
    extern Logger logger;
    #define ADD_EVENT(e) {logger.addMessage(e);}  
#else
    #define ADD_EVENT(e)  
#endif
//-----------------------------------------------------

template <class T>
class ConcurrentBoundedQueue {
public:
    //-----------------------------------------------------
    //Pre:  0<N
    //Post: this->bq=< > AND #this=N AND this->log=nullptr
    //Com:  constructor

    ConcurrentBoundedQueue(const int N, char c='n');
    //-----------------------------------------------------
    //Pre:
    //Post:
    //Com:  destructor
    ~ConcurrentBoundedQueue();

    //-----------------------------------------------------
    //Pre:  
    //Post: this->bq=<>
    void empty();

    //-----------------------------------------------------
    //Pre:  this->bq=< d_1 ... d_n >
    //Post: this->bq=< d_1 ... d_n d > 
    //Coms: blocks until there is a free position to insert data
    void enqueue(int i);

    //-----------------------------------------------------
    //Pre:  this->bq=< d_1 ... d_n> and n>0
    //Post: this->bq=< d_2 ... d_n >
    //Coms: blocks until this->bq=< d_1 ... d_n> and n>0
    void dequeue();

    //-----------------------------------------------------
    //Pre:  this->bq=< d_1 ... d_n> and n>0
    //Post: f=d_1
    //Coms: blocks until this->bq=< d_1 ... d_n> and n>0
    void first(T &f);

    //-----------------------------------------------------
    //Pre:  this->bq=< d_1 ... d_n> and n>0
    //Post: f=d_1 and this->bq=< d_2 ... d_n>
    //Coms: blocks until que this->bq=< d_1 ... d_n> and n>0
    void firstR(T &f, bool &not_empty, int * p_check_array);

    //-----------------------------------------------------
    //Pre:  this->bq=< d_1 ... d_n >
    //Post: l=n
    void length(int &l);

    //-----------------------------------------------------
    //Pre:  
    //Post: s=#this->bq
    void size(int &s);

    //-----------------------------------------------------
    //Pre:
    //Post:
    //Com:  dump queue elements to the standard output
    void print();

    
private:
	int N;
	BoundedQueue<T> *bq;    
	Semaphore mutex;       
	void AVISAR();   
};
#include "ConcurrentBoundedQueue.cpp"
#endif
