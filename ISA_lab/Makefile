#*****************************************************************
# File:   Makefile
# Author: Multis-Unizar
# Date:   October 2020
# Copyright: Universidad de Zaragoza
# Coms:   A Learning Experience Toward the Understanding of Abstraction-Level Interactions in Parallel Applications.    
#*****************************************************************

#---------------------------------------------------------
# macros
RM = /bin/rm -f  # to clean up 
CC = g++         # to compile C++ source 
CC2= gcc		 # to compile asm source
#---------------------------------------------------------
# vars
EJEC = ./main
BQ_LIB = ./BoundedQueue
BQ=${BQ_LIB}/BoundedQueue
CBQ_LIB=./ConcurrentBoundedQueue
CBQ=${CBQ_LIB}/ConcurrentBoundedQueue

SEM_FUT_BW=./sem-futex-mutex-busy-wait

SEM_FBW=${SEM_FUT_BW}/Semaphore

PFS_LIB=./pfs
PFS=${PFS_LIB}/pfs

LOG_LIB = ./Logger
LOG=${LOG_LIB}/Logger

ENS_LIB=${PFS_LIB}
ISA_MUTEX=${ENS_LIB}/mi_mutex
ISA_MUTEX_SLEEP=${ENS_LIB}/mi_mutex_sleep
#---------------------------------------------------------
# compile and link 
CPPFLAGS=-std=c++11 -fmax-errors=1  #-D LOGGING_MODE=false # compilation flags
CPPINC=-I. -I${BQ_LIB} -I${CBQ_LIB} -I${PFS_LIB} -I${LOG_LIB} -I${ENS_LIB} 
LDFLAGS=-pthread            # linking flags
#---------------------------------------------------------

all: ${EJEC} 
#---------------------------------------------------------
# link and generate executable. 
${EJEC}: ${EJEC}.o ${SEM_FBW}.o ${LOG}.o ${PFS}.o ${PFS}_s.o 
	${CC} ${EJEC}.o ${SEM_FBW}.o ${PFS}.o ${LOG}.o -o main_isa_mtx ${CPPINC} -I${SEM_FUT_BW} ${CPPFLAGS} ${LDFLAGS}
	${CC} ${EJEC}.o ${SEM_FBW}.o ${PFS}_s.o ${LOG}.o -o main_isa_mtx_sleep ${CPPINC} -I${SEM_FUT_BW} ${CPPFLAGS} ${LDFLAGS}
#---------------------------------------------------------
${EJEC}.o: ${EJEC}.cpp ${BQ}.hpp ${BQ}.cpp  ${CBQ}.hpp ${CBQ}.cpp
	${CC} -c ${EJEC}.cpp -o ${EJEC}.o ${CPPINC} -I${SEM_FUT_BW} ${CPPFLAGS}
#---------------------------------------------------------
${PFS}.o: ${PFS}.cpp ${PFS}.hpp  
	${CC2} ${ISA_MUTEX}.s -c -o ${ISA_MUTEX}.o
	${CC} -c ${PFS}.cpp -o ${PFS}.o ${CPPINC} ${CPPFLAGS}
#---------------------------------------------------------
${PFS}_s.o: ${PFS}.cpp ${PFS}.hpp  
	${CC2} ${ISA_MUTEX_SLEEP}.s -c -o ${ISA_MUTEX_SLEEP}.o
	${CC} -c ${PFS}.cpp -o ${PFS}_s.o ${CPPINC} ${CPPFLAGS}
#---------------------------------------------------------
${SEM_FBW}.o: ${SEM_FBW}.cpp ${SEM_FBW}.hpp 
	${CC} -c ${SEM_FBW}.cpp -o ${SEM_FBW}.o ${CPPINC} -I${SEM_FUT_BW} ${CPPFLAGS}
#---------------------------------------------------------
${LOG}.o: ${LOG}.cpp ${LOG}.hpp 
	${CC} -c ${LOG}.cpp -o ${LOG}.o ${CPPINC} ${CPPFLAGS}

#---------------------------------------------------------
# Cuidado con lo que se pone aquí, que se borra sin preguntar
clean:
	$(RM) ${EJEC}.o ${LOG}.o ${SEM_FBW}.o ${PFS}_s.o ${PFS}.o main_isa_mtx main_isa_mtx_sleep
