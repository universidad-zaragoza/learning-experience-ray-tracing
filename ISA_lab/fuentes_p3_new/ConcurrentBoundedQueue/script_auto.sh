#script automatizado experimentos
# $0 nombre programa, $1 1er arg...
# $# número de argumentos después del comando

if (($# != 1)); then
	echo ["USO: ./script_auto TAM_COLA"];
else
	for j in 0 5 25 50 250 500 1000 4000
	do
		echo ["./script3_con_temp"] $1 $j;
		./script3_con_temp.sh $1 $j;
		echo "sleep 10minutos";
		sleep 600s;
	done
fi

