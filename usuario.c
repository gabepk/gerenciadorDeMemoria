/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 * 
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 * 
 */

#include "alocador.h"
#include "shutdown.h"
#include <stdio.h>

int main ()
{
	if ((pid = fork()) < 0)
	{
		printf ("Erro ao criar processo.");
		exit(1); // Aborta
	}
	
	// Processo filho
	if (pid > 0)
	{
		referencia_pagina(0);
		referencia_pagina(1);
		referencia_pagina(2);
	}
	
	// Processo pai
	
	shutdown();
	return 0;
}
