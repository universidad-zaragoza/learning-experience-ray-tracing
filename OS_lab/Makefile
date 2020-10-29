#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   October 2020
# Copyright: Universidad de Zaragoza
# Coms:   A Learning Experience Toward the Understanding of Abstraction-Level Interactions in Parallel Applications.    
#*****************************************************************

main: main.o
	gcc main.o -o main -lpthread -lm

main.o:
	gcc main.c -c

clean:
	rm main main.o

