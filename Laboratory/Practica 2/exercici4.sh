#!/bin/bash

# Comprobamos si el numero de parametros es correcto
if [ $# -ne 1 ]
then
	echo "Nombre de parametres incorrecte"
	exit 1
else
	# Guardamos el parametro en una variable
	usuari=$1

	# Verificamos si el usuario no existe
	count=$(grep $usuari etc/passwd | wc -l)
        if [ $count == 0 ]; then
		echo "Aquest usuari no existeix en aquest ordinador."
		exit 2
	# Si el usuario existe
	# Redirecionamos las informaciones utiles a archivos de texto y separamos los dados
	else
	IFS=':'
	grep $usuari etc/passwd >> 'pwd.txt'
	grep $usuari etc/group >> 'gp.txt'

	idUser=($(awk '{print $1}' 'pwd.txt'))
	nameUser=($(awk -F ":" '{print $5}' 'pwd.txt'))
	users=($(awk -F ":" '{print $4}' 'pwd.txt'))
	grups=($(awk -F ":" 'BEGIN {ORS=" "}; {print $3,"("$1"),";sep=","}' 'gp.txt'))
	directory=($(awk -F ":" '{print $6}' 'pwd.txt'))
	interprete=($(awk -F ":" '{print $7}' 'pwd.txt'))

	# Imprimimos de manera estructurada
	echo "Nom de usuari: ${nameUser}"
	echo "Identificador de l’usuari: ${idUser}"
	echo "Grups als quals pertany l’usuari: ${users} (users), ${grups}"
	echo "Directori arrel de l’usuari: ${directory}"
	echo "Intèrpret per defecte: ${interprete}"

	# Removemos los archivos de texto temporales
	rm 'pwd.txt'
	rm 'gp.txt'
	fi
	exit 0
fi
