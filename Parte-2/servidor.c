/******************************************************************************
 ** ISCTE-IUL: Trabalho prático de Sistemas Operativos
 **
 ** Aluno: Nº: 98704       Nome: Gonçalo Miguel Silva
 ** Nome do Módulo: servidor.c
 ** Descrição/Explicação do Módulo: 
 ** Código comentado a abaixo
 **    |         |          |
 **    v         v          v
 ******************************************************************************/
#include "common.h"
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <locale.h>

//variáveis e pointers globais
int count, pidfilho;
Cidadao c;
Enfermeiro *enfermeiros;
Enfermeiro enf;
Vaga vagas[NUM_VAGAS];

//Cria o ficheiro servidor.pid com o PID do servidor
int cria_pidfile(){
  FILE *pidfile = fopen(FILE_PID_SERVIDOR, "w");  //abre o ficheiro servidor.pid para escrever
  if(pidfile){
    fprintf(pidfile, "%d", getpid());  //escreve o pid do servidor no ficheiro servidor.pid
    fclose(pidfile);  //fecha o ficheiro servidor.pid
    sucesso("S1) Escrevi no ficheiro FILE_PID_SERVIDOR o PID: %d", getpid());
  } else {
    erro("S1) Não consegui registar o servidor!");
  }
}

int counter(){
  count = 0;
  Enfermeiro enf;
  FILE * file = fopen(FILE_ENFERMEIROS, "r");  //abre o ficheiro enfermeiros.dat para ler
  while (fread(&enf, sizeof(Enfermeiro), 1, file) > 0){
    count++;  //incrementa o número de enfermeiros enquanto existir enfermeiros no ficheiro enfermeiros.dat
  }
  fclose(file);  //fecha o ficheiro enfermeiro.dat
}

//Cria uma estrutura de dados dinâmica e preenche-a com todos os enfermeiros do ficheiro enfermeiros.dat
int cria_malloc(){
  FILE *file2 = fopen(FILE_ENFERMEIROS, "r");  //abre o ficheiro enfermeiros.dat para ler
  enfermeiros = malloc(count * sizeof(Enfermeiro));  //inicializa uma estrutura de dados dinâmica na memória
  if(file2){
    fread(enfermeiros, sizeof(Enfermeiro), count, file2);  //guarda as informações de todos os enfermeiros do ficheiro enfermeiros.dat na estrutura de dados dinâmica
  }else{
    erro("S2) Não consegui ler o ficheiro FILE_ENFERMEIROS!"); 
  }
  fclose(file2);  //fecha o ficheiro enfermeiros.dat
  sucesso("S2) Ficheiro FILE_ENFERMEIROS tem %d bytes, ou seja, %d enfermeiros", sizeof(Enfermeiro)*count, count);
}

//Inicia as vagas todas com o index do enfermeiro a -1
void iniciar_vagas() {
  for(int i = 0; i < NUM_VAGAS; i++) {
    vagas[i].index_enfermeiro = -1;
  }
  sucesso("S3) Iniciei a lista de %d vagas", NUM_VAGAS);
}

//handler do sinal sigchld para quando receber esse sinal
void sigchld_handler(){
  int pid = wait(NULL);  //guarda o pid do filho que enviou o sinal 
  for (int index_vaga = 0; index_vaga < NUM_VAGAS; index_vaga++) {  //percorre o número de vagas
    if(vagas[index_vaga].PID_filho == pid) {
      int index_enf = vagas[index_vaga].index_enfermeiro;  //gaurda o index do enfermeiro do servidor-filho que tem o mesmo pid que a variável "pid"
      
      //S5.5.3.1
      vagas[index_vaga].index_enfermeiro = -1;  //limpa a vaga ocupada pelo cidadão
      sucesso("S5.5.3.1) Vaga %d que era do servidor dedicado %d libertada", index_vaga, vagas[index_vaga].PID_filho);
      
      //S5.5.3.2
      enfermeiros[index_enf].disponibilidade = 1;  //volta a disponibilizar o enfermeiro
      sucesso("S5.5.3.2) Enfermeiro %d atualizado para disponível", index_enf);
      
      //S5.5.3.3
      enfermeiros[index_enf].num_vac_dadas++;  //incrementa o número de vacinas dadas
      sucesso("S5.5.3.3) Enfermeiro %d atualizado para %d vacinas dadas", index_enf, enfermeiros[index_enf].num_vac_dadas);
      
      //S5.5.3.4
      FILE *enfile = fopen(FILE_ENFERMEIROS, "r+");  //abre o ficheiro enfermeiros.dat para ler e escrever
      fseek(enfile, index_enf*sizeof(Enfermeiro), SEEK_SET);  //move o file pointer a localização exata no ficheiro para alterar a informação do enfermeiro em questão
      fread(&enf,sizeof(enf),1,enfile);  //lê a informação do enfermeiro em questão
      enf.num_vac_dadas++;  //incrementa o número de vacinas dadas para escrever no ficheiro
      fseek(enfile, sizeof(Enfermeiro)*-1, SEEK_CUR);   //move o file pointer para o enfermeiro anterior no ficheiro
      fwrite(&enf, sizeof(Enfermeiro), 1, enfile);  //escreve os novos dados do enfermeiro em questão
      fclose(enfile);  //fecha o ficheiro enfermeiros.dat
      sucesso("S5.5.3.4) Ficheiro FILE_ENFERMEIROS %d atualizado para %d vacinas dadas", index_enf, enfermeiros[index_enf].num_vac_dadas);
      sucesso("S5.5.3.5) Retorna");
      sucesso("S4) Servidor espera pedidos");
      break;
    }
  }
  
  
}

