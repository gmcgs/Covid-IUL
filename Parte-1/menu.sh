#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº: 98704      Nome: Gonçalo Miguel Silva
## Nome do Módulo: menu.sh
## Descrição/Explicação do Módulo: 
##
## opcao -> guarda a opção escolhida no menu
## opcaoaux -> guarda a opção escolhida no sub-menu do stats.sh
## 
###############################################################################

# menu
clear
figlet -f slant menu covid-iul
echo " "
echo "1. Listar cidadãos"
echo " "
echo "2. Adicionar enfermeiro"
echo " "
echo "3. Stats"
echo " "
echo "4. Agendar vacinação"
echo " "
echo "0. Sair"
echo " "

read -p "Insira uma opção: " opcao

# while para continuar a mostrar o menu (iterativamente) até o utilizador decidir sair (opção = 5)
while [[ $opcao != 0 ]]; do
  # case para separar todas as opções do menu
  case $opcao in
  
    1)
      # execução do script lista_cidadaos.sh para criar o ficheiro cidadaos.txt com todos os cidadãos registados na listagem.txt 
      echo " "
      ./lista_cidadaos.sh
      echo " "
      echo -e "Pressione ENTER para voltar ao menu \c"
      read enter
      ;;
      
    2)
      # recolha dos argumentos necessários e execução do script adiciona_enfermeiros.sh para adicionar um enfermeiro ao ficheiro enfermeiros.txt
      echo " "
      read -p "Insira o nome do enfermeiro: " nome
      echo " "
      read -p "Insira o número do enfermeiro: " num
      echo " "
      read -p "Insira o Centro de Saúde do enfermeiro: " CS
      echo " "
      read -p "Insira a disponibilidade do enfermeiro: " disp
      echo " "
      ./adiciona_enfermeiros.sh "$nome" $num "$CS" $disp
      echo " "
      echo -e "Pressione ENTER para voltar ao menu \c"
      read enter
      ;;
      
    3)
      # sub-menu para o script stats.sh
      echo " "
      echo "1. Nº de cidadãos registados de uma localidade"
      echo " "
      echo "2. Lista de cidadãos registados com mais de 60 anos"
      echo " "
      echo "3. Lista com o nome dos enfermeiros assinalados como disponíveis na plataforma"
      echo " "
      read -p "Insira o que pretende procurar: " opcaoaux
      
      # case relativo ao sub-menu do stats.sh para escolha dos argumentos
      case $opcaoaux in 
        1)
          # execução do script stats.sh com o argumento cidadaos [localidade] para devolver o nº de cidadãos dessa localidade
          echo " "
          echo "Insira a localidade onde pretende procurar"
          read localidade
          echo " "
          ./stats.sh "cidadaos" $localidade
          echo " "
          echo -e "Pressione ENTER para voltar ao menu \c"
          read enter
          ;;
        2)
          # execução do script stats.sh com o argumento registados para devolver a lista de cidadãos com mais de 60 anos registados no sistema
          echo " "
          ./stats.sh "registados"
          echo " "
          echo -e "Pressione ENTER para voltar ao menu \c"
          read enter
          ;;  
        3)
          # execução do script stats.sh com o argumento enfermeiros para devolver a lista de enfermeiros disponíveis no sistema
          echo " " 
          ./stats.sh "enfermeiros"
          echo " "
          echo -e "Pressione ENTER para voltar ao menu \c"
          read enter
          ;;
      esac
      ;;
      
    4)
      # execução do script agendamento.sh para devolver o ficheiro agenda.txt com os enfermeiros associados aos devidos cidadãos e as suas datas de vacinação 
      echo " "
      ./agendamento.sh
      echo " "
      echo -e "Pressione ENTER para voltar ao menu \c"
      read enter
      ;;
  esac
  
  # menu
  clear
  figlet -f slant menu covid-iul
  echo " "
  echo "1. Listar cidadãos"
  echo " "
  echo "2. Adicionar enfermeiro"
  echo " "
  echo "3. Stats"
  echo " "
  echo "4. Agendar vacinação"
  echo " "
  echo "0. Sair"
  echo " "

  read -p "Insira uma opção: " opcao
  
done
clear