#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TAMANHO_LINHA 100

typedef struct mensagem
{
	long pid; // TODO: nao eh necessariamente um pid 
	char pagina[10]; // pagina[30];
} mensagem;

struct mensagem msg_fila_pids;
int fila_1, fila_2, fila_pids;
char *pid_logico;
int numero_page_faults = 0;

//Prototipos de funcoes
void shutdown_usuario ();
void obtem_estruturas_compartilhadas();

