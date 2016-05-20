/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 *  Naiara Watanabe - 090126785
 *
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 *
 */

#include <stdlib.h>
#include <signal.h>

#define NUMERO_FRAMES 10 // TODO: Colocar tudo num .h soh

typedef struct tabela
{
	int pid[NUMERO_FRAMES];
	bool livre[NUMERO_FRAMES];
	char *pagina[NUMERO_FRAMES];
	int tempo_de_referencia[NUMERO_FRAMES];
} tabela;

struct tabela *ptr_tabela;

void shutdown_substituidor () {
	exit(1);
}

void substituicao () {
	int i, indice, tempo_maximo;
	int frames_livres = 0;

	while (1) {

		// Block com Psem

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

		// Unlock com Psem
	}
}

int main () {
	signal(SIGUSR1, shutdown_substituidor);
	
	if ((id_mem = semget(0x89951, 1, 0x1FF)) < 0)
	{
		printf("Erro na obtencao da memoria compartilhada\n");
		exit(1);
	}

	ptr_tabela = (tabela *) shmat(id_mem, (char *)0, 0);
	if (pshm == (tabela *)-1) 
	{
		printf("Erro no attach do ponteiro para a tabela de frames\n");
		exit(1);
	}

	substituicao();

	return 0;
}
