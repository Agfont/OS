#!/bin/bash

# Comprobamos si el numero de parametros es correcto
if [ $# -ne 1 ]
then
    echo "Nombre de parametres incorrecte."
    exit 1

else
	word=$1
	longitud=${#word}
	newWord=$word
	
	# Añadimos carácter por carácter en sentido contraeio para obtener el palíndromo
	for ((i = longitud-2; i >= 0; i--))
	do    
    		newWord+=${word:$i:1}
	done
    	
	echo "Palindrom: ${newWord}"
	exit 0

fi

