# Interactions between Abstraction Level: Ray Tracing

This repository is part of a teaching innovation project for the Computer Engineering Degree of the University of Zaragoza.
The project is structured into 4 folders, one for each lab session of the involved course in the learning experince. In each folder, 
it can be found a working implementation of each lab. 

## OS lab

This folder contains the implementation of the three OS-level mutex alternatives referred to as Spin, Basic, and Drepper. At this level, both Basic and Drepper solutions exploit futex 
syscalls. The folder contains a 'Makefile' and the source code 'main.c'. In order to compile the 'main.c', it should be run:
'''
$prompt> make 
'''
The 'main' exectuable should be run with the following order:
'''
./main [spin|basic|Kdrepper] #threads(1<=th<=240) #MAX\_SUMA max\_rep(>=0)
'''

* \[spin|basic|Kdrepper\]: selects a mutex implementation to be run
* \#threads(1<=th<=240): defines the number of threads cooperating at execution
* \#MAX\_SUMA: number of times that the kernel code should run at most cooperatively for all threads
* max\_rep(>=0): number of times that the previous experiment is repeated

## Library lab

## ISA lab

## Application lab