//Handler do sinal SIGTERM caso receba este sinal envia um sinal SIGTERM ao processo cidadão
void sigterm_handler(){
  sucesso("S5.6.1) SIGTERM recebido, servidor dedicado termina Cidadão");
  kill(c.PID_cidadao, SIGTERM);
  exit(1);
}

//handler do sinal sigusr1 para quando receber esse sinal
void sigusr1_handler(){
  FILE *pedido_vacina = fopen(FILE_PEDIDO_VACINA, "r");  //abre o ficheiro pedidovacina.txt para leitura
  
  if(pedido_vacina) {  //se o ficheiro abrir...
  
    if(access(FILE_PEDIDO_VACINA, R_OK ) == 0){  //se o ficheiro der para ler
      fscanf(pedido_vacina, "%d:%99[^:]:%d:%99[^:]:%9[^:]:%d:%d", &c.num_utente, c.nome, &c.idade, c.localidade, c.nr_telemovel, &c.estado_vacinacao, &c.PID_cidadao);  //Lê os dados do cidadão registado em pedidovacina.txt
      printf("Chegou o cidadão com o pedido nº %d, com nº utente %d, para ser vacinado no Centro de Saúde CS%s\n", c.PID_cidadao, c.num_utente, c.localidade);
      fclose(pedido_vacina);  //fecha o ficheiro pedidovacina.txt
      sucesso("S5.1) Dados Cidadão: %d; %s; %d; %s; %s; 0", c.num_utente, c.nome, c.idade, c.localidade, c.nr_telemovel);
    } else {
      erro("S5.1) Não foi possível ler o ficheiro FILE_PEDIDO_VACINA");
    }
  } else {
     erro("S5.1) Não foi possível abrir o ficheiro FILE_PEDIDO_VACINA");
  }
  
  //S5.2
  int enf_index = -1;  //inicializa uma variável enf_index a -1
  char CSLocalidade[100];
  sprintf(CSLocalidade,"CS%s",c.localidade);  //junta CS à localidade do cidadão
  for(int i = 0; i < count; i++){  //percorre todos os enfermeiros
  
    if(strcmp(enfermeiros[i].CS_enfermeiro, CSLocalidade) == 0){  //compara o centro de saúde do cidadão com o do enfermeiro
      if(enfermeiros[i].disponibilidade == 1){  //verifica se a disponibilidade do enfermeiro é 1(disponível)
        
        enf_index = i;  //guarda o index do enfermeiro com o mesmo centro de saúde do cidadão em questão
        sucesso("S5.2.1) Enfermeiro %d disponível para o pedido %d", i, c.PID_cidadao);
        
      }else{
        erro("S5.2.1) Enfermeiro %d indisponível para o pedido %d para o Centro de Saúde %s", i, c.PID_cidadao, enfermeiros[i].CS_enfermeiro);
        kill(c.PID_cidadao, SIGTERM);  //envia um sinal SIGTERM ao processo cidadao ativo
        return;
      }
    }
  }
  
  if(enf_index == -1){  //caso não tenha encontrado nenhum enfermeiro do centro de saúde dá erro
    erro("S5.2) Bónus: Não existem enfermeiros registados no CS%s", c.localidade);
    kill(c.PID_cidadao, SIGTERM);
    sucesso("S4) Servidor espera pedidos");
    return;
  }
  
  int index_vaga = -1;
  for(int j = 0; j<NUM_VAGAS; j++){//percorre o número de vagas para encontrar a primeira vaga disponível
    if(vagas[j].index_enfermeiro == -1){  //se a vaga estiver disponível
      index_vaga = j;
      sucesso("S5.2.2) Há vaga para vacinação para o pedido %d", c.PID_cidadao);
      vagas[index_vaga].index_enfermeiro = enf_index;  //guarda o index do enfermeiro nessa vaga
      vagas[index_vaga].cidadao = c;  //guarda os dados do cidadão nessa vaga
      enfermeiros[enf_index].disponibilidade = 0;  //muda a disponibilidade do enfermeiro para 0
      sucesso("S5.3) Vaga nº %d preenchida para o pedido %d", index_vaga, c.PID_cidadao);
      break;
    }
  }
  if(index_vaga == -1){// Caso não haja vagas disponíveis
    erro("S5.2.2) Não há vaga para vacinação para o pedido %d", c.PID_cidadao);
    kill(c.PID_cidadao, SIGTERM);  //envia um sinal SIGTERM ao processo cidadao ativo
    return;
  }
  
  //S5.4
  pidfilho = fork();  //cria um servidor filho
  if(pidfilho == 0){  //código do servidor-filho
    sucesso("S5.4) Servidor dedicado %d criado para o pedido %d", getpid(), c.PID_cidadao);
    //S5.6.1
    signal(SIGTERM, sigterm_handler);  //arma o sinal SIGTERM
    //S5.6.2
    kill(c.PID_cidadao,SIGUSR1);  //envia um sinal SIGUSR1 ao processo cidadao ativo 
    sucesso("S5.6.2) Servidor dedicado inicia consulta de vacinação");
    //S5.6.3
    sleep(TEMPO_CONSULTA);  //para o processo servidor durante o TEMPO_CONSULTA 
    sucesso("S5.6.3) Vacinação terminada para o cidadão com o pedido nº %d", c.PID_cidadao);
    //S5.6.4
    kill(c.PID_cidadao, SIGUSR2);  //envia um sinal SIGUSR2 ao processo cidadao ativo
    sucesso("S5.6.4) Servidor dedicado termina consulta de vacinação");
    exit(0);
    
  } else if(pidfilho > 0) {  //código do servidor-pai
  
    vagas[index_vaga].PID_filho = pidfilho;  //inicia o campo PID_filho da vaga em questão com o pid do servidor-filho
    sucesso("S5.5.1) Servidor dedicado %d na vaga %d", pidfilho, index_vaga);
    signal(SIGCHLD, sigchld_handler);  //trata o sinal SIGCHLD
    sucesso("S5.5.2) Servidor aguarda fim do servidor dedicado %d", pidfilho);
  } else {
    erro("S5.4) Não foi possível criar o servidor dedicado");
  }
}

