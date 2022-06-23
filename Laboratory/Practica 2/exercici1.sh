#!/bin/bash

#Comprobacion del numero de parametros
if [ $# -ne 1 ]
then
	echo "Nombre de parametres incorrecte"
	exit 1
#Comprobar que el parametro es un directorio
elif [ ! -d $1 ]
then
	echo "El parametre no es un directori"
	exit 2
else
	#Guardamos el directorio en una variable
	directori=$1

	#Imprimir por pantalla el nombre del directorio y los tres archivos mas grandes en orden ascendente
	echo "Directori: $directori"
	find $directori -maxdepth 1 -type f -exec ls -s -S {} \; | sort -n | tail -n 3

	#Buscamos por los archivos del directorio si hay algun otro subdirectorio y repetimos el mismo proceso
	for file in $(ls -s -S $directori)
	do
		if [ -d $directori$file ]
		then
			$0 $directori$file
		fi
	done
	exit 0
fi
