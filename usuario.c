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
	exit(1);
}

void obtem_estruturas_compartilhadas() {
	// Obtem numeros_resultado
	if ((id_num = shmget(0x678500, sizeof(int), 0x1FF)) < 0)
	{
		printf("erro na criacao da memoria compartilhada para struct numeros_resutlado \n");
		exit(1);
	}
	ptr_result = (numeros_resultado *) shmat(id_num, (char *)0, 0);
	if (ptr_result == (numeros_resultado *)-1) 
	{
		printf("Erro no attach do ponteiro para a struct numeros_resutlado\n");
		exit(1);
	}
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
	if ( (fila_pids = msgget(0x118785, 0x1FF)) < 0)
	{
		printf("Erro na obtenca da fila de pids\n");
		exit(1);
	}
	return;
}

void envia_pid_shutdown()
{
	//Envia pid para o shutdown
	msg_fila_pids.pid = getpid();
	printf("pid = %ld\n", msg_fila_pids.pid);
	if ((msgsnd(fila_pids, &msg_fila_pids, sizeof(msg_fila_pids)-sizeof(long), 0)) < 0)
	{
		printf("Erro no envio de mensagem na fila 3\n");
		exit(1);
	}
}

int main (int argc, char *argv[]) {
	signal(SIGUSR1, shutdown_usuario);
	
	if (argc > 1)
	{
		// Pid logico que sera usado como indice na tabela de frames
		pid_logico = atoi(strndup(argv[1]+13, 1));

		FILE *fp;
		fp = fopen(argv[1], "r");
		if (fp != NULL)
		{
			int i = 0;
			struct mensagem msg_fila_1, msg_fila_2;
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
			envia_pid_shutdown();

			// Envia paginas e recebe respostas
			msg_fila_1.pid = getpid();
			printf("pid = %ld\n", msg_fila_1.pid);
			i = 0;
			while (paginas[i] != "\n") { // TODO usar string compare
				int j = 0;
				while(paginas[j] != "\n") { // TODO deve existir uma funcao que faz isso
					msg_fila_1.pagina[j] = paginas[i][j];
					j++;
				}
				
				if ( msgsnd(fila_1, &msg_fila_1, sizeof(msg_fila_1)-sizeof(long), 0) < 0)
				{
					printf("Erro no envio da pagina %s do processo %ld\n", msg_fila_1.pagina, msg_fila_1.pid);
					exit(1);
				}
				printf("Enviado: %s\n", msg_fila_1.pagina);

				sleep(3);
				
				if ( msgrcv(fila_2, &msg_fila_2, sizeof(msg_fila_2)-sizeof(long), 0, 0) < 0)
				{
					printf("Erro na obtencao da mensagem na fila 2\n");
					exit(1);
				}
				printf("Recebido = %s\n", msg_fila_2.pagina);

				if (strstr(msg_fila_2.pagina, "fault") != NULL) {
					ptr_result->numero_page_faults[pid_logico]++;
					printf("numero_page_faults = %d\n", ptr_result->numero_page_faults[pid_logico]);
				}

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
