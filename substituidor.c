/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 *  Naiara Watanabe - 090126785
 *
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 *
 */

#include "utils.h"
#include "substituidor.h"

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
	return;

}

void executa_substituicao () {
	int i, j=0, k, indice, tempo_maximo;
	int frames_livres = 0;

	while (1) {

		// Conta numero de frames livres
		for (i = 0; i < NUMERO_FRAMES; i++) {
			if (ptr_tabela->livre[i]) frames_livres++;
		}

		// Armazena indice da tabela cujo tempo de referencia ah pagina eh o MAIOR
		while (frames_livres < (NUMERO_FRAMES - OCUPACAO_OK)) {

			printf("frames livres: %d\n", frames_livres);

			// Block com Psem
			printf("Bloqueado no semaforo\n");
			Psem();
			printf("Semaforo liberado pela %da vez\n", j++);

			tempo_maximo = 0;
			for (i = 0; i < NUMERO_FRAMES; i++) {
				if (ptr_tabela->tempo_de_referencia[i] > tempo_maximo) {
					tempo_maximo = ptr_tabela->tempo_de_referencia[i];
					indice = i;
				}

			}
			printf("indice = %d\n", indice);
			frames_livres++;
			ptr_tabela->pid[indice] = 0;
			ptr_tabela->livre[indice] = true;
			ptr_tabela->pagina[indice] = 9999;
			ptr_tabela->tempo_de_referencia[indice] = -9999;

			// Unlock com Vsem
			Vsem();
			sleep(1);
		}
		frames_livres = 0;
	}
}

int main () {
	signal(SIGUSR1, shutdown_substituidor);
	int i;

	// Escreve pid em vetor global
	vetor_pids[1] = getpid();

	// TESTE VETOR DE PID
	for (i = 0; i < NUMERO_USUARIOS + 2; i++) {
		printf("pid %d = %d\n", i, vetor_pids[i]);
	}


	obtem_estruturas_compartilhadas();
	executa_substituicao();

	return 0;
}
