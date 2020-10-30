/*--------------------------------------------------------------------------------------*/
// Name: practica_futex.c               */
// Author: Alejandro Valero y Ruben Gran 
// Copyright: Universidad de Zaragoza
// Date: 12/12/2018
// Description: Mutex implementation and performance assesment
/*--------------------------------------------------------------------------------------*/

#ifndef PFS_HPP
#define PFS_HPP

class mutex
{
  private:
    char type = 's'; //s, b, d
    int mi_mutex=0; 
  public:
  mutex();
  ~mutex();
    void lock();
    void unlock();
    void setMutexType(char c ='s');
  private:
  //friend class Lock;
};

#endif
