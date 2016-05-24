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

int main ()
{

	int fila_3, i=0;
	long msg_fila_3[7];	

	if ( (fila_3 = msgget(0x118785, 0x1FF)) < 0) // obtem fila de pids para shutdown
	{
		printf("Erro na obtenca da fila 3\n");
		exit(1);
	}

	if ((msgrcv(fila_3, &msg_fila_3, sizeof(msg_fila_3)-sizeof(long), 0, 0)) < 0) //obtem os pids para shutdown
	{
		printf("Erro na obtencao da mensagem na fila 3\n");
		exit(1);
	}
	//mata todos os processos em execucao
	while(msg_fila_3[i] != 0)
	{
		printf("msg_fila_3[%d]: %ld\n", i, msg_fila_3[i]);
		kill(msg_fila_3[i], SIGUSR1);
		i++;
	}

	if (msgctl(fila_3, IPC_RMID, NULL) < 0) // exclui fila 3 de pids
	{
		printf("Erro na exclusao da fila 3\n");
		exit(1);
	}

	return 0;
}
