#!/bin/bash
# $# number of arguments after the command
# $0 is the command, $1 is the first argument, $2 is the 2º

tInicial=$(echo -e "$(echo "scale=1; $(cat /sys/class/thermal/thermal_zone0/temp)/1000" | bc)\n")
tUmbral=`echo $tInicial + 10 | bc`
if (($# != 2)); then
	echo ["USO: ./timeScript MAX_SUMA max_rep"];
else

	for i in {1..3} #spin-lock, sleep-basic or sleep-advanced
	do
		for j in 1 4 8 12 16 20 24 28 32 36 40 44 48 52 56 60 64 #68 72 76 80 84 88 92 96 100 #number of threads
		do

			tActual1=$(echo -e "$(echo "scale=1; $(cat /sys/class/thermal/thermal_zone0/temp)/1000" | bc)\n");
			while (( $(echo "$tActual1 > $tUmbral" |bc -l) ))
			do
				sleep 5s;
				tActual1=$(echo -e "$(echo "scale=1; $(cat /sys/class/thermal/thermal_zone0/temp)/1000" | bc)\n");
			done
			
			if ((i==1)); then
				./main spin $j $1 $2 >> salida_$1_$2.txt 2>>contencion_$1_$2.txt; #imple, numThreads, limSuma, real/sint
			elif ((i==2)); then
				./main basic $j $1 $2 >> salida_$1_$2.txt 2>>contencion_$1_$2.txt;
			else ./main Kdrepper $j $1 $2 >> salida_$1_$2.txt 2>>contencion_$1_$2.txt;
			fi

			tActual2=$(echo -e "$(echo "scale=1; $(cat /sys/class/thermal/thermal_zone0/temp)/1000" | bc)\n");
			tempDiff=`echo $tActual2 - $tActual1 | bc`
			#echo "$tActual2 $tActual1";
			echo $tempDiff >>temperatura_$1_$2.txt;
			# >> appends (doesn't override) but > overrides the file
			#rm resultado_numerico_temp.txt
		done
	done
fi
