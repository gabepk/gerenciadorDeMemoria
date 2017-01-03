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
#include "../include/shutdown.h"

void obtem_estruturas_compartilhadas()
{
	// Obtem tabela
	if ((id_tab = shmget(0x89951, sizeof(int), 0x1FF)) < 0)
	{
		printf("Erro na obtencao da memoria compartilhada da tabela\n");
		exit(1);
	}
	ptr_tabela = (tabela *) shmat(id_tab, (char *)0, 0);
	if (ptr_tabela == (tabela *)-1) 
	{
		printf("Erro no attach do ponteiro para a tabela de frames\n");
		exit(1);
	}
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

	// Obtem fila de pids para shutdown
	if ( (fila_pids = msgget(0x118785, 0x1FF)) < 0)
	{
		printf("Erro na obtenca da fila de pids\n");
		exit(1);
	}
	return;
}

void imprime_resultado()
{
	int i=0, numero_page_faults_total=0;

	printf("\n\n");
	while(ptr_result->numero_page_faults[i] != -1)  {
		// Numero de page faults eh somado com 1 pois eh inicializado com -1
		printf("\t Numero de page faults do processo %d: %d\n", i, ptr_result->numero_page_faults[i] + 1);
		numero_page_faults_total += ptr_result->numero_page_faults[i] + 1;
		i++;
	}

	printf("\t Numero de page faults total: %d\n", numero_page_faults_total);
	printf("\t Numero de execucoes do processo de substituicao: %d\n", ptr_result->numero_exec_substituicao);
	printf("\t Configuracao final da memoria:\n");
	printf("\t Pid \t Pagina \t Tempo de referecia\n");
	for (i = 0; i < NUMERO_FRAMES; i++)
	{
		if (! ptr_tabela->livre[i])
			printf("\t %d \t %d \t\t %d\n", ptr_tabela->pid[i], ptr_tabela->pagina[i], ptr_tabela->tempo_de_referencia[i]);
		else printf("\t - \t Livre\n");
	}
	printf("\n\n");
	return;
}

int main ()
{
	int i=0, result=1;
	obtem_estruturas_compartilhadas();

	while((msgrcv(fila_pids, &msg_fila_pids, sizeof(msg_fila_pids)-sizeof(long), 0, IPC_NOWAIT)) > 0) {
		kill(msg_fila_pids.pid, SIGUSR1);
	}

	imprime_resultado();

	return 0;
}
