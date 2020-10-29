#*****************************************************************
# File:   Makefile
# Author: Adolof Muñoz
# Date:    2017
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
CPPFLAGS=-I. -I../fuentes_p3_new/ConcurrentBoundedQueue -I../fuentes_p3_new/Semaphore_V4 -I../fuentes_p3_new/Logger_V2  -I../fuentes_p3_new/BoundedQueue  -std=c++11     # opciones compilación
LDFLAGS=-pthread -latomic           # opciones de "linkado"
#---------------------------------------------------------
# vars
CBQ = ../fuentes_p3_new/ConcurrentBoundedQueue/ConcurrentBoundedQueue
BQ = ../fuentes_p3_new/BoundedQueue/BoundedQueue
SM = ../fuentes_p3_new/Semaphore_V4/Semaphore_V4
LG = ../fuentes_p3_new/Logger_V2/Logger
AM = ../fuentes_p3_new/Semaphore_V4/asm_mutex
all: smallpt_thd smallpt_queue_mutex_def smallpt_queue_mutex_K smallpt_queue_mutex_n smallpt_queue_mutex_s #lo que se generará al ejecutar "make"

.PHONY: smallpt_queue_mutex_def smallpt_queue_mutex_K smallpt_queue_mutex_n smallpt_queue_mutex_s #lo que se generará al ejecutar "make"
#---------------------------------------------------------
# "linkar" y generar ejecutable.
smallpt_thd:
	${CC} smallpt_thd.cpp -o smallpt_thd ${CPPFLAGS} ${LDFLAGS}
smallpt_queue_mutex_def:
	${CC} smallpt_queue.cpp ${LG}.cpp ${SM}.cpp -o $@ ${CPPFLAGS} ${LDFLAGS}
smallpt_queue_mutex_K: 
	${CC} -DMUTEX=1 smallpt_queue.cpp ${SM}.cpp ${LG}.cpp -o $@ ${CPPFLAGS} ${LDFLAGS}
smallpt_queue_mutex_s: 
	${CC} -DMUTEX=2 smallpt_queue.cpp ${SM}.cpp ${LG}.cpp -o $@ ${CPPFLAGS} ${LDFLAGS}
smallpt_queue_mutex_n:
	${CC} -DMUTEX=3 smallpt_queue.cpp ${SM}.cpp ${LG}.cpp -o $@ ${CPPFLAGS} ${LDFLAGS}
smallpt_queue_mutex_asm:
	${CC} -DMUTEX=4 smallpt_queue.cpp ${SM}.cpp ${LG}.cpp ${AM}.cpp ${AM}.s -o $@ ${CPPFLAGS} ${LDFLAGS}
smallpt_queue_mutex_asm_sleep:
	${CC} -DMUTEX=5 smallpt_queue.cpp ${SM}.cpp ${LG}.cpp ${AM}.cpp ${AM}.s -o $@ ${CPPFLAGS} ${LDFLAGS}

# Cuidado con lo que se pone aquí, que se borra sin preguntar
clean:
	$(RM) *.o core smallpt_thd smallpt_queue_mutex_* ${SM}.o ${LG}.o
