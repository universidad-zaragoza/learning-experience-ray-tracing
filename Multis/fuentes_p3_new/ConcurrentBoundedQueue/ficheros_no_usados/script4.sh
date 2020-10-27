


for j in 1 2 5 10 25 50 100 500 1000 5000 10000 50000 #number of threads
do
	for i in {1..4} #spin-lock, sleep-basic, sleep-advanced or master
	do				
		if ((i==1)); then
			./main_consuspension 50000 100 $j s >> salida.txt;
		elif ((i==2)); then
			./main_consuspension 50000 100 $j n >> salida.txt;
		elif ((i==3)); then
			./main_consuspension 50000 100 $j K >> salida.txt;
		else ./main_master 50000 100 $j s >> salida.txt; #internamente ignora el tipo de mutex
		fi
	done
	
done
