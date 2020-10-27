#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   octubre 2017
# Coms:   Ejemplo de Makefile
#         Ejecutar 'make' para compilar (o 'make -f Makefile')
#         Ejecutar 'make' clean para limpiar (borrar lo que genere)
#*****************************************************************

#---------------------------------------------------------
# macros
RM = /bin/rm -f  # para limpiar
CC = g++         # para compilar
#---------------------------------------------------------
# para compilación y enlazado ("linkado")
CPPFLAGS=-I. -std=c++11     # opciones compilación
LDFLAGS=-pthread            # opciones de "linkado"
#---------------------------------------------------------
# vars
EJEC = mainBoundedQueue
BQ = BoundedQueue

all: ${EJEC} #lo que se generará al ejecutar "make"
#---------------------------------------------------------
# "linkar" y generar ejecutable.
${EJEC}: ${EJEC}.o
	${CC} ${EJEC}.o -o ${EJEC} ${CPPFLAGS} ${LDFLAGS}
#---------------------------------------------------------
${EJEC}.o: ${EJEC}.cpp ${BQ}.hpp ${BQ}.cpp
	${CC} -c ${EJEC}.cpp ${CPPFLAGS}

#---------------------------------------------------------
# Cuidado con lo que se pone aquí, que se borra sin preguntar
clean:
	$(RM) ${EJEC}.o ${EJEC}
