#!/bin/bash

# $RANDOM returns a different random integer at each invocation.
# Nominal range: 0 - 32767 (signed 16-bit integer).
# https://tldp.org/LDP/abs/html/randomvar.html

rm ultimo_comando.txt
rm salida.txt
rm histograma.txt

# reg comandos

# categorías salida
echo -e "N_DATOS\tTAM_COLA\tN_LECTORES\tTAM_PAQ\tsleep_escritor\tsleep_lectores\tn_cola\tn_cola_vacia\tn_cola_llena\ttiempo" >> salida.txt

# TAM_PAQ=100;

for i in {1..100} #número de pruebas
do
	# rm salida.txt
	FLOOR=0; # mayores a esto

	RANGE=100;
	TAM_PAQ=0   # initialize
	while [ "$TAM_PAQ" -le $FLOOR ]
	do
	  TAM_PAQ=$RANDOM
	  let "TAM_PAQ %= $RANGE"  # Scales $number down within $RANGE.
	done

	RANGE=2000;
	TAM_COLA=0   # initialize
	while [ "$TAM_COLA" -le $FLOOR ]
	do
	  TAM_COLA=$RANDOM
	  let "TAM_COLA %= $RANGE"  # Scales $number down within $RANGE.
	done

	RANGE=100;
	N_LECTORES=0 
	while [ "$N_LECTORES" -le $FLOOR ]
	do
	  N_LECTORES=$RANDOM
	  let "N_LECTORES %= $RANGE" 
	done

	RANGE=100;
	N_DATOS=0
	while [ "$N_DATOS" -le $FLOOR ]
	do
	  N_DATOS=$RANDOM
	  let "N_DATOS %= $RANGE"  
	done
	let "N_DATOS = N_DATOS*TAM_PAQ"

	RANGE=2; #ms
	sleep_escritor=0  
	while [ "$sleep_escritor" -le $FLOOR ]
	do
	  sleep_escritor=$RANDOM
	  let "sleep_escritor %= $RANGE" 
	done

	RANGE=200; #ms
	sleep_lectores=0  
	while [ "$sleep_lectores" -le $FLOOR ]
	do
	  sleep_lectores=$RANDOM
	  let "sleep_lectores %= $RANGE" 
	done

	echo "N_DATOS TAM_COLA N_LECTORES TAM_PAQ sleep_escritor sleep_lectores" >> ultimo_comando.txt
	echo "$N_DATOS $TAM_COLA $N_LECTORES $TAM_PAQ $sleep_escritor $sleep_lectores" >> ultimo_comando.txt

	./main_p3_e1 $N_DATOS $TAM_COLA $N_LECTORES $TAM_PAQ $sleep_escritor $sleep_lectores >> salida.txt 2>>histograma.txt
	rm ultimo_comando.txt
done
