# learning-experience-ray-tracing
This repository is part of a teaching innovation project for the Computer Engineering Degree of the University of Zaragoza.
It is structured into 4 forlders, one for each lab session of the involved course in the learning experince. In each folder, 
it can be found a working implementation of each lab. 

##OS lab
In this folder yo can find the implementation of the 3 alternatives to mutex. At this level the implmentation uses the futex 
syscall. The directory contains a 'Makefile' and the source code 'main.c'. In order to compile the 'main.c', it should be run:
'''
$prompt> make 
'''
The 'main' exectuable should be run with the folloing order:
'''
./main [spin|basic|Kdrepper] #threads(1<=th<=240) #MAX\_SUMA max\_rep(>=0)
'''

* \[spin|basic|Kdrepper\]: it selects the proper mutex implementation to run
* \#threads(1<=th<=240): it defines the number of threads cooperating at execution.
* \#MAX\_SUMA: number of times that the kernel code should run at most cooperatively for all threads.
* max\_rep(>=0): number of times that the previous experiment repeat

##Library lab

##ISA lab

##Application lab


