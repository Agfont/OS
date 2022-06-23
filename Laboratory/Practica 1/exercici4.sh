#!/bin/bash

if [ $# -ne 2 ]
then
        echo "Nombre de parametres incorrecte." #Comprobar el nombre de parametres
        exit 1

elif [ ! -d $1 ] #Comprobar si el parametre es un directori
then
		echo "El parametre no es un directori."
		exit 2
else
	directori=$1
	ext=$2 #Guardar el directori i l'extensio
	numfitxers=0 #Inicialitzar un comptador de fitxers
	mkdir fitxers #Crear el directori fitxers
	len=${#ext} #Guardar la longitud de la extensio
	for file in $(ls $directori) #Iterar per tots els fitxers del directori
	do
		namelen=$((${#file}-len)) #Guardar la longitud del nom del fitxer
		fileext=${file:namelen:len} #Guardar la extensio del fitxer
		lenstring=${#file} #Guardar la longitud del nom del fitxers i de l'extensio
		for ((i=0;i<lenstring;i++)) #Iterar per tots els caracters del nom del fitxer
		do
			if [ ${file:i:1} = "." ] #Comprobar si el caracter actual es un punt
			then
				fileext=${file:i:$(($lenstring-$i))} #Guardar la extensio del fitxer
			fi
		done
		if [ $fileext = "."$ext ] #Comprobar si la extensio del fitxer coincideix amb la demanada
		then
			numfitxers=$(($numfitxers+1)) #Augmentar el nombre de fitxers trobats amb la extensio demanada
			filedir=$directori"/"$file #Guardar la direccio del fitxer
			cp $filedir fitxers #Copiar el fitxer a la carpeta creada anteriorment
		fi
	done
fi

if [ $numfitxers -eq 0 ] #Comprobar que el nombre de fitxers trobats no es cero
then
	echo "No hi ha cap fitxer amb aquesta extensio" #Mostrar per pantalla que no hi ha coincidencies
	rmdir fitxers #Eliminar la carpeta
else
	echo "S'han guarduat els arxius a fitxers.tar.gz" #Mostrar per pantalla a on s han guardat els fitxers
	tar -czf fitxers.tar.gz fitxers #Comprimir els fitxers
	for file in $(ls fitxers) #Iterar per tots els fitxers
	do
		rm fitxers/$file #Eliminar el fitxer actual
	done
	rmdir fitxers #Eliminar la carpeta creada al principi
fi
exit 0
