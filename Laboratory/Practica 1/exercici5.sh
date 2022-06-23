#!/bin/bash

# Comprobamos si el numero de parametros es correcto
if [ $# -lt 2 ]
then
        echo "Nombre de parametres incorrecte."
        exit 1

else
	# Asignación de argumentos a variables
        directori=$1
	ext1=$2
	ext2=$3
	ext3=$4

	# Inicialización de variables
	lenExt1=${#ext1}
	lenExt2=${#ext2}
	lenExt3=${#ext3}

	countExt1=0
        countExt2=0  
	countExt3=0

	# Comprobamos si el argumento es un directorio
	if [ ! -d $directori ]
        then
                echo "El argument no es un directori."
                exit 2

        else
		# Ordenamos los archivos conforme el tamaño de manera decresciente
		files=($(ls $directori))
		numFiles=${#files[*]}
		count=0
		
		# Para cada archivo en el directorio
		for file in $(ls $directori)
                do 
			# Sumamos uno al contador de archivos visitados
			currentFile=$directori/$file
			((count++))

			# Comprobamos si el argumento es un directorio
			if [ ! -d $currentFile ]
			then
				# Caso ext1
				if [ ${file:$lenFile-$lenExt1-1:$lenExt1+1} = "."$ext1 ]
                        	then
       				((countExt1++))
				# Caso ext2
       	                	elif [ ${file:$lenFile-$lenExt2-1:$lenExt2+1} = "."$ext2 ]
               	        	then
                                ((countExt2++))
				# Caso ext3
                        	elif [ ${file:$lenFile-$lenExt3-1:$lenExt3+1} = "."$ext3 ]
                        	then
                                ((countExt3++))
				fi
				
				# Si el numero de archivos visitados es el mismo que el numero de archivos del directorio, llegamos al final
				if [ $count -eq $numFiles ]
				then
				echo "${directori} ${ext1}:${countExt1} ${ext2}:${countExt2} ${ext3}:${countExt3}"

				fi

			else
				# Llamada a la recursión
                                ./$0 $currentFile $ext1 $ext2 $ext3
			fi
                done
        fi
fi
exit 0
