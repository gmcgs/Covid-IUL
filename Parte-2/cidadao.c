/******************************************************************************
 ** ISCTE-IUL: Trabalho prático de Sistemas Operativos
 **
 ** Aluno: Nº: 98704      Nome: Gonçalo Miguel Silva
 ** Nome do Módulo: cidadao.c
 ** Descrição/Explicação do Módulo: 
 ** Código comentado a abaixo
 **    |         |          |
 **    v         v          v
 ******************************************************************************/
#include "common.h"
#include "utils.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <locale.h>

void handler_sigint(){
  sucesso("C5) O cidadão cancelou a vacinação, o pedido nº %d foi cancelado", getpid());
  remove(FILE_PEDIDO_VACINA);  //remove o ficheiro pedidovacina.txt
  exit(0);  //fecha o processo cidadão 
}

void handler_sigusr1(){
  sucesso("C7) Vacinação do cidadão com o pedido nº %d em curso", getpid());
  remove(FILE_PEDIDO_VACINA);  //remove o ficheiro pedidovacina.txt
}

void handler_sigusr2(){
  sucesso("C8) Vacinação do cidadão com o pedido nº %d concluída", getpid());
  exit(0);  //fecha o processo cidadão 
}

void handler_sigterm(){
  sucesso("C9) Não é possível vacinar o cidadão no pedido nº %d", getpid());
  remove(FILE_PEDIDO_VACINA);  //remove o ficheiro pedidovacina.txt
  exit(0);  //fecha o processo cidadão
}

void handler_sigalarm(){
    if(access(FILE_PEDIDO_VACINA, F_OK) == 0){
        erro("C3) Não é possível iniciar o processo de vacinação neste momento");
    }else{
        sucesso("C3) Ficheiro FILE_PEDIDO_VACINA pode ser criado");
    }
}

Cidadao criar_cidadao() {
  Cidadao c;  //cria um cidadão
  printf("Insira o seu número de utente:\n");   //
  scanf("%d", &c.num_utente);                   //
  printf("Insira o seu nome:\n");               //
  my_fgets(c.nome, 100, stdin);                 //
  printf("Insira a sua idade:\n");              //  Recolhe os dados do cidadão e guarda nas variáveis correspondentes
  scanf("%d", &c.idade);                        //
  printf("Insira a sua localidade:\n");         //
  my_fgets(c.localidade, 100, stdin);           //
  printf("Insira o seu nº de telemóvel:\n");    //
  my_fgets(c.nr_telemovel, 10, stdin);          //
  c.estado_vacinacao = 0;                       //
  sucesso("C1) Dados Cidadão: %d; %s; %d; %s; %s; 0", c.num_utente, c.nome, c.idade, c.localidade, c.nr_telemovel);
  c.PID_cidadao = getpid();  //guarda o pid do processo cidadão
  sucesso("C2) PID Cidadão: %d", c.PID_cidadao);
  return c;
}

int verificar_pedidovacina(Cidadao c){
  FILE *file;
  if (file = fopen(FILE_PEDIDO_VACINA, "r")) { //Verifica se o ficheiro pedidovacina.txt existe
    //caso exista
    fclose(file);  //fecha o ficheiro pedidovacina.txt
    erro("C3) Não é possível iniciar o processo de vacinação neste momento");
    signal(SIGALRM, handler_sigalarm);  //arma o sinal SIGALARM
    while(access(FILE_PEDIDO_VACINA, F_OK) == 0){  //enquanto o ficheiro pedidovacina.txt existir executa o ciclo
      alarm(5);  //espera 5 segundos e envia um sinal SIGALARM
      pause();
    }
  } else {
    //caso não exista
    FILE *pedido;
    pedido = fopen(FILE_PEDIDO_VACINA, "w");  //preparação do ficheiro pedidovacina.txt para escrever os dados do cidadão
    if(pedido) {
      sucesso("C3) Ficheiro FILE_PEDIDO_VACINA pode ser criado");
      fprintf(pedido, "%d:%s:%d:%s:%s:%d:%d", c.num_utente, c.nome, c.idade, c.localidade, c.nr_telemovel, c.estado_vacinacao, c.PID_cidadao);  //escreve os dados do cidadão no ficheiro pedidovacina.txt
      fclose(pedido);  //fecha o ficheiro pedido pedidovacina.txt
      sucesso("C4) Ficheiro FILE_PEDIDO_VACINA criado e preenchido");
    } else {
      //caso ocorra um erro ao criar o ficheiro
      erro("C4) Não é possível criar o ficheiro FILE_PEDIDO_VACINA");
      return -1;
    }
  }
}

int main(){
  setlocale(LC_ALL,"Portuguese");  //permite a presença de caracteres portugueses (ç, ´, `, etc...)
  //signals
  signal(SIGUSR1, handler_sigusr1);  //arma o sinal SSIGUSR1
  signal(SIGUSR2, handler_sigusr2);  //arma o sinal SIGUSR2
  signal(SIGTERM, handler_sigterm);  //arma o sinal SIGTERM
  
  //C1-C2
  Cidadao c = criar_cidadao();  //Cria um elemento cidadão com os dados introduzidos pelo utilizador
  
  //C3-C4
  verificar_pedidovacina(c);  //verfica se existe o ficheiro pedidovacina.txt e escreve os dados do cidadão
  
  //C5
  signal(SIGINT, handler_sigint);  //arma o sinal SIGINT
  
  //C6
  FILE *pidfile;
  int pid;
  pidfile = fopen(FILE_PID_SERVIDOR, "r");  //prepara o ficheiro servidor.pid para ler
  if(pidfile){
    fscanf(pidfile, "%d", &pid);  //lê o pid do servidor e guarda na variável pid
    fclose(pidfile);
    kill(pid, SIGUSR1);  //envia um sinal SIGUSR1 ao servidor
    sucesso("C6) Sinal enviado ao Servidor: %d", pid);
  } else {
    erro("C6) Não existe ficheiro FILE_PID_SERVIDOR!");
  }
  
  pause();  //espera por um sinal do servidor
  pause();  //espera por um sinal do servidor
  
}














