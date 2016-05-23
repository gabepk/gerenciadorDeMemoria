/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 *  Naiara Watanabe - 090126785
 *
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 *
 */

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define NUMERO_FRAMES 10 // TODO: Colocar tudo num .h soh
#define OCUPACAO_OK 8

typedef struct tabela
{
	int pid[NUMERO_FRAMES];
	bool livre[NUMERO_FRAMES];
	char *pagina[NUMERO_FRAMES];
	int tempo_de_referencia[NUMERO_FRAMES];
} tabela;

struct tabela *ptr_tabela;
struct sembuf op[2];
int id_mem, id_sem;

void Psem()
{
	op[0].sem_num = 0;
	op[0].sem_op = 0;
	op[0].sem_flg = 0; // Semaforo blocante
	op[1].sem_num = 0;
	op[1].sem_op = 1; // Comeca com zero permissoes
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

void shutdown_substituidor () {
	exit(1);
}

void envia_pid_arquivo() {
	FILE *fp;
	long pid;

	fp = fopen("arq_pids.txt", "a+");

	if (fp != NULL)
	{
		pid = getpid();
		printf("pid: %ld\n", pid);
		fprintf(fp, "%ld,", pid);		
	}
	fclose(fp);
	
	return;
}

void obtem_estruturas_compartilhadas() {
	// Obtem memoria compartilhada
	if ((id_mem = shmget(0x89951, sizeof(int), 0x1FF)) < 0)
	{
		printf("Erro na obtencao da memoria compartilhada\n");
		exit(1);
	}
	ptr_tabela = (tabela *) shmat(id_mem, (char *)0, 0);
	if (ptr_tabela == (tabela *)-1) 
	{
		printf("Erro no attach do ponteiro para a tabela de frames\n");
		exit(1);
	}
	// Obtem semaforos
	if ((id_sem = semget(0x89950, 1, 0x1FF)) < 0)
	{
		printf("Erro na obtencao do semaforo\n");
		exit(1);
	}
}

void executa_substituicao () {
	int i, j=0, indice, tempo_maximo;
	int frames_livres = 0;

	Psem();

	while (1) {

		// Block com Psem
		printf("Bloqueado no semaforo\n");
		Psem();
		printf("Semaforo liberado pela %da vez\n", j++);

		// Conta numero de frames livres
		for (i = 0; i < NUMERO_FRAMES; i++) {
			if (ptr_tabela->livre[i]) frames_livres++;
		}

		// Armazena indice da tabela cujo tempo de referencia ah pagina eh o MAIOR
		while (frames_livres <= NUMERO_FRAMES - OCUPACAO_OK) {
			tempo_maximo = 0;
			for (i = 0; i < NUMERO_FRAMES; i++) {
				if (ptr_tabela->tempo_de_referencia[i] > tempo_maximo) {
					tempo_maximo = ptr_tabela->tempo_de_referencia[i];
					indice = i;
				}
			}
			frames_livres++;
			ptr_tabela->pid[indice] = 0;
			ptr_tabela->livre[indice] = true;
			ptr_tabela->pagina[indice] = "-";
			ptr_tabela->tempo_de_referencia[indice] = -9999;
		}

		// Unlock com Vsem
	}
}

int main () {
	signal(SIGUSR1, shutdown_substituidor);
	envia_pid_arquivo();
	obtem_estruturas_compartilhadas();
	
	executa_substituicao();

	return 0;
}
