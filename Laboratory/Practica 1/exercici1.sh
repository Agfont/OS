#!/bin/bash

# Comprobamos si el numero de parametros es correcto
if [ $# -ne 1 ] 
then   
	echo "Nombre de parametres incorrecte."   
	exit 1
 
else
	argument=$1
	
	# Ordenamos los archivos conforme el tamaño de manera decresciente
	files=($(ls -lS $argument))

	# Comprobamos si el argumento es un directorio
	if [ ! -d $argument ]
	then
		echo "El argument no es un directori."	
		exit 2
	
	else
		# Si la segunda columna del array es un 0, el directorio esta vacío
		if [ ${files[1]} == 0 ]
		then
			echo "El directori especificat no conte cap fitxer."  
                	exit 3
		else
			echo "Fitxer mes gran: ${files[10]}, ${files[6]} bytes."
			exit 0	
		fi
	fi	
fi


