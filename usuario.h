
typedef struct mensagem
{
	long pid;
	char pagina[10];
} mensagem;
int fila_1, fila_2, fila_3;
char *pid_logico;
long msg_fila_3[7];

//Prototipos de funcoes
void shutdown_usuario ();
void obtem_estruturas_compartilhadas();

