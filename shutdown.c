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
#include<sys/ipc.h>
#include<sys/msg.h>

#define TAMANHO_LINHA 100

int main ()
{

	int fila_3, i;
	long msg_fila_3[7];	

	if ( (fila_3 = msgget(0x118785, 0x1FF)) < 0) // obtem a fila 3
	{
		printf("Erro na criacao da fila 3\n");
		exit(1);
	}

	if ((msgrcv(fila_3, &msg_fila_3, sizeof(msg_fila_3)-sizeof(long), 0, 0)) < 0)
	{
		printf("Erro na obtencao da mensagem na fila 3\n");
		exit(1);
	}
	for(i=0;i<7;i++)
	{
		printf("msg_fila_3[%d]: %ld\n", i, msg_fila_3[i]);
		//kill(msg_fila_3[i], SIGUSR1);
	}

	if (msgctl(fila_3, IPC_RMID, NULL) < 0) // exclui fila 3 de pids
	{
		printf("Erro na exclusao da fila 3\n");
		exit(1);
	}

	// Recebe pids
/*
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
	fclose(fp);*/
	return 0;
}
