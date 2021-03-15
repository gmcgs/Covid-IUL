#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº: 98704       Nome: Gonçalo Miguel Silva
## Nome do Módulo: agendamento.sh
## Descrição/Explicação do Módulo: 
##
## numEnfermeiros -> guarda o número de enfermeiros disponíveis
## data -> guarda da data atual
## cidade -> guarda a cidade associada ao enfermeiro em questão
## numCed -> guarda o número de cédula do enfermeiro em questão
## nome -> guarda o nome do enfermeiro em questão
## numCid -> guarda o número de cidadãos registados na localidade do enfermeiro em questão
## nomeCid -> guarda o nome do cidadão em questão registado na localidade guardada em $cidade
## numCid -> guarda o número de utente do cidadão em questão registado na localidade guardada em $cidade
##
## Descrição nos comentários embaixo:
##      |                 |
##      v                 v
###############################################################################

if [ -e agenda.txt ]; then
  rm agenda.txt
fi

numEnfermeiros=$(cut -d ":" -f5 enfermeiros.txt | grep -c "1")
# if para verificar se existe de facto enfermeiros disponíveis 
if [ $numEnfermeiros != 0 ]; then
  touch agenda.txt
  data=$(date -I)
  # for para percorrer todos os enfermeiros disponíveis
  for i in $(seq 1 $numEnfermeiros); do
    # variáveis com as várias informações relativas ao enfermeiro em questão (explicadas acima)
    cidade=$(cut -d ":" -f3,5 enfermeiros.txt | grep ":1" | sed -n "$i"p | cut -d ":" -f1 | sed -e "s/^CS//") 
    numCed=$(cut -d ":" -f1,3 enfermeiros.txt | awk -F"[:]" '{print $1 ":" $2 ":"}' | grep "CS$cidade:" | cut -d ":" -f1)
    nome=$(cut -d ":" -f2,3 enfermeiros.txt | awk -F"[:]" '{print $1 ":" $2 ":"}' | grep "CS$cidade:" | cut -d ":" -f1)
    numCid=$(cut -d ":" -f4 cidadaos.txt | awk '{print $1 ":"}' | grep -c "$cidade:")
    # for para percorrer todos os cidadãos da localidade guardada em $cidade
    for j in $(seq 1 $numCid); do
      # variáveis com as várias informações relativas ao cidadão em questão (explicadas acima)
      nomeCid=$(grep ":$cidade:" cidadaos.txt | cut -d ":" -f2 | sed -n "$j"p)
      numCid=$(grep ":$cidade:" cidadaos.txt | cut -d ":" -f1 | sed -n "$j"p)
      echo $nome":"$numCed":"$nomeCid":"$numCid":CS"$cidade":"$data >> agenda.txt
    done
  done
  cat agenda.txt
else
  echo "Erro: Não há enfermeiros disponíveis"
  exit 0
fi

