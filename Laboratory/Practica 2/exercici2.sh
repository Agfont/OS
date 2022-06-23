#!/bin/bash
if [ $# -ne 3 ]
then
	echo "Nombre de parametres incorrecte"
	exit 1
elif [ ! -d $1 ]
then
	echo "El primer parametre no es un directori"
	exit 2
else
	#Guardar los parametros en variables
	directori=$1
	cadena1=$2
	cadena2=$3

	#Recorrer el directorio
	for file in $(ls $directori)
	do
		#Si encontramos un directorio volver a llamar al script
		if [ -d $directori$file ]
		then
			$0 $directori$file/ $cadena1 $cadena2
		else

			#Si encontramos un archivo imprimir el nombre y el numero de coincidencias
			echo $directori$file
			grep [A-z]*$cadena1[A-z]*$cadena2[A-z]* $directori$file -c
		fi
	done
	exit 0
fi
