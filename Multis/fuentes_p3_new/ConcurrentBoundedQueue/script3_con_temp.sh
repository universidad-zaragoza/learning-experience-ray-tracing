#USO: ./script3.sh TAM_COLA max_rep

#./main_p3_e1 TAM_COLA N_LECTORES mutex_type(s,n,K) max_rep
# $0 nombre programa, $1 1er arg...
# $# número de argumentos después del comando

tInicial=$(echo -e "$(echo "scale=1; $(cat /sys/class/thermal/thermal_zone0/temp)/1000" | bc)\n")
tUmbral=`echo $tInicial + 10 | bc`

if (($# != 2)); then
	echo ["USO: ./script3_con_temp.sh TAM_COLA max_rep"];
else
	#rm salida.txt
	#rm temperatura.txt

	for i in {1..2} # 2 tipos de master (normal y sleep)
	do
		for j in 1 4 8 12 16 20 24 28 32 36 40 44 48 52 56 60 64 #68 72 76 80 84 88 92 96 100 104 108 112 116 120 124 128 132 136 140 144 148 152 156 160 164 168 172 176 180 184 188 192 196 200 #number of threads
		do
			tActual1=$(echo -e "$(echo "scale=1; $(cat /sys/class/thermal/thermal_zone0/temp)/1000" | bc)\n");
			while (( $(echo "$tActual1 > $tUmbral" |bc -l) ))
			do
				sleep 5s;
				tActual1=$(echo -e "$(echo "scale=1; $(cat /sys/class/thermal/thermal_zone0/temp)/1000" | bc)\n");
			done

			if ((i==1)); then
				./main_normal $1 $j n $2 >> salida_$1_$2.txt;
			elif ((i==2)); then
				./main_sleep $1 $j s $2 >> salida_$1_$2.txt;
			fi

			tActual2=$(echo -e "$(echo "scale=1; $(cat /sys/class/thermal/thermal_zone0/temp)/1000" | bc)\n");
			tempDiff=`echo $tActual2 - $tActual1 | bc`
			#echo "$tActual2 $tActual1";
			echo $tempDiff >> temperatura_$1_$2.txt;

		done
	done
fi
