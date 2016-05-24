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
	long pid;
	char pagina[10];
} mensagem;
int fila_1, fila_2, fila_3;
char *pid_logico;
int numero_page_faults = 0;
long msg_fila_3[7];

//Prototipos de funcoes
void shutdown_usuario ();
void obtem_estruturas_compartilhadas();

