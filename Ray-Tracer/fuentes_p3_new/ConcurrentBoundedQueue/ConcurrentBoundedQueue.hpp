//*****************************************************************
// File:   ConcurrentBoundedQueue.hpp
// Author: PSCD-Unizar
// Date:   Octubre 2018
// Coms:   Especificación de una cola FIFO acotada con gestión de acceso concurrente
//*****************************************************************

#ifndef CONCURRENT_BOUNDED_QUEUE
#define CONCURRENT_BOUNDED_QUEUE

#include <Semaphore_V4.hpp>
#include <Logger.hpp>
#include <BoundedQueue.hpp>

using namespace std;

//-----------------------------------------------------
//  Compilación condicional: ¿quiero hacer "logging"?
#ifdef LOGGING_MODE
    #include <Logger.hpp>
    extern Logger logger;
    #define ADD_EVENT(e) {logger.addMessage(e);}  //generar evento
#else
    #define ADD_EVENT(e)  // nada
#endif
//-----------------------------------------------------

template <class T>
class ConcurrentBoundedQueue {
public:
    //-----------------------------------------------------
    //Pre:  0<N
    //Post: this->bq=< > AND #this=N AND this->log=nullptr
    //Com:  constructor

    ConcurrentBoundedQueue(const int N);
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
    //Coms: se bloquea hasta que haya un hueco para insertar el dato
    void enqueue(const T d);

    //-----------------------------------------------------
    //Pre:  this->bq=< d_1 ... d_n> and n>0
    //Post: this->bq=< d_2 ... d_n >
    //Coms: se bloquea hasta que this->bq=< d_1 ... d_n> and n>0
    void dequeue();

    //-----------------------------------------------------
    //Pre:  this->bq=< d_1 ... d_n> and n>0
    //Post: f=d_1
    //Coms: se bloquea hasta que this->bq=< d_1 ... d_n> and n>0
    void first(T &f);

    //-----------------------------------------------------
    //Pre:  this->bq=< d_1 ... d_n> and n>0
    //Post: f=d_1 and this->bq=< d_2 ... d_n>
    //Coms: se bloquea hasta que this->bq=< d_1 ... d_n> and n>0
    void firstR(T &f);

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
    //Com:  muestra la cola por la salida estándar, separando los elementos mediante comas
    void print();

    //----------------------------------------------------- 
private:
	int N;
	BoundedQueue<T> *bq;    //"BoundedQueue" con los datos
    //estructura para traducir el diseño mediante <await ...>
	Semaphore mutex;       //se deberá inicializar a 1 y pasarle (log,"mutex")
	Semaphore b_hay_hueco; //se deberá inicializar a 0 y pasarle (log,"b_hay_hueco")
	Semaphore b_hay_dato;  //se deberá inicializar a 0 y pasarle (log,"b_hay_dato")
	int d_hay_hueco;       //se deberán inicializar de acuerdo a la técnica propuesta
	int d_hay_dato;
	void AVISAR();   
};
// Para poder trabajar con tipos genéricos, declaración y código deben estar juntos
#include "ConcurrentBoundedQueue.cpp"
#endif