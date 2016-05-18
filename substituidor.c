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
#include <signal.h>

void shutdown_substituidor () {
	exit(1);
}

int main () {
	signal(SIGUSR1, shutdown_substituidor);
	signal(SIGUSR2, substituicao);
	// Obtem semaforo que sera usado para bloquear escrita na tabela invertida
	if ((idsem = semget(0x89950, 1, 0777)) < 0)
	{
		printf("Erro na obtencao do semaforo\n");
		exit(1);
	}
	// executa LRU
	return 0;
}
