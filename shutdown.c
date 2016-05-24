/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 *  Naiara Watanabe - 090126785
 *
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 *
 */

#include "utils.h" // Imprime as paradas
#include "shutdown.h" // Vazia por enquanto

int main ()
{
	int i = 0;
	while(vetor_pids[i] != -1) {
		printf("pid %d = %d", i, vetor_pids[i++]);
		//kill(vetor_pids[i++], SIGUSR1);
	}

	return 0;
}
