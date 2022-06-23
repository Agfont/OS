#!/bin/bash

if [ ! $# -eq 1 ]
then
  	echo "Falta argument"
   	exit 1
fi

if [ -d $1 ]
then
	echo "L' argument es un directori"
	exit 2
elif [ ! -f $1 ]
then
	echo "L' fixter no existeix"
        exit 3
else

	fitxer=$1
	elements=$(cat $fitxer)
        existeixen=0
        noExisteixen=0
        
	for i in $elements
        do
		# Miramos si existe el directorio o el archivo
        	if [ -d $i ] || [ -f $i ]
       		then
               		((existeixen++))

       		else
               		((noExisteixen++))
		fi
		
        done
        echo "Existeixen: $existeixen"
        echo "No existeixen: $noExisteixen"
	exit 0
fi

