Material offered by Professors Alejandro Valero, Darío Suárez-Gracia, Rubén Gran-Tejero, Adolfo Muñoz, Joaquín Ezpeleta, Luis M. Ramos, Pedro Álvarez, and Pablo Ibáñez, all of them members of the
Department of Computer Science and Systems Engineering (DIIS) from the University of Zaragoza (UNIZAR).

E-mails: {alvabre, dario, rgran, adolfo, ezpeleta, luisma, alvaper, imarin}@unizar.es.

This material includes four lab assignments from different courses of the Degree in Computer Engineering from UNIZAR. The labs conform a multidisciplinary project whose main objective is to provide the students of the degree
of a global vision of a computer system.

This work allows students to consolidate their knowledge of atomicity, consistency, parallelism, and concurrency present in a computer system. The system to be implemented consists of a ray tracer that uses
concurrent queues, which employ library functions based on mutexes or system calls like futex implemented directly in assembly language. This way, the project covers the levels of abstraction of
Application, Library, Operating System, and Instruction Set Architecture, involving the courses of Computer Graphics, Distributed and Concurrent Systems Programming, Operating Systems, and Multiprocessors, respectively.

In each 2-hour lab session, a different level of abstraction is covered, adding pointers to the rest of the levels. To strengthen the vision of a whole computer system, the Raspberry Pi platform is used in all the labs. Because
the distribution of courses in different academic years of the degree, the development cannot be carried out from the Application level down to the Instruction Set Architecture. Instead,
students begin to develop the system in the order established by the degree. In the first lab, the concurrent queues are implemented using library functions. In the second lab, functions are replaced by operating system calls. The third lab replaces the
system calls with assembly instructions. Finally, in the last lab a ray tracer algorithm makes use of concurrent queues.
