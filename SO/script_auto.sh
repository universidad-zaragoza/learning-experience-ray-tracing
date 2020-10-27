
if (($# != 1)); then
	echo ["USO: ./timeScript MAX_SUMA"];
else
	for j in 0 5 25 50 250 500 1000 4000
	do
		echo ["./timeScript.sh"] $1 $j;
		./timeScripts.sh $1 $j;
		echo "sleep 10 minutes";
		sleep 600s;
	done
fi

