#include <sys/types.h>
#include <sys/sem.h>
#include <stdbool.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define NUMERO_USUARIOS 5 
#define NUMERO_FRAMES 10 
#define MAX_OCUPACAO 9 // Se 9 frames existirem, ocorre substituicao
#define OCUPACAO_OK 8 // Ocupação deve ser < 8
#define TAMANHO_LINHA 100
