//*****************************************************************
// File:   mainBoundedQueue.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2016
// Coms:   Parte del práctica 1 de PSCD
//         ejemplo de uso del TAD "BoundedQueue"
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>

#include "BoundedQueue.hpp"

using namespace std;

//-----------------------------------------------------
void pruebaInt() {
    const int N = 12;
    BoundedQueue<int> bq(N);
    
    for (int i=1; i<=N; i++) {
        bq.enqueue(i);
    }
    bq.print();
    cout << endl;

    cout << "nEl: " << bq.length() << endl;
    cout << "pri: " << bq.first() << endl;
    
    cout << "------------------------------" << endl;
    cout << "Desencolando ..." << endl;
    bq.dequeue();
    bq.print();
    cout << endl;
    cout << "------------------------------" << endl;
    cout << "Encolando el 1000 ..." << endl;
    bq.enqueue(1000);
    bq.print();
    cout << endl;
    cout << "------------------------------" << endl;
    cout << "Desencolando todos menos dos ..." << endl;

    int aEliminar = bq.length()-2;
    for (int i=0; i<aEliminar; i++) {
        bq.dequeue();
    }
    bq.print();
    cout << endl;
    cout << "------------------------------" << endl;
    cout << "Clonando ..." << endl;
    BoundedQueue<int> otraBQ(N);
    bq.clone(otraBQ);
    otraBQ.print();
    cout << endl;
    cout << "------------------------------" << endl;
}
//-----------------------------------------------------
void pruebaString() {
    const int N = 12;
    BoundedQueue<string> bq(N);
    
    for (int i=1; i<=N; i++) {
        bq.enqueue("verso_"+to_string(i));
    }
    bq.print();
    cout << endl;

    cout << "nEl: " << bq.length() << endl;
    cout << "pri: " << bq.first() << endl;
    
    cout << "------------------------------" << endl;
    cout << "Desencolando ..." << endl;
    bq.dequeue();
    bq.print();
    cout << endl;
    cout << "------------------------------" << endl;
    cout << "Encolando el 1000 ..." << endl;
    bq.enqueue("verso_"+to_string(1000));
    bq.print();
    cout << endl;
    cout << "------------------------------" << endl;
    cout << "Desencolando todos menos dos ..." << endl;
    int aEliminar = bq.length()-2;
    for (int i=0; i<aEliminar; i++) {
        bq.dequeue();
    }
    bq.print();
    cout << endl;
    cout << "------------------------------" << endl;
    cout << "Clonando ..." << endl;
    BoundedQueue<string> otraBQ(N);
    bq.clone(otraBQ);
    otraBQ.print();
    cout << endl;
    cout << "------------------------------" << endl;
}
//-----------------------------------------------------
int main() {
    pruebaInt();
    pruebaString();

    return 0;
}