/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 *  Naiara Watanabe - 090126785
 *
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 *
 */

#include "utils.h"
#include "usuario.h"

void shutdown_usuario () {
	printf("\n\n");
	// Numero de page faults soma + 1 na impressao, pois e inicializado com -1
	printf("\t Numero de page faults do processo %d: %d\n", pid_logico, numero_page_faults[pid_logico + 2] + 1);
	exit(1);
}

void obtem_estruturas_compartilhadas() {
	// Obtem filas de mensagens
	if ((fila_1 = msgget(0x126785, 0x1FF)) < 0) // referencia_pagina
	{
		printf("Erro na obtencao do id da fila 1\n");
		exit(1);
	}
	if ((fila_2 = msgget(0x118995, 0x1FF)) < 0) // recebe resposta
	{
		printf("Erro na obtencao do id da fila 2\n");
		exit(1);
	}
	return;
}

int main (int argc, char *argv[]) {
	int i = 0;
	signal(SIGUSR1, shutdown_usuario);



	
	if (argc > 1)
	{
		// Pid logico que sera usado como indice na tabela de frames
		pid_logico = atoi(strndup(argv[1]+13, 1));
		
		// Escreve pid em vetor global
		vetor_pids[pid_logico + 2] = getpid();



		printf("pid logico = %d\n\n", pid_logico);
		// TESTE VETOR DE PID
		for (i = 0; i < NUMERO_USUARIOS + 2; i++) {
			printf("pid %d = %d\n", i, vetor_pids[i]);
		}



		FILE *fp;
		fp = fopen(argv[1], "r");
		if (fp != NULL)
		{			
			mensagem msg_fila_1, msg_fila_2;
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

			obtem_estruturas_compartilhadas();

			// Envia paginas e recebe respostas
			msg_fila_1.pid = getpid();
			printf("pid = %ld\n", msg_fila_1.pid);
			i = 0;
			while (paginas[i] != "\n") { // TODO usar string compare
				int j = 0;
				while(paginas[j] != "\n") {
					msg_fila_1.pagina[j] = paginas[i][j];
					j++;
				}
			
				if ( msgsnd(fila_1, &msg_fila_1, sizeof(msg_fila_1)-sizeof(long), 0) < 0)
				{
					printf("Erro no envio da pagina %s do processo %ld\n", msg_fila_1.pagina, msg_fila_1.pid);
					exit(1);
				}
				printf("Enviado: %s\n", msg_fila_1.pagina);

				sleep(2);
			
				if ( msgrcv(fila_2, &msg_fila_2, sizeof(msg_fila_2)-sizeof(long), 0, 0) < 0)
				{
					printf("Erro na obtencao da mensagem na fila 2\n");
					exit(1);
				}
				printf("Recebido = %s\n", msg_fila_2.pagina);

				if (strstr(msg_fila_2.pagina, "fault") != NULL) {
					numero_page_faults[pid_logico + 2]++;
					printf("numero_page_faults = %d\n", numero_page_faults[pid_logico + 2]);
				}

				i++;
			}

			fclose(fp);
			shutdown_usuario();
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
