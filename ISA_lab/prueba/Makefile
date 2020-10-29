#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   October 2020
# Copyright: Universidad de Zaragoza
# Coms:   A Learning Experience Toward the Understanding of Abstraction-Level Interactions in Parallel Applications.    
#*****************************************************************

#---------------------------------------------------------
# macros
RM = /bin/rm -f  # para limpiar
CC = g++         # para compilar
#---------------------------------------------------------
# vars
EJEC = ./main
BQ_LIB = ./BoundedQueue
BQ=${BQ_LIB}/BoundedQueue
CBQ_LIB=./ConcurrentBoundedQueue
CBQ=${CBQ_LIB}/ConcurrentBoundedQueue

SEM_LIB=./sem-library-mutex
SEM_FUT_BW=./sem-futex-mutex-busy-wait
SEM_FUT_SU=./sem-futex-mutex-w-suspension

SEM_L=${SEM_LIB}/Semaphore
SEM_FBW=${SEM_FUT_BW}/Semaphore
SEM_FSU=${SEM_FUT_SU}/Semaphore

PFS_FBW=${SEM_FUT_BW}/pfs
PFS_FSU=${SEM_FUT_SU}/pfs

LOG_LIB = ./Logger
LOG=${LOG_LIB}/Logger

#---------------------------------------------------------
# compile and link 
CPPFLAGS=-std=c++11 -fmax-errors=1  #-D LOGGING_MODE=false # compilation flags
CPPINC_LIB=-I. -I${BQ_LIB} -I${CBQ_LIB} -I${SEM_LIB} -I${LOG_LIB} 
CPPINC_FBW=-I. -I${BQ_LIB} -I${CBQ_LIB} -I${SEM_FUT_BW} -I${LOG_LIB} 
CPPINC_FSU=-I. -I${BQ_LIB} -I${CBQ_LIB} -I${SEM_FUT_SU} -I${LOG_LIB} 
LDFLAGS=-pthread            # linking flags
#---------------------------------------------------------

all: ${EJEC} 
#---------------------------------------------------------
# link and generate executable. 
${EJEC}: ${EJEC}.o ${SEM_L}.o ${SEM_FBW}.o ${SEM_FSU}.o ${LOG}.o ${PFS_FBW}.o ${PFS_FSU}.o
	${CC} ${EJEC}.o ${SEM_L}.o ${LOG}.o -o main_lib ${CPPINC_LIB} ${CPPFLAGS} ${LDFLAGS}
	${CC} ${EJEC}.o ${SEM_FBW}.o ${PFS_FBW}.o ${LOG}.o -o main_futex_busy_wait ${CPPINC_FBW} ${CPPFLAGS} ${LDFLAGS}
	${CC} ${EJEC}.o ${SEM_FSU}.o ${PFS_FSU}.o ${LOG}.o -o main_futex_suspension ${CPPINC_FSU} ${CPPFLAGS} ${LDFLAGS}
#---------------------------------------------------------
${EJEC}.o: ${EJEC}.cpp ${BQ}.hpp ${BQ}.cpp  ${CBQ}.hpp ${CBQ}.cpp
	${CC} -c ${EJEC}.cpp -o ${EJEC}.o ${CPPINC_LIB} ${CPPFLAGS}
#---------------------------------------------------------
${SEM_L}.o: ${SEM_L}.cpp ${SEM_L}.hpp 
	${CC} -c ${SEM_L}.cpp -o ${SEM_L}.o ${CPPINC_LIB} ${CPPFLAGS}
#---------------------------------------------------------
${PFS_FBW}.o: ${PFS_FBW}.cpp ${PFS_FBW}.hpp 
	${CC} -c ${PFS_FBW}.cpp -o ${PFS_FBW}.o ${CPPINC_FBW} ${CPPFLAGS}
#---------------------------------------------------------
${SEM_FBW}.o: ${SEM_FBW}.cpp ${SEM_FBW}.hpp 
	${CC} -c ${SEM_FBW}.cpp -o ${SEM_FBW}.o ${CPPINC_FBW} ${CPPFLAGS}
#---------------------------------------------------------
${PFS_FSU}.o: ${PFS_FSU}.cpp ${PFS_FSU}.hpp 
	${CC} -c ${PFS_FSU}.cpp -o ${PFS_FSU}.o ${CPPINC_FSU} ${CPPFLAGS}
#---------------------------------------------------------
${SEM_FSU}.o: ${SEM_FSU}.cpp ${SEM_FSU}.hpp 
	${CC} -c ${SEM_FSU}.cpp -o ${SEM_FSU}.o ${CPPINC_FSU} ${CPPFLAGS}
#---------------------------------------------------------
${LOG}.o: ${LOG}.cpp ${LOG}.hpp 
	${CC} -c ${LOG}.cpp -o ${LOG}.o ${CPPINC_LIB} ${CPPFLAGS}

#---------------------------------------------------------
# Cuidado con lo que se pone aquí, que se borra sin preguntar
clean:
	$(RM) ${EJEC}.o ${LOG}.o ${SEM_L}.o ${SEM_FBW}.o ${SEM_FSU}.o ${PFS_FSU}.o ${PFS_FBW}.o main_lib main_futex_busy_wait main_futex_suspension
