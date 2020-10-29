# Interactions between Abstraction Levels: Ray Tracing

This project consists of four lab assignments from different courses of the Degree in Computer Engineering from Universidad de Zaragoza, Spain. The labs conform a multidisciplinary project whose main objective is to provide the students of the degree of a global vision of a computer system.

This work allows students to consolidate their knowledge of atomicity, consistency, parallelism, and concurrency present in a computer system. The system to be implemented consists of a ray tracer that uses concurrent queues, which employ library functions based on mutexes or system calls like futex implemented directly in assembly language. This way, the project covers the levels of abstraction of Application, Library, Operating System, and Instruction Set Architecture, involving the courses of Computer Graphics, Distributed and Concurrent Systems Programming, Operating Systems, and Multiprocessors, respectively.

In each 2-hour lab session, a different level of abstraction is covered, adding pointers to the rest of the levels. To strengthen the vision of a whole computer system, the Raspberry Pi platform is used in all the labs. Because the distribution of courses in different academic years of the degree, the development cannot be carried out from the Application level down to the Instruction Set Architecture. Instead, students begin to develop the system in the order established by the degree. In the first lab (Library lab), the concurrent queues are implemented using library functions. In the second lab (OS lab), functions are replaced by operating system calls. The third lab (ISA lab) replaces the system calls with assembly instructions. Finally, in the last lab (Application lab) a ray tracer makes use of the concurrent queues.

The project is structured in 4 folders, one for each lab session of the involved course in the learning experince. A working lab implementation can be found in each folder.

The material is offered by the Undergraduate Student Emanuel A. Georgescu and by Professors Alejandro Valero, Rubén Gran-Tejero, Darío Suárez-Gracia, Joaquín Ezpeleta, Pedro Álvarez, Adolfo Muñoz, Luis M. Ramos, and Pablo Ibáñez. All professors are members of the Department of Computer Science and Systems Engineering (DIIS) from Universidad de Zaragoza.

## OS lab

This folder contains the implementation of the three OS-level mutex alternatives referred to as Spin, Basic, and Drepper. At this level, both Basic and Drepper solutions exploit futex 
syscalls. The folder contains a 'Makefile' and the source code 'main.c'. In order to compile the 'main.c', it should be run:

```
$prompt> make 
```

The 'main' executable should be run in accordance with the following usage:

```
$prompt> ./main [spin|basic|Kdrepper] #threads(1<=th<=240) #MAX_SUM max_rep(>=0)
```

* \[spin|basic|drepper\]: selects a mutex implementation to be run
* \#threads(1<=th<=240): defines the number of threads cooperating at execution
* \#MAX\_SUM: number of times that the kernel code should run
* max\_rep(>=0): number of times that the entire experiment is repeated

## Library lab
This folder holds the code for a Concurrent Bounded Queue, which uses a Semaphore class. Three different implantations are provided for this Semaphore class, and it can be found at folders: 'sem-library-mutex', 'sem-futex-mutex-busy-wait', and 'sem-futex-mutex-w-suspension'. The first uses mutex from the standard library to protect critical sections, addtionally, it uses conditional variables to suspend the execution in case of contention. The second and third versions use the system call futex to protect critial sections on a busy-wait or with-suspension approach, respectively. Additionally, these two last can rely on a mutex implemntation based on spin-lock, naive, and Kdrepper, as it can be seen n 'OS\_lab'. The folder contains a 'Makefile' and the source code 'main.c'. In order to compile the 'main.c', it should be run:
```
$prompt> make 
```
After the execution of the 'make' command there are provided three different executables meant to benchmark the implementation of a Concurrent Bounded Queue with the three types of underlying mutex implementations: 'main\_lib', 'main\_futex\_busy\_wait', and 'main\_futex\_suspension'.  

```
$prompt> ./main QUEUE_SIZE N_READERS MUTEX_TYPE(s,n,K) max_rep
```

* QUEUE\_SIZE: size of the concurrent bounded queue. Measured in number of item that can be read or extrated
* N\_READERS: number of concurrent reader threads to the queue
* MUTEX\_TYPE: when applicable it reffers to the approach to implement the mutex itself, as it can be seen in 'OS\_lab' 
* max\_rep(>=0): number of times that the entire experiment is repeated

## ISA lab
This folder holds the code for a Concurrent Bounded Queue, which uses a Semaphore class based on the 'sem-futex-mutex-busy-wait' implementation of previous **Library lab**. The mutex implementation to guarantee mutual exclusion in the critical sections of this Semaphore class is coded in ARMv8 assembly. 
There are provided two alternatives for the mutex assembly:
1. A spin-lock approach implemented with the ISA instructions 'ldaxr' and 'stlxr' under synchronization load-link store-conditional paradigm. 
2. An extension of the previous approach that relies on ISA instructions 'wfe' and 'sevl' to hint a low-power state of cores under a contention scenario.  
The folder contains a 'Makefile' and the source code 'main.c'. In order to compile the 'main.c', it should be run:
```
$prompt> make 
```
After the execution of the 'make' command there are provided two different executables meant to benchmark the implementation of a Concurrent Bounded Queue with the two types of underlying mutex implementations: 'main\_isa\_mtx' and 'main\_isa\_mtx\_sleep'. First one corresponds to a basic mutex implementation and the second with an extension to hint a low-power state for cores under a contention scenario.  
```
$prompt> ./main QUEUE_SIZE N_READERS max_rep
```

* QUEUE\_SIZE: size of the concurrent bounded queue. Measured in number of item that can be read or extrated
* N\_READERS: number of concurrent reader threads to the queue
* max\_rep(>=0): number of times that the entire experiment is repeated


## Application lab


