/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 *  Naiara Watanabe - 090126785
 *
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 *
 */

#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Criar um .h soh pra isso, pra nao ter que botar em memoria compartilhada o "NUMERO_FRAMES"
#define TAMANHO_LINHA 100
#define NUMERO_FRAMES 10 // 5 page frames existem
#define MAX_OCUPACAO 9 // Se 9 frames existirem, ocorre substituicao
#define OCUPACAO_OK 8 // Ocupação deve ser < 8

struct tabela
{
	int pid[NUMERO_FRAMES];
	bool livre[NUMERO_FRAMES];
	char *pagina[NUMERO_FRAMES];
	int tempo_de_referencia[NUMERO_FRAMES];
};

int numero_page_faults;
int numero_page_faults_total;
int numero_exec_substituicao;
struct tabela *ptr_tabela;

void imprime_resultado() {
	int i;
	printf("\n\n");
	for (i = 0; i < 1; i++) {
		printf("\t Numero de page faults do processo %d: %d\n", i, numero_page_faults);
		numero_page_faults_total += numero_page_faults;
	}
	printf("\t Numero de page faults total: %d\n", numero_page_faults_total);
	printf("\t Numero de execucoes do processo de substituicao: %d\n", numero_exec_substituicao);
	printf("\t Configuracao final da memoria:\n");

	printf("\t Pid \t Pagina \t Tempo de referecia\n");
	for (i = 0; i < NUMERO_FRAMES; i++)
	{
		if (! ptr_tabela->livre[i])
			printf("\t %d \t %s \t\t %d\n", ptr_tabela->pid[i], ptr_tabela->pagina[i], ptr_tabela->tempo_de_referencia[i]);
		else printf("\t %d \t Livre\n", ptr_tabela->pid[i]);
	}
}

void inicializa_tabela() {
	int i;
	ptr_tabela = malloc(sizeof(struct tabela));

	for (i = 0; i < NUMERO_FRAMES; i++) {
		ptr_tabela->pid[i] = 0;
		ptr_tabela->livre[i] = true;
		ptr_tabela->pagina[i] = "-";
		ptr_tabela->tempo_de_referencia[i] = -9999;
	}

}

void substituicao_de_frames() {
	int i, indice, tempo_maximo;
	int frames_livres = 0;

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

		printf("Liberou frame %d\n", indice);
		frames_livres++;
		ptr_tabela->pid[indice] = 0;
		ptr_tabela->livre[indice] = true;
		ptr_tabela->pagina[indice] = "-";
		ptr_tabela->tempo_de_referencia[indice] = -9999;
	}

}

void aloca_frames(char *paginas[], int num_paginas) {
	int i, j, ocupacao_tabela = 0;
	bool page_fault;

	// Block com Psem() - tem q ter 2 semaforos

	inicializa_tabela();

	for (i = 0; i < num_paginas; i++)
	{
		page_fault = true;

		// Verifica se pagina esta na tabela de frames
		for (j = 0; j < NUMERO_FRAMES; j++)
		{
			if (*(paginas[i]) == *(ptr_tabela->pagina[j]))
			{
				printf("Pagina %s ENCONTRADA no indice %d \n", paginas[i], j);
				ptr_tabela->tempo_de_referencia[j] = 0; // Marca como usado mais recentemente
				page_fault = false;
				break;
			}
		}

		// Se nao estah, houve page fault
		if (page_fault) {
			printf("Page Fault.\n \b");
			numero_page_faults++;

			if (ocupacao_tabela >= MAX_OCUPACAO)
			{
				printf("Nao tem espaco. Libera Frame.\n");
				numero_exec_substituicao++;
				substituicao_de_frames();
			}

			for (j = 0; j < NUMERO_FRAMES; j++)
			{
				// Se frame estiver livre, insere nova pagina lah
				if (ptr_tabela->livre[j])
				{
					printf("Pagina %s INSERIDA no indice %d \n", paginas[i], j);
					ptr_tabela->livre[j] = false;
					ptr_tabela->pagina[j] = paginas[i];
					ptr_tabela->tempo_de_referencia[j] = 0; // Marca como usado mais recentemente
					ocupacao_tabela++;
					break;
				}
			}
		}

		// Incrementa tempo de referencia de todas as paginas
		for (j = 0; j < NUMERO_FRAMES; j++) {
			ptr_tabela->tempo_de_referencia[j]++;
		}
				
	}
}

int main (int argc, char *argv[]) {
	if (argc > 1)
	{
		FILE *fp;
		fp = fopen(argv[1], "r");
		if (fp != NULL)
		{
			int i = 0;
			char linha[TAMANHO_LINHA], *token;
			char *paginas[TAMANHO_LINHA]; // mudar para dinamico

			fgets(linha, TAMANHO_LINHA, fp);
			token = strtok(linha, ",");
			while (token != NULL) {
				paginas[i] = token;
				token = strtok(NULL, ","); // ve se funciona
				i++;
			}
			paginas[i] = "\n";
			
			aloca_frames(paginas, i);
			imprime_resultado();

			fclose(fp);
		}
		else
		{
			printf("Erro na abertura do arquivo.\n");
		}
	}
	else
	{
		printf("Falta arquivo .txt de entrada.\n");
	}

	return 0;
}