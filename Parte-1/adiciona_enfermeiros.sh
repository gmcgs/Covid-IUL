#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº: 98704      Nome: Gonçalo Miguel Silva
## Nome do Módulo: adiciona_enfermeiros.sh
## Descrição/Explicação do Módulo: 
## |    |     |     |      |     |
## v    v     v     v      v     v 
###############################################################################

#if para verificar se o número de argumentos é válido e não é introduzido argumentos a mais
if [ $# != 4 ]; 
then  
  echo "Erro: Síntaxe: $0 <nome>:<número cédula profissional>:<centro saúde associado>:<disponibilidade>" 
  exit 0

# else if para verificar se o número de cédula contém letras
elif [[ "$2" =~ [^0-9] ]]; then
  echo "Erro: O número de cédula só pode conter números"
  exit 0

# else if para verificar se o Centro de Saúde foi corretamente introduzido
elif ! [[ "$3" =~ CS.+ ]]; then
  echo "Erro: Deve introduzir o Centro de saúde da seguinte forma: CS + localidade"
  exit 0
  
# else if para verificar se a disponibilidade é diferente de 0 ou 1
elif [[ $4 != 1 && $4 != 0 ]]; then
  echo "Erro: a disponibilidade só pode ser 1 ou 0"
  exit 0

# else if para verificar se o ficheiro enfermeiros.txt existe, criando um novo ficheiro com o enfermeiro introduzido caso não exista esse ficheiro e fazendo outras confirmações de seguida caso exista
elif [ -e enfermeiros.txt ]; then

  CentroSaude=$(grep -c "$3:" enfermeiros.txt) #variável com o número de Centros de saúde da mesma localidade que o enfermeiro introduzido
  noEnfermeiro=$(grep -c "$2:" enfermeiros.txt) #variável com o número de enfermeiros com o mesmo número de cédula que o enfermeiro introduzido
  
  #if para verificar se de facto existe ou não um enfermeiro já registado ou não no Centro de saúde introduzido
  if [ $CentroSaude == 1 ]; then
    echo "Erro: O Centro de Saúde introduzido já tem um enfermeiro registado"     
    exit 0
  #if para verificar se de facto existe ou não um enfermeiro já registado com o mesmo número de cédula que foi introduzido  
  elif [ $noEnfermeiro == 1 ]; then
    echo "Erro: Já existe um enfermeiro registado noutro Centro de Saúde com esse número"     
    exit 0
  else
    #depois de todas as confirmações de que este enfermeiro não está registado é, agora sim, registado no enfermeiros.txt
    echo "$2:$1:$3:0:$4" >> enfermeiros.txt         
    cat enfermeiros.txt          
  fi 
else
  echo "$2:$1:$3:0:$4" > enfermeiros.txt    
  cat enfermeiros.txt 
fi
  