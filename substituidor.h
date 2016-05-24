#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include<sys/msg.h>
#include <sys/types.h>

#define NUMERO_FRAMES 10 // TODO: Colocar tudo num .h soh
#define OCUPACAO_OK 8

typedef struct mensagem
{
	long pid; // TODO: nao eh necessariamente um pid 
	char pagina[10]; // pagina[30];
} mensagem;

typedef struct tabela
{
	int pid[NUMERO_FRAMES];
	bool livre[NUMERO_FRAMES];
	int pagina[NUMERO_FRAMES];
	int tempo_de_referencia[NUMERO_FRAMES];
} tabela;

struct mensagem msg_fila_pids;
struct tabela *ptr_tabela;
struct sembuf op[2];
int id_mem, id_sem;
int fila_pids;

//Prototipos de funcoes
void Psem();
void Vsem();
void shutdown_substituidor ();
void obtem_estruturas_compartilhadas();
void executa_substituicao ();
