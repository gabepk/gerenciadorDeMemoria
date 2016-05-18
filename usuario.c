#include <stdio.h>
#include <signal.h>
#include <string.h>

#define TAMANHO_LINHA 100

typedef struct mensagem
{
	long pid;
	char *pagina;
};

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
			struct mensagem msg_fila_1;
			struct mensagem msg_fila_2;
			char linha[TAMANHO_LINHA], *token;
			char *paginas[TAMANHO_LINHA]; // mudar para dinamico

			fgets(linha, TAMANHO_LINHA, fp);
			token = strtok(linha, ",");
			while (token != NULL) {
				paginas[i] = token;
				token = strtok(NULL, ","); // ve se funciona
				//printf("paginas[%d] = %s\n", i, paginas[i]);
				i++;
			}
			paginas[i] = "\n";

			// Cria e obtem filas de mensagens
			if ( (int fila_1 = msgget(0x126785, IPC_CREAT|0777)) < 0); // referencia_pagina
			{
				printf("Erro na criação da fila 1\n");
				exit(1);
			}
			if ( (int fila_2 = msgget(0x118995, 0777)) < 0); // recebe resposta
			{
				printf("Erro na obtencao da fila 2\n");
				exit(1);
			}
			
			// Envia paginas e recebe respostas
			msg.pid = getpid();
			i = 0;
			while (paginas[i] != "\n") {
				msg.pagina = paginas[i];
				
				if ( msgsnd(fila_1, &msg_fila_1, sizeof(msg_fila_1), 0) < 0)
				{
					printf("Erro no envio da pagina %s do processo %d\n", msg.pagina, msg.pid);
					exit(1);
				}
				// block
				// msgrcv(fila_2, msg_fila_2, sizeof(msg_fila_2), 0); // verificar se msg.pid é igual ao pid dele
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