/** Universidade de Brasilia - CIC
 *  Sistemas Operacionais - professora Alba Cristina
 *  Gabriella de Oliveira Esteves - 110118995
 * 
 *  Ubuntu - versao 14.04 LTS
 *  GCC - versao 4.8.4
 * 
 */

#ifndef UTIL_H
#define UTIL_H

#define NUMERO_PROCESSOS 5 // Numero de forks que processo usuario vai dar
#define MAX_OCUPACAO 4 // Se 5 frames existirem, ocorre substituicao
#define OCUPACAO_OK 3 // Ocupação deve ser < 4
#define NUMERO_FRAMES 5 // 5 page frames existem

int page_fautls_total;
int num_substituicoes;
int page_faults[NUMERO_PROCESSOS];

char *pagina;
char *tempo_de_referencia;

#endif // UTIL_H
