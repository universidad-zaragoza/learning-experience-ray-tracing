/*--------------------------------------------------------------------------------------*/
// Name: practica_futex.c                               */
// Author: Alejandro Valero y Ruben Gran 
// Copyright: Universidad de Zaragoza
// Date: 12/12/2018
// Description: Mutex implementation and performance assesment
/*--------------------------------------------------------------------------------------*/


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

extern "C" void mi_mutex_lock(int *x);
extern "C" void mi_mutex_unlock(int *x);

mutex::mutex()
{ 
}
mutex::~mutex()
{
}

void mutex::lock()
{
    mi_mutex_lock(&mi_mutex);
}

void mutex::unlock()
{
    mi_mutex_unlock(&mi_mutex);
}

void mutex::setMutexType(char c)
{
    type = c;
}
