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
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>

/// TDDO : diminuir tamanho das variaveis

int id_page_faults_total, *page_faults_total;
int id_num_substituicoes, *num_substituicoes;
int id_pagina, *(pagina[NUMERO_FRAMES]);
int id_tempo_de_referencia, *(tempo_de_referencia[NUMERO_FRAMES]);
int id_page_faults, *(page_faults[NUMERO_PROCESSOS]);

void init_variaveis_compartilhadas()
{
	id_page_faults_total = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0x1FF);
	id_num_substituicoes = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0x1FF);
	id_pagina = shmget(IPC_PRIVATE, sizeof(int)*NUMERO_FRAMES, IPC_CREAT | 0x1FF);
	id_tempo_de_referencia = shmget(IPC_PRIVATE, sizeof(int)*NUMERO_FRAMES, IPC_CREAT | 0x1FF);
	id_page_faults = shmget(IPC_PRIVATE, sizeof(int)*NUMERO_PROCESSOS, IPC_CREAT | 0x1FF);
}

void attach_variaveis_compartilhadas()
{
	page_faults_total = (int *) shmat(id_page_faults_total, (char *) 0, 0);
	num_substituicoes = (int *) shmat(id_num_substituicoes, (char *) 0, 0);
	pagina = (int **) shmat(id_pagina, (char *) 0, 0);
	tempo_de_referencia = (int **) shmat(id_tempo_de_referencia, (char *) 0, 0);
	page_faults = (int **) shmat(id_page_faults, (char *) 0, 0);
}

void deleta_variaveis_compartilhadas()
{
	shmdt(id_page_faults);
	shmdt(id_tempo_de_referencia);
	shmdt(id_pagina);
	shmdt(id_num_substituicoes);
	shmdt(id_page_faults_total);

	shmctl(id_page_faults, IPC_RMID, NULL);
	shmctl(id_tempo_de_referencia, IPC_RMID, NULL);
	shmctl(id_pagina, IPC_RMID, NULL);
	shmctl(id_num_substituicoes, IPC_RMID, NULL);
	shmctl(id_page_faults_total, IPC_RMID, NULL);
}

int main ()
{
	int estado, i;
	pid_t pid;
	printf("pid pai = %d\n", getpid());

	init_variaveis_compartilhadas();
	attach_variaveis_compartilhadas();

	// Criacao de processos
	for (i = 0; i < NUMERO_PROCESSOS; i++)
	{
		if ((pid = fork()) < 0)
		{
			printf ("Erro ao criar processo.\n");
			_exit(1); // Aborta
		}

		// Processo filho
		if (pid == 0)
		{
			printf("pid filho = %d \t seu pai = %d\n", getpid(), getppid());
			attach_variaveis_compartilhadas();
			referencia_pagina(0);
			referencia_pagina(1);
			referencia_pagina(2);
			_exit(2383);
		}
	}

	// Processo pai
	while ((pid = wait(&estado)) > 0) // Precisa esperar todos os filhos
		printf("\tEstado exit de %d foi %d %s\n", (int)pid, estado, estado == 139 ? "segmetation fault" : "");

	deleta_variaveis_compartilhadas();
	shutdown();
	_exit(0);

	return 0;
}
