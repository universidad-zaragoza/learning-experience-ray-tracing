/*--------------------------------------------------------------------------------------*/
// Name: practica_futex.c								*/
// Author: Alejandro Valero y Ruben Gran 
// Copyright: Universidad de Zaragoza
// Date: 12/12/2018
// Description: Mutex implementation and performance assesment
/*--------------------------------------------------------------------------------------*/



class mutex
{
  private:
    volatile char interlock;
    char type = 'K';
  public:
	mutex();
	~mutex();
    void lock();
    void unlock();
  private:
	//friend class Lock;
};
