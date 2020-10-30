#pragma once

#include <iostream>

#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

class my_mutex
{
  private:
    volatile char _interlock;
    char _type;
  public:
    my_mutex(char type = 'd') : _type(type), _interlock(0) {} ;
    ~my_mutex() {};

    void lock() {

        switch (_type){
            case 's':
                {
                    while (__atomic_test_and_set ( &_interlock, __ATOMIC_SEQ_CST));
                }
                break;

            case 'b':
                {
                    while (__atomic_test_and_set ( &_interlock, __ATOMIC_SEQ_CST)){
                        syscall(__NR_futex, &_interlock, FUTEX_WAIT, 1, NULL, 0, 0);
                    }
                }
                break;

            case 'd':
                {
                    int c = 0;

                    __atomic_compare_exchange_n( &_interlock, &c, 1, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);

                    if (c == 0) return; // No contention

                    if (c != 2) { // Assign 2 if it was 1 before
                        c = __atomic_exchange_n( &_interlock, 2, __ATOMIC_SEQ_CST);
                    }

                    while (c != 0) { // Wait until is unlocked
                        if ( -1 == syscall(__NR_futex, &_interlock, FUTEX_WAIT, 2, NULL, 0, 0)){
                            //	   perror("lock");
                        }
                        c = __atomic_exchange_n( &_interlock, 2, __ATOMIC_SEQ_CST);
                    }
                }
                break;
            default:
                {
                    std::cout << "default case"<< std::endl;
                }
        }

    }
    void unlock()
{
    switch (_type){
      case's':
	_interlock = 0;
    	break;
    case 'b':
	_interlock = 0;

	syscall(__NR_futex, &_interlock, FUTEX_WAKE, 1, NULL, 0, 0);

        break;
    case 'd':

	if (__atomic_fetch_sub( &_interlock, 1, __ATOMIC_SEQ_CST) != 1) {
		// There are waiters, wake one

		_interlock = 0;

		syscall(__NR_futex, (int *) &_interlock, FUTEX_WAKE, 1, NULL, 0, 0);
	}

        break;
    default:
	std::cout << "default case"<< std::endl;


    }

}
  private:
};
