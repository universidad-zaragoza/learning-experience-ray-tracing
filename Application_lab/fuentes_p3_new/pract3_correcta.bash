#!/bin/bash
#*****************************************************************
# File:   pract3_correcta.bash
# Author: PSCD-Universidad de Zaragoza
# Date:   octubre 2018
# Coms:   script de comprobación de entrega de la práctica 3 de PSCD (18/19)
#         Funcionamiento: 
#          1) Dar permisos de ejecución al script: "chmod u+x pract3_correcta.bash"
#          2) Ejecutar mediante "pract3correcta.bash miNip" en el directorio donde esté el zip
#             a entregar, en este caso "practica3_miNip.zip". Creará un directorio temporal
#             donde descomprimirá el zip, comprobará si están los ficheros que tienen que estar
#             y si se compilan correctamente, informando de los problemas.
#             Posteriormente borra el directorio creado
#*****************************************************************


DIR_CORRECCION=$(pwd)
MAKE_FILE=Makefile_p3_e1
EJECUTABLE=main_p3_e1
FUENTE_ZIP="practica3_${1}.zip"
USER=a$1       #usuarios en Hendrix son "a" seguido del NIP
FALTAN=()      #ficheros que faltan
FICHEROS=(ConcurrentBoundedQueue/ConcurrentBoundedQueue.cpp \
	      ConcurrentBoundedQueue/ConcurrentBoundedQueue.hpp \
	      ConcurrentBoundedQueue/$EJECUTABLE.cpp)
DIRECTORIOS=(ConcurrentBoundedQueue Semaphore_V2 BoundedQueue Logger)

#si alguien usa esto como nombre de directorio, se merece el castigo de perderlo
DIR_TEMPORAL=$DIR_CORRECCION/__PSCD_NO_ES_INTERESANTE__
DIR_COMPILACION=$DIR_TEMPORAL/ConcurrentBoundedQueue

FORM=" ------> "

echo "------------------------------------------"
#----------------------------------------------------------------
#Comprueba que se invoque con un parámetro (que se supone es un NIP)
if [[ "$#" -ne 1 ]]
then
	echo "Invocar como: pract2_correcta.sh miNip"
	echo "------------------------------------------"
	exit
fi
#----------------------------------------------------------------
#  crear directorio temporal. Si existe, lo borra. Si alguien
#  había llamado a un directorio así, se lo merece
echo "${FORM}Creando directorio temporal ..."
if [ -d "$DIR_TEMPORAL" ]
then
	/bin/rm -rf $DIR_TEMPORAL
fi
mkdir $DIR_TEMPORAL
#----------------------------------------------------------------
#  mirar si ha entregado el fichero "zip"

if [ ! -f "$FUENTE_ZIP" ]
then
	FALTAN=( $FUENTE_ZIP "$FALTAN[@]")
	echo "${FORM}Falta fichero ${FUENTE_ZIP}"
else #comprobar están todos los directorios requeridos
	cp $FUENTE_ZIP $DIR_TEMPORAL/$FUENTE_ZIP
	cd $DIR_TEMPORAL
	echo "${FORM}Extrayendo zip ..."
	unzip $FUENTE_ZIP
	for dir in "${DIRECTORIOS[@]}"
	do
		if [ ! -d "$dir" ]
		then
			echo "${FORM}Falta directorio ${dir}"
			FALTAN=($dir "$FALTAN[@]")
		fi
	done
fi
#comprobar están todos los ficheros requeridos
if [[ ${#FALTAN[@]} -eq 0 ]] 
then
	for fich in "${FICHEROS[@]}"
	do
		if [ ! -f "$fich" ]
		then
			echo "${FORM}Falta fichero ${fich}"
			FALTAN=($fich "$FALTAN[@]")
		fi
	done
fi

#Si faltan directorios o ficheros, no se compila
if [[ ${#FALTAN[@]} -eq 0 ]] 
then
	cd $DIR_COMPILACION
	make -f $MAKE_FILE
	resMake=$?
	if [[ "$resMake" == 0 ]]
	then    #ha funcionado bien
		echo "${FORM}Compila correctamente"
	else
		echo "${FORM}ERROR: fallo al compilar"
	fi
else
	echo "${FORM}ERROR: faltan ficheros obligatorios"
fi

#volver al directorio original
cd $DIR_CORRECCION
#borrar el temporal
echo "${FORM}Borrando directorio temporal ..."
/bin/rm -rf $DIR_TEMPORAL*
echo "------------------------------------------"
stty erase "^?"
exit 0
