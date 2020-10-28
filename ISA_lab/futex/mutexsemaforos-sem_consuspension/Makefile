#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   octubre 2016
# Coms:   Ejecutar "make" para compilar (o "make -f Makefile")
#         Ejecutar "make clean" para limpiar (borrar lo que genere)
#*****************************************************************

#---------------------------------------------------------
# macros
RM = /bin/rm -f  # para limpiar
CC = g++         # compilador
#---------------------------------------------------------
# compilación y enlazado ("linkado")
CPPFLAGS=-I. -O2 -std=c++11 -fmax-errors=1 # opciones compilación
LDFLAGS=-pthread                           # opciones de "linkado"
#---------------------------------------------------------
# vars
EJEC = pruebaSemaforos
SEM = Semaphore
MTX = pfs

all: ${EJEC}
#---------------------------------------------------------
# "linkar"
${EJEC}: ${EJEC}.o ${SEM}.o ${MTX}.o 
	${CC} ${EJEC}.o ${SEM}.o ${MTX}.o -o ${EJEC} ${CPPFLAGS} ${LDFLAGS}

#---------------------------------------------------------
# compilar
${EJEC}.o: ${EJEC}.cpp ${SEM}.hpp ${SEM}.cpp
	${CC} -c ${EJEC}.cpp ${CPPFLAGS}

${SEM}.o: ${SEM}.hpp ${SEM}.cpp
	${CC} -c ${SEM}.cpp ${CPPFLAGS}

${MTX}.o: ${MTX}.hpp ${MTX}.cpp
	${CC} -c ${MTX}.cpp ${CPPFLAGS}
#---------------------------------------------------------
# Cuidado con lo que se pone aquí, que se borra sin preguntar
clean:
	$(RM) ${MTX}.o ${SEM}.o ${EJEC}.o ${EJEC}
