/*--------------------------------------------------------------------------------------*/
// Name:          pfs.cpp								
// Authors:       Emanuel A. Georgescu, Alejandro Valero, Ruben Gran-Tejero
// Copyright:     Universidad de Zaragoza
// Date:          28/10/2020
// Description:   A Learning Experience Toward the Understanding of Abstraction-Level 
//                Interactions in Parallel Applications. Library lab.
//                Specification of a mutex class implemented with futex system call.
//                Three approaches delivered: spin-lock, naive, Kdrepper
//*--------------------------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>
#include <iostream>
#include "pfs.hpp"

mutex::mutex()
{
	    interlock = 0;
 
}
mutex::~mutex()
{
}

void mutex::lock()
{
    if (print == 1) {
        fprintf(stderr, "%c\t", type);
        print = 0;
    }
    switch (type){
    case 's':
    {
	while (__atomic_test_and_set ( &interlock, __ATOMIC_SEQ_CST));	
    }
    	break;

    case 'b':
    {
	while (__atomic_test_and_set ( &interlock, __ATOMIC_SEQ_CST)){
             syscall(__NR_futex, &interlock, FUTEX_WAIT, 1, NULL, 0, 0);
	}
    }
        break;

    case 'd':
    {
	int c = 0;

	__atomic_compare_exchange_n( &interlock, &c, 1, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);

	if (c == 0) return; // No contention

	if (c != 2) { // Assign 2 if it was 1 before
		c = __atomic_exchange_n( &interlock, 2, __ATOMIC_SEQ_CST);
	}

	while (c != 0) { // Wait until is unlocked
        	if ( -1 == syscall(__NR_futex, &interlock, FUTEX_WAIT, 2, NULL, 0, 0)){
	//	   perror("lock");
	        }
	        c = __atomic_exchange_n( &interlock, 2, __ATOMIC_SEQ_CST);
	}
    }
        break;
    default:
        {
        	std::cout << "default case"<< std::endl;
        }
    }
    
}

void mutex::unlock()
{
    switch (type){
      case's':
	interlock = 0;
    	break;
    case 'b':
	interlock = 0;
	
	syscall(__NR_futex, &interlock, FUTEX_WAKE, 1, NULL, 0, 0);

        break;
    case 'd':
  
	if (__atomic_fetch_sub( &interlock, 1, __ATOMIC_SEQ_CST) != 1) {
		// There are waiters, wake one

		interlock = 0;
	
		syscall(__NR_futex, (int *) &interlock, FUTEX_WAKE, 1, NULL, 0, 0);
	}

        break;
    default:
		std::cout << "default case"<< std::endl;
    break;

    }

}

void mutex::setMutexType(char c)
{
    type = c;
}
