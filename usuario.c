/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 * 
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 * 
 */

#include "util.h"
#include "alocador.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

void referencia_pagina(int i) 
{
	printf("\t %d\n", i);
}

void shutdown() {}

int main ()
{
	int estado, i;
	pid_t pid;
	printf("pid pai = %d\n", getpid());
	
	// Inicializando variaveis compartilhadas
	num_substituicoes = 0;
	page_fautls_total = 0;
	pagina = "";
	tempo_de_referencia = "";
	for (i = 0; i < NUMERO_PROCESSOS; i++)
		page_faults[i] = 0;
	
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
			printf("[%d] pid filho = %d \t seu pai = %d\n", i, getpid(), getppid());
			referencia_pagina(0);
			referencia_pagina(1);
			referencia_pagina(2);
			_exit(0);
		}
	}
	
	// Processo pai
	while ((pid = wait(&estado)) > 0) // Precisa esperar todos os filhos
		printf("Estado exit de %d foi %d\n", (int)pid, estado);
		
	shutdown();
	_exit(0);
	
	return 0;
}
