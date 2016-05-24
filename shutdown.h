#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>


typedef struct mensagem
{
	long pid; // TODO: nao eh necessariamente um pid 
	char pagina[10]; // pagina[30];
} mensagem;

struct mensagem msg_fila_pids;
int fila_pids;