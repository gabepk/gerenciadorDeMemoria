/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 * 
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 * 
 */


#include "util.h"
#include <stdio.h>

void shutdown()
{
	int pid = 0, i;
	printf("Numero de page faults do processo %d: %d\n", pid, page_faults[pid]);
	printf("Numero de pade faults total: %d\n", page_fautls_total);
	printf("Numero de execucoes do processo de substituicao: %d\n", num_substituicoes);
	printf("Configuracao final da memoria:\n")
	
	printf("Pagina \t\t Tempo de referecia");
	for (i = 0; i < NUMERO_FRAMES; i++)
		printf("%s \t\t %s", pagina, tempo_de_referencia);
}
