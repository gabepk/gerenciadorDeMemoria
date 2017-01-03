/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 *  Naiara Watanabe - 090126785
 *
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 *
 */

#include "../include/utils.h"
#include "../include/alocador.h"

void Psem()
{
	op[0].sem_num = 0;
	op[0].sem_op = 0;
	op[0].sem_flg = 0; // Semaforo blocante
	op[1].sem_num = 0;
	op[1].sem_op = 1; // Comeca com zero permissoes
	op[1].sem_flg = 0; // Semaforo blocante
	if (semop(id_sem, op, 2) < 0)
		printf("Erro %d\n", errno);
	return;
}

void Vsem()
{
	op[0].sem_num = 0;
	op[0].sem_op = -1;
	op[0].sem_flg = 0;
	if (semop(id_sem, op, 1) < 0)
		printf("Erro %d\n", errno);
	return;
}

void cria_estruturas_compartilhadas()
{
	// Cria semaforo que sera usado para bloquear escrita na tabela invertida
	if ((id_sem = semget(0x89950, 1, IPC_CREAT|0x1FF)) < 0)
	{
		printf("Erro na criacao do semaforo\n");
		exit(1);
	}
	// Cria filas de mensagens
	if ((fila_1 = msgget(0x126785, IPC_CREAT|0x1FF)) < 0) // recebe referencia_pagina
	{
		printf("Erro na obtencao da fila 1\n");
		exit(1);
	}
	if ( (fila_2 = msgget(0x118995, IPC_CREAT|0x1FF)) < 0) // envia resposta
	{
		printf("Erro na criacao da fila 2\n");
		exit(1);
	}
	if ( (fila_pids = msgget(0x118785, IPC_CREAT|0x1FF)) < 0) // envia pids para shutdown
	{
		printf("Erro na criacao da fila de pids\n");
		exit(1);

	}
	// Cria memoria compartilhada da tabela
	if ((id_tab = shmget(0x89951, sizeof(int), IPC_CREAT|0x1FF)) < 0)
	{
		printf("erro na criacao da memoria compartilhada para tabela\n");
		exit(1);
	}
	// Aloca tabela na memória compartilhada
	ptr_tabela = (tabela *) shmat(id_tab, (char *)0, 0);
	if (ptr_tabela == (tabela *)-1) 
	{
		printf("Erro no attach do ponteiro para a tabela de frames\n");
		exit(1);
	}
	//  Cria memoria compartilhada da struct numero_resultado
	if ((id_num = shmget(0x678500, sizeof(int), IPC_CREAT|0x1FF)) < 0)
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
}

void exclui_estruturas_compartilhadas()
{
	// Exclui semaforo
	if (semctl(id_sem, 1, IPC_RMID, NULL) < 0)
	{
		printf("Erro na exclusao do semaforo\n");
		exit(1);
	}
	// Exclui filas de mensagens
	if (msgctl(fila_1, IPC_RMID, NULL) < 0) // recebe referencia_pagina
	{
		printf("Erro na exclusao da fila 1\n");
		exit(1);
	}
	if (msgctl(fila_2, IPC_RMID, NULL) < 0) // envia resposta
	{
		printf("Erro na exclusao da fila 2\n");
		exit(1);
	}
	if (msgctl(fila_pids, IPC_RMID, NULL) < 0) // exclui fila 3 de pids
	{
		printf("Erro na exclusao da fila de pids\n");
		exit(1);
	}
	// Exclui memorias compartilhadas
	if (shmctl(id_tab, IPC_RMID, NULL) < 0)
	{
		printf("erro na exclusao da memoria compartilhada da tabela \n");
		exit(1);
	}
	if (shmctl(id_num, IPC_RMID, NULL) < 0)
	{
		printf("erro na exclusao da memoria compartilhada da struct numeros_resutlado\n");
		exit(1);
	}
}

void envia_pid_shutdown()
{
	//Envia pid para o shutdown
	msg_fila_pids.pid = getpid();
	printf("\nPID: %ld\n", msg_fila_pids.pid);
	if ((msgsnd(fila_pids, &msg_fila_pids, sizeof(msg_fila_pids)-sizeof(long), 0)) < 0)
	{
		printf("Erro no envio de mensagem na fila de pids\n");
		exit(1);
	}
}

