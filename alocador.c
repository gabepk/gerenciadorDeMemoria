/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 *  Naiara Watanabe - _________
 *
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 *
 */

#include <sys/types.h>
 #include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_OCUPACAO 9 // Se 9 frames existirem, ocorre substituicao
#define OCUPACAO_OK 8 // Ocupação deve ser < 8
#define NUMERO_FRAMES 10 // 5 page frames existem
#define NUMERO_USUARIOS 4 // 5 usuarios

struct mensagem
{
	long pid;
	char pagina[1];
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

void shutdown_alocador()
{
	printf("\n\n");
	for (i = 0; i < NUMERO_USUARIOS; i++) {
		printf("\t Numero de page faults do processo %d: %d\n", i, numero_page_faults[i]));
		numero_page_faults_total += numero_page_faults[i];
	}
	printf("\t Numero de page faults total: %d\n", numero_page_faults_total);
	printf("\t Numero de execucoes do processo de substituicao: %d\n", numero_exec_substituicao);
	printf("\t Configuracao final da memoria:\n");

	/*printf("Pagina \t\t Tempo de referecia");
	for (i = 0; i < NUMERO_FRAMES; i++)
	{
		if (tempo_de_referencia < 1) // Uma pagina eh referenciada no minimo uma vez
			printf("\t %d \t\t %d", *pagina, *tempo_de_referencia);
		else printf("\t Livre");
	}
*/

	exit(1);

}

void aloca_frame(struct usuario user)
{
	
	// tentar reservar page frame pra pag i
	/// se pág i já tá lá
		// retorna ela
	/// se não
		/// se tiver espaço na frame
			// mapear página numa page frame livre aleatória
			// sinalizar que houve page fault signal unix
			printf("\t pid : %d", uer.pid);
		    *(page_faults[user.pid]) = *(page_faults[user.pid]) + 1;
			(*page_fautls_total)++;
		/// se não
			// substituição
			// kill (_, SIGUSR2);
			numero_exec_substituicao++;
	return;
}

int main ()
{
	signal(SIGUSR1, shutdown_alocador);

	// Cria semaforo que sera usado para bloquear escrita na tabela invertida
	if ((idsem = semget(0x89950, 1, IPC_CREAT|0777)) < 0)
	{
		printf("Erro na criacao do semaforo\n");
		exit(1);
	}

	int fila_1 = msgget(0x126785, 0777); // recebe referencia_pagina
	int fila_2 = msgget(0x118995, IPC_CREAT|0777); // envia resposta

	// user = recebe mensagem fila 1
	// processa(user)
	// envia mensagem fila 2
	return 0;
}
