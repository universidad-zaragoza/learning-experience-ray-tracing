//*****************************************************************
// File:   pruebaSemaforos.cpp
// Author: PSCD-Unizar
// Date:   octubre 2016
// Coms:   Ejemplo de uso de la clase "Semaphore", para la práctica 3
//         Compilar mediante el "Makefile" asociado
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>
#include <cstdlib>

#include <Semaphore.hpp>

using namespace std;


//----------------------------------------------------
//Pre:
//Post: irrelevante
void func1(int n, Semaphore& s1, Semaphore& s2) {

    for(int i=0; i<n; i++) {
        s1.wait();
        cout << "ping\n";
        s2.signal();
    }
}
//----------------------------------------------------
//Pre:
//Post: irrelevante
void func2(int n, Semaphore& s1, Semaphore& s2) {

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
void func3(int n, Semaphore& sExt) {

    sExt.wait(5);
    cout << "Me he llevado 5 de golpe\n";
    sExt.signal(3);
    cout << "Dejo 3\n";
    sExt.signal(2);
    cout << "Dejo 2 y me quedo en paz\n";
}
//----------------------------------------------------
int main() {
    Semaphore s1(1);   //usa segundo constructor
    Semaphore s2;      //usa primer constructor
    Semaphore s[2];    //array de semáforos. Usará el primer
                       //constructor para cada uno
    Semaphore* r[2];   //de momento, no usa constructor

    //================================================================
    s2.setInitValue(0); //necesario
    thread th_1(&func1, 10, ref(s1), ref(s2)),
           th_2(&func2, 10, ref(s1), ref(s2));

    th_1.join();
    th_2.join();
    cout << "----------- Fin primera fase -----------\n";
    //usamos el operador de inicialización
    //s[0].signal(); //esta invocación daría error: NO inicializado
    s[0].setInitValue(1);
    s[1].setInitValue(0);

    thread th_3(&func1, 10, ref(s[0]), ref(s[1])),
           th_4(&func2, 10, ref(s[0]), ref(s[1]));

    th_3.join();
    th_4.join();

    cout << "----------- Fin segunda fase -----------\n";
    //usamos punteros
    r[0] = new Semaphore(1); //segundo constructor
    r[1] = new Semaphore(0);

    thread th_5(&func1_2, 10, r[0], r[1]),
           th_6(&func2_2, 10, r[0], r[1]);

    th_5.join();
    th_6.join();
    //si usamos "new", el programador es responsable de liberar
    //la memoria que se ocupa con el "new", mediante "delete"
    delete r[0];
    delete r[1];

    cout << "----------- Fin tercera fase -----------\n";
    Semaphore sExt(9);

    thread th_7(&func3, 10, ref(sExt));
    th_7.join();

    cout << "----------- Fin cuarta fase  -----------\n";

    return 0;
}

