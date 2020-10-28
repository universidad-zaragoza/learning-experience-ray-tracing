#!/bin/bash
#*****************************************************************
# File:   prog_correcto.bash
# Author: PSCD-Universidad de Zaragoza
# Date:   octubre 2018
# Coms:   script de comprobación de entrega de la práctica 2 de PSCD (18/19)
#         Funcionamiento: 
#*****************************************************************

HOME_REVISION=/home/ezpeleta/Documents/clase/PSCD/Gestion/controlPracticasEntregadas/correcciones

DIR_CORRECCION=$(pwd)
MODEL_CHECKER_HOME=$HOME_REVISION/model_checker
SCRIPT_MC=$DIR_CORRECCION/mc.sh
MODEL_GENERATOR=$MODEL_CHECKER_HOME/generaModelo/main.lua

FICH_LOG=$DIR_CORRECCION/elLog.log

#si alguien usa esto como nombre de directorio, se merece el castigo de perderlo
DIR_TEMPORAL=${DIR_CORRECCION}/__PSCD_NO_ES_INTERESANTE__

echo "------------------------------------------"

cd $MODEL_CHECKER_HOME/generaModelo

echo $FICH_LOG | $MODEL_GENERATOR

$SCRIPT_MC $FICH_LOG.txt $DIR_CORRECCION/queries.txt $DIR_CORRECCION/res.txt

#mostrar solo líneas con resputestas a las queries
cat $DIR_CORRECCION/res.txt | sed -n '/^[01].*/p'

#borrar el temporal
/bin/rm -rf $DIR_TEMPORAL*
echo "------------------------------------------"

exit 0
