/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 *  Naiara Watanabe - 090126785
 *
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 *
 */

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TAMANHO_LINHA 100

typedef struct mensagem
{
	long pid;
	char *pagina;
} mensagem;

void shutdown_usuario () {
	exit(1);
	return;
}

int main (int argc, char *argv[]) {
	signal(SIGUSR1, shutdown_usuario);
	if (argc > 1)
	{
		// Pid logico que sera usado como indice na tabela de frames
		char *pid_logico;
		pid_logico = strndup(argv[1]+13, 1);

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
			if ((fila_1 = msgget(0x1BC, 0x1FF)) < 0) // referencia_pagina
			{
				printf("Erro na obtencao do id da fila 1\n");
				exit(1);
			}
			if ((fila_2 = msgget(0x118995, 0x1FF)) < 0) // recebe resposta
			{
				printf("Erro na obtencao do id da fila 2\n");
				exit(1);
			}
			
			// Envia paginas e recebe respostas
			printf("fila_1 = %d\n", fila_1);
			printf("fila_2 = %d\n", fila_2);
			msg_1.pid = getpid();
			printf("pid = %ld\n", msg_1.pid);
			i = 0;
			while (paginas[i] != "\n") { // TODO usar string compare
				msg_1.pagina = paginas[i];
			
				if ( msgsnd(fila_1, &msg_1, sizeof(msg_1)-sizeof(long), 0) < 0)
				{
					printf("Erro no envio da pagina %s do processo %ld\n", msg_1.pagina, msg_1.pid);
					exit(1);
				}
				
				printf("Enviou paginas[%d] = %s\n", i, paginas[i]);
				printf("Mensagem enviada: %ld %s\n", msg_1.pid, msg_1.pagina);
				/*if ( msgrcv(fila_2, &msg_2, sizeof(msg_2), 0, 0) < 0) 
				{
					printf("Erro no recebimento da fila 2.\n");
					exit(1);
				}*/
				//printf("mensagem recebida = %ld %s\n", msg_2.pid, msg_2.pagina);
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