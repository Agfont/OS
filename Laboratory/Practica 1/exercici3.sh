#!/bin/bash

if [ $# -ne 3 ]
then
	echo "Nombre de parametres incorrecte." #Comprobar el nombre de parametres
	exit 1
elif [ ! -d $1 ] #Comprobar que el primer parametre es un directori
then
	echo "El parametre no es un directori."
	exit 2
else
	directori=$1
	total=0
	inici=$2
	final=$3
	lenin=${#inici}
	lenfi=${#final} #Guardar i inicialitzar les variables
	files=($(ls $directori)) #Guardar el contingut del directori
	len=${#files} #Guardar la longitud del directori
	for file in $(ls $directori) #Iterar per el contingut del directori
	do
		if [ -f $directori/$file ] #Comrpobar si el contingut actual es un fitxer
		then
			for word in $(cat $directori/$file) #Iterar per totes les paraules del fitxer
			do
				if [ ${#word} -ge $lenin -a ${#word} -ge $lenfi ] #Comrpobar que la paraula sigui mes llarga que el segon i tercer parametre
				then
					wordlen=$((${#word}-$lenfi)) #Guardar la posicio a on hauria de començar la part del final
					trozoin=${word:0:lenin}
					trozofi=${word:wordlen:lenfi} #Guardar el tros de l'inici i el final de la paraula actual
					if [ "$trozoin" = "$inici" -a "$trozofi" = "$final" ] #Comprobar que la paraula comença i acaba com ens interessa
					then
						echo "S'ha trobat: "$word" a "$file #Mostrar per pantalla les paraules que coincideixen
						total=$(($total+1)) #Sumar 1 al total de coincidencies
					fi
				fi
			done
		fi
	done
fi
	echo "Total de coincidencies: "$total #Mostrar per pantalla el total de coincidencies

exit 0
