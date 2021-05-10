#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº: 98704      Nome: Gonçalo Miguel Silva
## Nome do Módulo: lista_cidadaos.sh
## Descrição/Explicação do Módulo: 
##uso do comando sed para mudar os caracteres " | " para ":" e assim facilitar o uso do awk usado para manipular as linhas do ficheiro listagem.txt e dar output para os cidadaos.txt
## Descrição das variáveis abaixo:
###############################################################################

aux=$(date +%Y) #variável auxiliar para guardar o ano atual (irá funcionar para qualquer ano sem ser o presente 2021)
sed "s/ | /:/g" listagem.txt | awk -F "[:-]" -v ano=$aux '{print 10000+NR ":" $2 ":" ano-$6 ":" $8 ":" $10 ":" "0"}' > cidadaos.txt 
cat cidadaos.txt