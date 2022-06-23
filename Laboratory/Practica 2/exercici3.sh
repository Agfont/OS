#!/bin/bash

# Comprobamos si el numero de parametros es correcto
if [ $# -ne 3 ]
then
	echo "Nombre de parametres incorrecte"
	exit 1
else
	#Guardamos los parametros en variables
	directori=$1
	extensio=$2
	valor=$3

	#Contamos el numero de archivos que cumples con lo requisitos 
	lines=$(find $directori -name "*.$extensio" -size +$valor"k" | wc -l)
	if [ $lines -eq 0 ]
	then
		echo "No hi ha cap fitxer que compleixi amb les condicions especificades."
	else
		# Creamos el directorio temporal si no existe
		mkdir -p fitxers
		#Buscamos y copiamos los archivos al directorio temporal fitxers
		find $directori -name "*.$extensio" -size +$valor"k" -exec cp {} fitxers/ \;

		#Comprimimos en tar y despues apagamos el directorio temporal
		tar -czf fitxers.tar.gz fitxers
		rm -rf fitxers
		echo "He comprimit els fitxers a fitxers.tar.gz."
	fi
	exit 0
fi
