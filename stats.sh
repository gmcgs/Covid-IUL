#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº: 98704      Nome: Gonçalo Miguel Silva
## Nome do Módulo: stats.sh
## Descrição/Explicação do Módulo: 
##
## Ndecidadaos -> guarda o número de cidadãos da localidade inserida
## 
## Descrição nos comentários abaixo:
##       |           |
##       v           v
###############################################################################

# if para verificar se foi de facto introduzido algum argumento
if [ -z $1 ]; then

  echo "Erro: Deve usar um dos argumentos disponíveis:"
  echo $0 "cidadaos <localidade>"
  echo $0 "registados"
  echo $0 "enfermeiros"
  
else
  # if para caso o argumento introduzido seja para devolver o nº de cidadãos de uma determinada localidade
  if [ $1 == "cidadaos" ]; then
    NdeCidadaos=$(cut -d ":" -f3,4,5 cidadaos.txt | grep -i -c ":$2:")
    echo "O número de cidadãos registados em" $2 "é" $NdeCidadaos"."
    exit 0

  # if para caso o argumento introduzido seja para devolver uma lista com os cidadãos com mais de 60 anos registados no sistema
  elif [ $1 == "registados" ]; then
    cut -d ":" -f1,2,3 cidadaos.txt | sort -t: -k 3 -n -r | awk -F"[:]" '$3 > 60 {print $1 ":" $2}'
  
  # if para caso o argumento introduzido seja para devolver uma lista com os nomes dos enfermeiros disponíveis
  elif [ $1 == "enfermeiros" ]; then
    cat enfermeiros.txt | cut -d ":" -f2,5 | grep "1" | cut -d ":" -f1 # devolve a lista de enfermeiros disponíveis
  else
    echo "Erro: Deve usar um dos argumentos disponíveis:"
    echo $0 "cidadaos <localidade>"
    echo $0 "registados"
    echo $0 "enfermeiros"
    exit 0
  fi
fi