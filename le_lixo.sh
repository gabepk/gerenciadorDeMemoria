#!/bin/bash
#! Dar permissão: chmod +x le_lixo.sh
#! Executar script: ./script_norm.sh

a=50

until [ $a -lt 50 ]
do

	gcc alocador.c -o x
	chmod +x x
	./x

	if [ $a -eq 0 ]
	then
		break
	fi

done
