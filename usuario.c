/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 *  Naiara Watanabe - 090126785
 *
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 *
 */

#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#define TAMANHO_LINHA 100

typedef struct mensagem
{
	long pid;
	char *pagina;
} mensagem;

void shutdown_usuario () {
	//exit(1);
	return;
}

int main (int argc, char *argv[]) {
	//signal(SIGUSR1, shutdown_usuario);
	if (argc > 1)
	{
		FILE *fp;
		fp = fopen(argv[1], "r");
		if (fp != NULL)
		{
			int i = 0;
			int fila_1, fila_2;
			mensagem msg_1, msg_2;
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

			// Obtem filas de mensagens
			if ( (fila_1 = msgget(0x126785, 0777)) < 0) // referencia_pagina
			{
				printf("Erro na obtencao da fila 1\n");
				//exit(1);
			}
			if ( (fila_2 = msgget(0x118995, 0777)) < 0) // recebe resposta
			{
				printf("Erro na obtencao da fila 2\n");
				//exit(1);
			}
			
			// Envia paginas e recebe respostas
			msg_1.pid = getpid();
			i = 0;
			while (paginas[i] != "\n") {
				msg_1.pagina = paginas[i];
				
				if ( msgsnd(fila_1, &msg_1, sizeof(msg_1), 0) < 0)
				{
					printf("Erro no envio da pagina %s do processo %ld\n", msg_1.pagina, msg_1.pid);
					//exit(1);
				}

				printf("Enviou paginas[%d] = %s\n", i, paginas[i]);
				
				// block
				// msgrcv(fila_2, msg_2, sizeof(msg_2), 0); // verificar se msg_2.pid é igual ao pid dele
				// unblock
				i++;
			}

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