
typedef struct mensagem
{
	long pid;
	char pagina[10];
} mensagem;
int fila_1, fila_2;
int pid_logico;

//Prototipos de funcoes
void shutdown_usuario ();
void obtem_estruturas_compartilhadas();

