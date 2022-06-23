#!/bin/bash

# Comprobamos si el numero de parametros es correcto
if [ $# -ne 1 ]
then
        echo "Nombre de parametres incorrecte."   
        exit 1

else
        argument=$1
	
	# Comprobamos si el argumento es un directorio
        if [ ! -d $argument ]
        then
                echo "El argument no es un directori."  
                exit 2

        else
		# Para cada subdirectorio del directorio pasado por argumento
		for i in $(find $argument -type d)
		do	
			# Contamos el numero de archivos
			count=$(find $i -maxdepth 1 -type f | wc -l)
			echo "directori:${i} nfiles:${count}"
		done
		exit 0

	fi

fi

