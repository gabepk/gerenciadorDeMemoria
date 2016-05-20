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
	char *pagina[NUMERO_FRAMES];
	int tempo_de_referencia[NUMERO_FRAMES];
} tabela;

struct sembuf op[2];
int id_sem;
int id_mem;
int fila_1, fila_2, fila_dummy;

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
	if ((fila_1 = msgget(0x1BC, IPC_CREAT|0x1FF)) < 0) // recebe referencia_pagina
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
	if ((id_mem = shmget(0x89951, sizeof(tabela), IPC_CREAT|0x1FF)) < 0)
	{
		printf("erro na criacao da memoria compartilhada\n");
		exit(1);
	}

	printf("id_sem = %d\n", id_sem);	
	printf("fila_1 = %d\n", fila_1);
	printf("fila_2 = %d\n", fila_2);
	printf("id_mem = %d\n", id_mem);
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

void shutdown_alocador()
{
	int i;
	printf("\n\n");
	for (i = 0; i < NUMERO_USUARIOS; i++) {
		printf("\t Numero de page faults do processo %d: %d\n", i, numero_page_faults[i]);
		numero_page_faults_total += numero_page_faults[i];
	}
	printf("\t Numero de page faults total: %d\n", numero_page_faults_total);
	printf("\t Numero de execucoes do processo de substituicao: %d\n", numero_exec_substituicao);
	printf("\t Configuracao final da memoria:\n");

	printf("Pid \t Pagina \t Tempo de referecia");
	for (i = 0; i < NUMERO_FRAMES; i++)
	{
		// Usar tab_invertida.livre

		/*if (tab_invertida.tempo_de_referencia < 1) // Uma pagina eh referenciada no minimo uma vez
			printf("\t %d \t %d \t %d", tab_invertida.pid, tab_invertida.pagina, tab_invertida.tempo_de_referencia);
		else printf("\t %d \t Livre", tab_invertida.pid,);*/
	}

	exclui_estruturas_compartilhadas();
	exit(1);
}

bool aloca_frame(mensagem *msg)
{
	// tentar reservar page frame pra pag i
	/// se pág i já tá lá
		// retorna ela
	/// se não
		/// se tiver espaço na frame
			// mapear página numa page frame livre aleatória
			// sinalizar que houve page fault signal unix
		    // numero_page_faults[msg->pid]++;
		/// se não
			// substituição
			// kill (_, SIGUSR2);
			numero_exec_substituicao++;
	return true;
}

int main ()
{
	struct mensagem msg_fila_1;
	struct mensagem msg_fila_2;
	tabela *ptr_tabela;

	signal(SIGUSR1, shutdown_alocador);

	cria_estruturas_compartilhadas();

	// Aloca tabela na memória compartilhada
	ptr_tabela = (tabela *) shmat(id_mem, (char *)0, 0);
	//ptr_tabela = &tab_invertida;
	printf("Cheguei aqui.\n");
	while (1) {
		printf("1.\n");
		if ((msgrcv(fila_1, &msg_fila_1, sizeof(msg_fila_1)-sizeof(long), 0, 0)) < 0)
		{
			printf("Erro na obtencao da mensagem na fila 1\n");
			exit(1);
		}

		//printf("Mensagem recebida: %ld %s \n", msg_fila_1.pid, msg_fila_1.pagina);
		if (!(&msg_fila_1)) {
			printf("Deu ruim\n");
		}
		printf("Deu bom\n");
		printf("Pid: %ld e Pag = %s \n", msg_fila_1.pid, msg_fila_1.pagina);

		msg_fila_2.pid = msg_fila_1.pid;
		if(aloca_frame(&msg_fila_1)) {
			msg_fila_2.pagina[0] = 's';
		}
		else {
			msg_fila_2.pagina[0] = 'n';
		}

		if ((msgsnd(fila_2, &msg_fila_2, sizeof(msg_fila_2)-sizeof(long), 0)) < 0)
		{
			printf("Erro no envio de mensagem na fila 2\n");
			exit(1);
		}
	}

	return 0;
}