void shutdown_alocador()
{
	exclui_estruturas_compartilhadas();
	exit(1);
}

void inicializa_memorias_compartilhadas()
{
	int i;
	for (i = 0; i < NUMERO_FRAMES; i++) {
		ptr_tabela->pid[i] = 0;
		ptr_tabela->livre[i] = true;
		ptr_tabela->pagina[i] = 9999;
		ptr_tabela->tempo_de_referencia[i] = -9999;
	}
	for(i = 0; i < NUMERO_USUARIOS; i++) {
		ptr_result->numero_page_faults[i] = -1;
	}
	ptr_result->numero_exec_substituicao = 0;

	return;
}

bool aloca_frame(mensagem *msg)
{
	int i, ocupacao_tabela = 0;
	bool page_fault = true;

	int pg = atoi(msg->pagina);

	// Verifica se pagina esta na tabela de frames

	for (i = 0; i < NUMERO_FRAMES; i++)
	{
		if (pg == ptr_tabela->pagina[i])
		{
			printf("Pagina %d ENCONTRADA no indice %d \n", pg, i);
			ptr_tabela->tempo_de_referencia[i] = 0; // Marca como usado mais recentemente
			ptr_tabela->pid[i] = msg->pid;
			page_fault = false;
			break;
		}
		if (!ptr_tabela->livre[i]) ocupacao_tabela++; // Conta numero de frames ocupadas
	}

	// Se nao estah, houve page fault
	if(page_fault) {
		printf("Page Fault. Ocupacao: %d\n", ocupacao_tabela);
		
		if (ocupacao_tabela >= MAX_OCUPACAO)
		{
			printf("Nao tem espaco. Libera uma frame.\n");
			Vsem(); // Libera Substituidor Semaforo deve começar com 0 permissoes
			sleep(1); // TODO: alarm ou pause
			Psem();	// Fica bloqueado ate substituidor terminar
		}
		

		for (i = 0; i < NUMERO_FRAMES; i++)
		{
			// Se frame estiver livre, insere nova pagina lah
			if (ptr_tabela->livre[i])
			{
				printf("Pagina %d INSERIDA no indice %d \n", pg, i);
				ptr_tabela->pid[i] = msg->pid;
				ptr_tabela->livre[i] = false;
				ptr_tabela->pagina[i] = pg;
				ptr_tabela->tempo_de_referencia[i] = 0; // Marca como usado mais recentemente
				ocupacao_tabela++;
				break;
			}
		}
	}
	// Incrementa tempo de referencia de todas as paginas
	for (i = 0; i < NUMERO_FRAMES; i++)
		ptr_tabela->tempo_de_referencia[i]++;

	return page_fault;
}

void executa_alocacao() {

	while (1) {
		// O quarto argumento é 0, então a primeira msg da fila é lida
		if ((msgrcv(fila_1, &msg_fila_1, sizeof(msg_fila_1)-sizeof(long), 0, 0)) < 0)
		{
			printf("Erro na obtencao da mensagem na fila 1\n");
			exit(1);
		}
		printf("Recebido: %ld \t %s \n", msg_fila_1.pid, msg_fila_1.pagina);

		msg_fila_2.pid = msg_fila_1.pid;
		strcpy(msg_fila_2.pagina, aloca_frame(&msg_fila_1) ? "Page fault" : "Page found");

		if ((msgsnd(fila_2, &msg_fila_2, sizeof(msg_fila_2)-sizeof(long), 0)) < 0)
		{
			printf("Erro no envio de mensagem na fila 2\n");
			exit(1);
		}
		printf("Enviado: %ld \t %s \n", msg_fila_2.pid, msg_fila_2.pagina);
	}
	return;
}

int main ()
{
	signal(SIGUSR1, shutdown_alocador);
	cria_estruturas_compartilhadas();
	envia_pid_shutdown();
	inicializa_memorias_compartilhadas();
	executa_alocacao();

	return 0;
}
