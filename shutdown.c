/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 *  Naiara Watanabe - 090126785
 *
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 *
 */

#include "shutdown.h"

int main ()
{
	int i=0, result=1;

	// Obtem fila de pids para shutdown
	if ( (fila_pids = msgget(0x118785, 0x1FF)) < 0)
	{
		printf("Erro na obtenca da fila de pids\n");
		exit(1);
	}

	while(result != 0) { // TODO Alterar esse loop, pra nao dar erro de result < 0 sem necessidade

		result = msgrcv(fila_pids, &msg_fila_pids, sizeof(msg_fila_pids)-sizeof(long), 0, IPC_NOWAIT);
		printf("result = %d\t", result);
		printf("pid = %ld\n", msg_fila_pids.pid);

		if(result < 0)
		{
			printf("Erro na obtencao da mensagem na fila de pids\n");
			exit(1);
		}
		else if (result > 0) {
			kill(msg_fila_pids.pid, SIGUSR1);
		}
	}


	return 0;
}
