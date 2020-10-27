#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   octubre 2018
# Coms:   Ejecutar "make" para compilar (o "make -f Makefile")
#         Ejecutar "make clean" para limpiar (borrar lo que genere)
#*****************************************************************

#---------------------------------------------------------
# macros
RM = /bin/rm -f  # para limpiar
CC = g++         # compilador
#---------------------------------------------------------
# opciones compilación y enlazado ("linkado")
CPPFLAGS = -I. -I${LOG_LIB} -I${SEM_LIB} -O2 -std=c++11 -fmax-errors=1 -D LOGGING_MODE=true # opciones compilación
LDFLAGS  = -pthread  # opciones de "linkado"
#---------------------------------------------------------
# vars
EJEC = pruebaSemaforos
#---------------------------------------------------------
# directorio y clase para semáforos
SEM_LIB=.
SEMAPHORE=${SEM_LIB}/Semaphore_V4
#---------------------------------------------------------
#directorio y clase para manejo de logs
LOG_LIB=../Logger_V2
LOGGER=${LOG_LIB}/Logger

all: ${EJEC}
#---------------------------------------------------------
# "linkar"
${EJEC}: ${EJEC}.o ${SEMAPHORE}.o ${LOGGER}.o
	${CC} ${EJEC}.o ${SEMAPHORE}.o ${LOGGER}.o -o ${EJEC} ${CPPFLAGS} ${LDFLAGS}

#---------------------------------------------------------
# compilar
${EJEC}.o: ${EJEC}.cpp 
	${CC} -c ${EJEC}.cpp ${CPPFLAGS}

${SEMAPHORE}.o: ${SEMAPHORE}.hpp ${SEMAPHORE}.cpp
	${CC} -c ${SEMAPHORE}.cpp -o ${SEMAPHORE}.o ${CPPFLAGS}

${LOGGER}.o: ${LOGGER}.hpp ${LOGGER}.cpp
	${CC} -c ${LOGGER}.cpp -o ${LOGGER}.o ${CPPFLAGS}

#---------------------------------------------------------
# Cuidado con lo que se pone aquí, que se borra sin preguntar
clean:
	$(RM) ${LOGGER}.o ${SEMAPHORE}.o ${EJEC}.o ${EJEC}
