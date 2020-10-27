#!/bin/bash

# $RANDOM returns a different random integer at each invocation.
# Nominal range: 0 - 32767 (signed 16-bit integer).
# https://tldp.org/LDP/abs/html/randomvar.html

# rm salida_sem_V4.txt
# rm salida_sem_master.txt
# rm salida_sem_consuspension.txt

rm salida.txt
rm histograma.txt
# categorías salida
# echo -e "N_DATOS\tTAM_COLA\tN_LECTORES\tTAM_PAQ\tsleep_escritor\tsleep_lectores\tn_cola\tn_cola_vacia\tn_cola_llena\ttiempo" >> salida_sem_v4.txt
# echo -e "N_DATOS\tTAM_COLA\tN_LECTORES\tTAM_PAQ\tsleep_escritor\tsleep_lectores\tn_cola\tn_cola_vacia\tn_cola_llena\ttiempo" >> salida_sem_master.txt
# echo -e "N_DATOS\tTAM_COLA\tN_LECTORES\tTAM_PAQ\tsleep_escritor\tsleep_lectores\tn_cola\tn_cola_vacia\tn_cola_llena\ttiempo" >> salida_sem_consuspension.txt

# TAM_PAQ=100;

for i in {1..10} #número de pruebas
do
	
	# ./main_sem_V4 625 721 7 25 1 113 >> salida_sem_V4.txt
	# ./main_sem_master 625 721 7 25 1 113 >> salida_sem_master.txt
	# ./main_sem_consuspension 625 721 7 25 1 113 >> salida_sem_consuspension.txt
	./main_p3_e1 1050	1388	8	30	1	179 >> salida.txt 2>>histograma.txt
done