//Handler do SIGINT, fecha todos os servidores filho, remove o ficheiro servidor.pid e termina o servidor pai quando recebe o sinal SIGINT
void sigint_handler(){
  for(int i = 0; i < NUM_VAGAS; i++){  //percorre todas as vagas
    if(vagas[i].PID_filho > 0)  
      kill(vagas[i].PID_filho, SIGTERM);  //envia um sinal SIGTERM ao servidor-filho da vaga caso este exista
  }
  remove(FILE_PID_SERVIDOR);  //remove o ficheiro servidor.pid
  sucesso("S6) Servidor terminado");
  exit(1);  //termina o servidor pai
}

int main() {
  setlocale(LC_ALL,"Portuguese");  //permite a presença de caracteres portugueses (ç, ´, `, etc...)
  //S1
  cria_pidfile();
  
  //conta os números de enfermeiros no ficheiro enfermeiros.dat - função aux.
  counter();
  
  //S2
  cria_malloc();
  
  //S3
  iniciar_vagas();
  
  sucesso("S4) Servidor espera pedidos");
  signal(SIGUSR1, sigusr1_handler);  //trata o sinal SIGUSR1
  pause();
  signal(SIGINT, sigint_handler);  //trata o sinal SIGINT
  
  while(1){   
    pause();  //fica em espera passiva por um sinal
  }
  return 0;
}
