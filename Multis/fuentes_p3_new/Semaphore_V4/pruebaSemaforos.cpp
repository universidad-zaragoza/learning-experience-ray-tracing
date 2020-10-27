//*****************************************************************
// File:   pruebaSemaforos.cpp
// Author: PSCD-Unizar
// Date:   octubre 2019
// Coms:   Ejemplo de uso de la clase "Semaphore", para la práctica 3
//         Compilar mediante el "Makefile" asociado
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>
#include <cstdlib>

#include <Semaphore_V4.hpp>

using namespace std;

//-----------------------------------------------------
//  Compilación condicional: ¿quiero hacer "logging"?
#ifdef LOGGING_MODE
    #include <Logger.hpp>
    Logger logger("_log_.log");
    // Logger logger("_log_.log, clog"); // si queremos echo de eventos en stdout
    #define ADD_EVENT(e) {logger.addMessage(e);} //generar evento
#else
    #define ADD_EVENT(e)   // nada
#endif

//----------------------------------------------------
//Pre:
//Post: irrelevante
void func1(int n, Semaphore &s1, Semaphore &s2) {

    for(int i=0; i<n; i++) {
        s1.wait();
        cout << "ping\n";
        s2.signal();
    }
}
//----------------------------------------------------
//Pre:
//Post: irrelevante
void func2(int n, Semaphore &s1, Semaphore &s2) {

    for(int i=0; i<n; i++) {
        s2.wait();
        cout << "\tpong\n";
        s1.signal();
    }
}
//----------------------------------------------------
//Pre:
//Post: irrelevante
void func1_2(int n, Semaphore* s1, Semaphore* s2) {

    for(int i=0; i<n; i++) {
        s1->wait();
        cout << "ping\n";
        s2->signal();
    }
}
//----------------------------------------------------
//Pre:
//Post: irrelevante
void func2_2(int n, Semaphore* s1, Semaphore* s2) {

    for(int i=0; i<n; i++) {
        s2->wait();
        cout << "\tpong\n";
        s1->signal();
    }
}
//----------------------------------------------------
//Pre:
//Post: irrelevante
void func3(int n, Semaphore &sExt) {

    sExt.wait(5);
    cout << "Me he llevado 5 de golpe\n";
    sExt.signal(3);
    cout << "Dejo 3\n";
    sExt.signal(2);
    cout << "Dejo 2 y me quedo en paz\n";
}
//----------------------------------------------------

int main() {

	//-------- distintos ejemplos de declaración
    Semaphore s1(1, "s1"),
              s2(2, "s2"),   //usa primer constructor
              s3;          //usa segundo constructor
    Semaphore* r[2];   //array de dos punteros a semáforo. De momento, no usa constructor
    //----------------------------------------------------
    ADD_EVENT("main,BEGIN_FUNC_PROC,0");

    thread th_1(&func1, 10, ref(s1), ref(s2)),
           th_2(&func2, 10, ref(s1), ref(s2));

    th_1.join();
    th_2.join();

    s3.setInitValue(2, "s3");

	thread th_3(&func1, 10, ref(s2), ref(s3)),
           th_4(&func2, 10, ref(s2), ref(s3));

    th_3.join();
    th_4.join();

    cout << "----------- Fin primera fase -----------\n";
    //usamos punteros
    r[0] = new Semaphore(1); //primer constructor. En caso de "logging", pondrá "NO_INFO". Será de poca utilidad
    r[1] = new Semaphore(0, "r_1");

    thread th_5(&func1_2, 10, r[0], r[1]),
           th_6(&func2_2, 10, r[0], r[1]);

    th_5.join();
    th_6.join();
    //si usamos "new", el programador es responsable de liberar
    //la memoria que se ocupa con el "new", mediante "delete"
    delete r[0];
    delete r[1];

    cout << "----------- Fin segunda fase -----------\n";
    Semaphore sExt(9, "sExt");

    thread th_7(&func3, 10, ref(sExt));
    th_7.join();

    cout << "----------- Fin tercera fase  -----------\n";

    ADD_EVENT("main,END_FUNC_PROC,0");

    return 0;
}

