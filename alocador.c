/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 *  Naiara Watanabe - 090126785
 *
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 *
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define NUMERO_USUARIOS 4 // 5 usuarios
#define NUMERO_FRAMES 10 // 5 page frames existem
#define MAX_OCUPACAO 9 // Se 9 frames existirem, ocorre substituicao
#define OCUPACAO_OK 8 // Ocupação deve ser < 8

typedef struct mensagem
{
	long pid; // TODO: nao eh necessariamente um pid 
	char pagina[10]; // pagina[30];
} mensagem;
typedef struct tabela
{
	int pid[NUMERO_FRAMES];
	bool livre[NUMERO_FRAMES];
	int pagina[NUMERO_FRAMES];
	int tempo_de_referencia[NUMERO_FRAMES];
} tabela;

struct mensagem msg_fila_1, msg_fila_2;
struct tabela *ptr_tabela;
struct sembuf op[2];
int id_sem, id_mem;
int fila_1, fila_2;

int numero_page_faults[NUMERO_USUARIOS];
int numero_page_faults_total;
int numero_exec_substituicao;

void Psem()
{
	op[0].sem_num = 0;
	op[0].sem_op = 0;
	op[0].sem_flg = 0; // Semaforo blocante
	op[1].sem_num = 0;
	op[1].sem_op = 1;
	op[1].sem_flg = 0; // Semaforo blocante
	if (semop(id_sem, op, 2) < 0)
		printf("Erro %d\n", errno);
	return;
}

void Vsem()
{
	op[0].sem_num = 0;
	op[0].sem_op = -1;
	op[0].sem_flg = 0;
	if (semop(id_sem, op, 1) < 0)
		printf("Erro %d\n", errno);
	return;
}

void cria_estruturas_compartilhadas()
{
	// Cria semaforo que sera usado para bloquear escrita na tabela invertida
	if ((id_sem = semget(0x89950, 1, IPC_CREAT|0x1FF)) < 0)
	{
		printf("Erro na criacao do semaforo\n");
		exit(1);
	}
	// Cria filas de mensagens
	if ((fila_1 = msgget(0x126785, IPC_CREAT|0x1FF)) < 0) // recebe referencia_pagina
	{
		printf("Erro na obtencao da fila 1\n");
		exit(1);
	}
	if ( (fila_2 = msgget(0x118995, IPC_CREAT|0x1FF)) < 0) // envia resposta
	{
		printf("Erro na criacao da fila 2\n");
		exit(1);
	}
	// Cria memoria compartilhada
	if ((id_mem = shmget(0x89951, sizeof(int), IPC_CREAT|0x1FF)) < 0)
	{
		printf("erro na criacao da memoria compartilhada para tabela\n");
		exit(1);
	}
}

void exclui_estruturas_compartilhadas()
{
	// Exclui semaforo
	if (semctl(id_sem, 1, IPC_RMID, NULL) < 0)
	{
		printf("Erro na exclusao do semaforo\n");
		exit(1);
	}
	// Exclui filas de mensagens
	if (msgctl(fila_1, IPC_RMID, NULL) < 0) // recebe referencia_pagina
	{
		printf("Erro na exclusao da fila 1\n");
		exit(1);
	}
	if (msgctl(fila_2, IPC_RMID, NULL) < 0) // envia resposta
	{
		printf("Erro na exclusao da fila 2\n");
		exit(1);
	}
	// Exclui memoria compartilhada
	if (shmctl(id_mem, IPC_RMID, NULL) < 0)
	{
		printf("erro na exclusao da memoria compartilhada\n");
		exit(1);
	}
}

void envia_pid_arquivo() {
	FILE *fp;
	long pid;

	fp = fopen("arq_pids.txt", "w+");

	if (fp != NULL)
	{
		pid = getpid();
		printf("pid: %ld\n", pid);
		fprintf(fp, "%ld,", pid);		
	}
	fclose(fp);
	
	return;
}

void imprime_tabela() {
	int i;
	printf("\t Pid \t Pagina \t Tempo de referecia\n");
	for (i = 0; i < NUMERO_FRAMES; i++)
	{
		if (! ptr_tabela->livre[i])
			printf("\t %d \t %d \t\t %d\n", ptr_tabela->pid[i], ptr_tabela->pagina[i], ptr_tabela->tempo_de_referencia[i]);
		else printf("\t %d \t Livre\n", ptr_tabela->pid[i]);
	}
}

