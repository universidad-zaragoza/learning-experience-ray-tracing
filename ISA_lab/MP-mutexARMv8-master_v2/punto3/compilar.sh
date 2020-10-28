gcc mi_fa.s -c
gcc mi_mutex.s -c 
gcc mi_mutex_sleep.s -c 
g++ ej3.cpp mi_fa.o mi_mutex.o -o ej3 -lpthread
g++ ej3.cpp mi_fa.o mi_mutex_sleep.o -o ej3_sleep -lpthread
