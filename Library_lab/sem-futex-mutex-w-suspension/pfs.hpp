/*--------------------------------------------------------------------------------------*/
// Name:          pfs.hpp								
// Authors:       Emanuel A. Georgescu, Alejandro Valero, Ruben Gran-Tejero
// Copyright:     Universidad de Zaragoza
// Date:          28/10/2020
// Description:   A Learning Experience Toward the Understanding of Abstraction-Level 
//                Interactions in Parallel Applications. Library lab.
//                Specification of a mutex class implemented with futex system call.
//                Three approaches delivered: spin-lock, naive, Kdrepper
//*--------------------------------------------------------------------------------------*/

#ifndef PFS_HPP
#define PFS_HPP

class mutex
{
  private:
    volatile char interlock;
    char type = 'n';
    int print = 1;
  public:
	mutex();
	~mutex();
    void lock();
    void unlock();
    void setMutexType(char c);
  private:
	//friend class Lock;
};

#endif
