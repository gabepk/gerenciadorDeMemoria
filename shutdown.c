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
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define TAMANHO_LINHA 100

int main ()
{
	// Recebe pids

	FILE *fp;
	int i = 0;
	char linha[TAMANHO_LINHA], *token;
	char *paginas[TAMANHO_LINHA]; // mudar para dinamico

	
	fp = fopen ("arq_pids.txt", "r");

	if (fp != NULL)
	{
		fgets(linha, TAMANHO_LINHA, fp);
		token = strtok(linha, ",");
		while (token != NULL) {
			paginas[i] = token;
			token = strtok(NULL, ","); // ve se funciona
			int pid_int = atoi(paginas[i]);
			kill(pid_int, SIGUSR1);
			i++;
		}
		paginas[i] = "\n";
	}
	fclose(fp);
	return 0;
}
