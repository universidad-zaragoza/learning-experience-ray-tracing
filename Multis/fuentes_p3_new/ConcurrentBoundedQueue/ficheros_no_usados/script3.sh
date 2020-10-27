#$0 nombre programa, $1 1er arg...
#USO: ./script3.sh TAM_COLA
#USO: ./main_p3_e1 TAM_COLA N_LECTORES mutex_type(s,n,K)

for j in 1 4 8 12 16 20 24 28 32 36 40 44 48 52 56 60 64 #68 72 76 80 84 88 92 96 100 104 108 112 116 120 124 128 132 136 140 144 148 152 156 160 164 168 172 176 180 184 188 192 196 200 #number of threads
do
	for i in {1..4} #spin-lock, sleep-basic, sleep-advanced or master
	do				
		if ((i==1)); then
			./main_consuspension $1 $j s >> salida.txt; #TAM_COLA, NUM_LECTORES, MUTEX_TYPE
		elif ((i==2)); then
			./main_consuspension $1 $j n >> salida.txt;
		elif ((i==3)); then
			./main_consuspension $1 $j K >> salida.txt;
		else ./main_master $1 $j s >> salida.txt; #internamente ignora el tipo de mutex
		fi
	done
	
done
