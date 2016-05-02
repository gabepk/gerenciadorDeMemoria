/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 * 	Gabriella de Oliveira Esteves - 110118995
 * 
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 * 
 */

#include "util.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>

void referencia_pagina(int i)
{
	// tentar reservar page frame pra pag i
	/// se pág i já tá lá
		// retorna ela
	/// se não
		/// se tiver espaço na frame
			// mapear página numa page frame livre aleatória
			// sinalizar que houve page fault signal unix 
			int pid = getpid();
			printf("\t pid : %d", pid);
		    *(page_faults[pid]) = *(page_faults[pid]) + 1;
			(*page_fautls_total)++;
		/// se não
			// substituição
			(*num_substituicoes)++;
}
