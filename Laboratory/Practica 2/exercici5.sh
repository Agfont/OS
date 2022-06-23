#!/bin/bash

# Comprobamos si el numero de parametros es correcto
if [ $# -ne 2 ]
then
	echo "Nombre de parametres incorrecte"
	exit 1

# Comprobamos el argumento introducido
elif [  $2 != "VSZ" -a $2 != "RSS" ]
then
	echo "El segon argument ha de ser VSZ o RSS"
	exit 2
else
	# Guardamos los parametros
        usuari=$1
        tipusMemoria=$2

	# Verificamos si el usuario tiene algún proceso asociado
	count=$(ps aux | grep '^'$usuari'\b' | wc -l)
        if [ $count == 0 ]; then
		echo "En $usuari no te cap procés associat."
		exit 3
	# Imprimimos los 10 primeros procesos ordenados por el parametro inserido
	elif [ $tipusMemoria = "VSZ" ]; then
		ps aux | awk '{print $5, $6, $11}' | sort -r | head -11
	elif [ $tipusMemoria = "RSS" ]; then
		ps aux | awk '{print $6, $5, $11}' | sort -r | head -11
	fi
	exit 0
fi
