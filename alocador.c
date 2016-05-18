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

#define MAX_OCUPACAO 9 // Se 9 frames existirem, ocorre substituicao
#define OCUPACAO_OK 8 // Ocupação deve ser < 8
#define NUMERO_FRAMES 10 // 5 page frames existem
#define NUMERO_USUARIOS 4 // 5 usuarios

typedef struct mensagem
{
	long pid; // TODO: nao eh necessariamente um pid 
	char *pagina;
};
typedef struct tabela
{
	int pid[NUMERO_FRAMES];
	bool livre[NUMERO_FRAMES];
	char *pagina[NUMERO_FRAMES];
	int tempo_de_referencia[NUMERO_FRAMES];
};
struct sembuf op[2];
int idsem;
int numero_page_faults[NUMERO_USUARIOS];
int numero_page_faults_total;
int numero_exec_substituicao;

int Psem()
{
	op[0].sem_num = 0;
	op[0].sem_op = 0;
	op[0].sem_flg = 0; // Semaforo blocante
	op[1].sem_num = 0;
	op[1].sem_op = 1;
	op[1].sem_flg = 0; // Semaforo blocante
	if (semop(idsem, op, 2) < 0)
		printf("Erro %d\n", errno);
}

int Vsem()
{
	op[0].sem_num = 0;
	op[0].sem_op = -1;
	op[0].sem_flg = 0;
	if (semop(idsem, op, 1) < 0)
		printf("Erro %d\n", errno);
}

void shutdown_alocador(struct tabela tab_invertida)
{
	printf("\n\n");
	for (int i = 0; i < NUMERO_USUARIOS; i++) {
		printf("\t Numero de page faults do processo %d: %d\n", i, numero_page_faults[i]));
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


	exit(1);

}

void aloca_frame(struct usuario msg)
{
	
	


	// tentar reservar page frame pra pag i
	/// se pág i já tá lá
		// retorna ela
	/// se não
		/// se tiver espaço na frame
			// mapear página numa page frame livre aleatória
			// sinalizar que houve page fault signal unix
			printf("\t pid : %d", msg.pid);
		    numero_page_faults[msg.pid]++;
		/// se não
			// substituição
			// kill (_, SIGUSR2);
			numero_exec_substituicao++;
	return 1;
}

int main ()
{
	struct mensagem msg_fila_1;
	struct mensagem msg_fila_2;
	struct tabela tab_invertida;
	int id_mem;
	char *ptr_tabela;
	signal(SIGUSR1, shutdown_alocador);

	// Cria semaforo que sera usado para bloquear escrita na tabela invertida
	if ((idsem = semget(0x89950, 1, IPC_CREAT|0777)) < 0)
	{
		printf("Erro na criacao do semaforo\n");
		exit(1);
	}

	// Obtem e cria filas de mensagens
	if ( (int fila_1 = msgget(0x126785, 0777)) < 0); // recebe referencia_pagina
	{
		printf("Erro na obtencao da fila 1\n");
		exit(1);
	}
	if ( (int fila_2 = msgget(0x118995, IPC_CREAT|07777)) < 0); // envia resposta
	{
		printf("Erro na criacao da fila 2\n");
		exit(1);
	}

	// Cria memoria compartilhada
	if ((id_mem = (int *) shmget(IPC_PRIVATE, sizeof(tabela), IPC_CREAT|0777)) < 0)
	{
		printf("erro na criacao da fila\n");
		exit(1);
	}
	// Aloca tabela na memória compartilhada
	ptr_tabela = (char *) shmat(idshm, (char *)0, 0);
	*ptr_tabela = tab_invertida;

	while (1) {
		if (msgrcv(fila_1, msg_fila_1, sizeof(msg_fila_1), 0) < 0)
		{
			printf("Erro na obtencao da mensagem na fila 1\n");
			exit(1);
		}

		msg_fila_2.pid = aloca_frame(msg_fila_1);
		msg_fila_2.pagina = "";

		if (msgrcv(fila_2, msg_fila_2, sizeof(msg_fila_2), 0) < 0)
		{
			printf("Erro no envio de mensagem na fila 2\n");
			exit(1);
		}
	}

	return 0;
}