void shutdown_alocador()
{
	int i;
	printf("\n\n");
	for (i = 0; i < NUMERO_USUARIOS; i++) {
		// TIRAR ISSO
		numero_page_faults[i] = 0; // ARRUMAR ISSO

		printf("\t Numero de page faults do processo %d: %d\n", i, numero_page_faults[i]);
		numero_page_faults_total += numero_page_faults[i];
	}
	printf("\t Numero de page faults total: %d\n", numero_page_faults_total);
	printf("\t Numero de execucoes do processo de substituicao: %d\n", numero_exec_substituicao);
	printf("\t Configuracao final da memoria:\n");
	
	imprime_tabela();
	exclui_estruturas_compartilhadas();
	exit(1);
}

void inicializa_tabela()
{
	int i;
	for (i = 0; i < NUMERO_FRAMES; i++) {
		ptr_tabela->pid[i] = 0;
		ptr_tabela->livre[i] = true;
		ptr_tabela->pagina[i] = 9999;
		ptr_tabela->tempo_de_referencia[i] = -9999;
	}
	return;
}

bool aloca_frame(mensagem *msg)
{
	int i, ocupacao_tabela = 0;
	bool page_fault = true;

	int pg = atoi(msg->pagina);

	// Verifica se pagina esta na tabela de frames
	for (i = 0; i < NUMERO_FRAMES; i++)
	{
		printf("minha pag: %d \t pag da tab: %d\n", pg, ptr_tabela->pagina[i]);
		//if (strcmp(msg->pagina, ptr_tabela->pagina[i]) == 0
		if (pg == ptr_tabela->pagina[i])
		{
			printf("Pagina %d ENCONTRADA no indice %d \n", pg, i);
			ptr_tabela->tempo_de_referencia[i] = 0; // Marca como usado mais recentemente
			page_fault = false;
			break;
		}
		if (!ptr_tabela->livre[i]) ocupacao_tabela++; // Conta numero de frames ocupadas
	}

	// Se nao estah, houve page fault
	if(page_fault) {
		printf("Page Fault.\n \b");
		// numero_page_faults[];
		// TODO : incrementar numero page faults de cada processo

		printf("ocupacao: %d\n", ocupacao_tabela);
		while (ocupacao_tabela >= MAX_OCUPACAO)
		{
			printf("Nao tem espaco. Libera Frame.\n");
			numero_exec_substituicao++;
			//substituicao_de_frames();
			// Psem();
			// Executa substituicao
			// Vsem();
			ocupacao_tabela--;
		}

		for (i = 0; i < NUMERO_FRAMES; i++)
		{
			// Se frame estiver livre, insere nova pagina lah
			if (ptr_tabela->livre[i])
			{
				printf("Pagina %d INSERIDA no indice %d \n", pg, i);
				ptr_tabela->pid[i] = msg->pid;
				ptr_tabela->livre[i] = false;
				ptr_tabela->pagina[i] = pg;
				ptr_tabela->tempo_de_referencia[i] = 0; // Marca como usado mais recentemente
				ocupacao_tabela++;
				break;
			}
		}
	}
	printf("\n\n");
	imprime_tabela();
	printf("\n\n");
	// Incrementa tempo de referencia de todas as paginas
	for (i = 0; i < NUMERO_FRAMES; i++)
		ptr_tabela->tempo_de_referencia[i]++;

	return page_fault;
}

void executa_alocacao() {
	numero_exec_substituicao = 0;
	inicializa_tabela();
	while (1) {
		printf("Esperando mensagens...\n");
		if ((msgrcv(fila_1, &msg_fila_1, sizeof(msg_fila_1)-sizeof(long), 0, 0)) < 0)
		{
			printf("Erro na obtencao da mensagem na fila 1\n");
			exit(1);
		}
		printf("Pid: %ld e Pag = %s \n", msg_fila_1.pid, msg_fila_1.pagina);

		msg_fila_2.pid = msg_fila_1.pid;
		strcpy(msg_fila_2.pagina, aloca_frame(&msg_fila_1) ? "Page fault" : "Page found");

		if ((msgsnd(fila_2, &msg_fila_2, sizeof(msg_fila_2)-sizeof(long), 0)) < 0)
		{
			printf("Erro no envio de mensagem na fila 2\n");
			exit(1);
		}
	}
	return;
}

int main ()
{
	signal(SIGUSR1, shutdown_alocador);
	cria_estruturas_compartilhadas();
	envia_pid_arquivo();

	ptr_tabela = (tabela *) shmat(id_mem, (char *)0, 0); // Aloca tabela na memória compartilhada
	if (ptr_tabela == (tabela *)-1) 
	{
		printf("Erro no attach do ponteiro para a tabela de frames\n");
		exit(1);
	}

	executa_alocacao();

	return 0;
}
