
typedef struct tabela
{
	int pid[NUMERO_FRAMES];
	bool livre[NUMERO_FRAMES];
	int pagina[NUMERO_FRAMES];
	int tempo_de_referencia[NUMERO_FRAMES];
} tabela;

struct tabela *ptr_tabela;
struct sembuf op[2];
int id_mem, id_sem;
int fila_3;
long msg_fila_3[7];

//Prototipos de funcoes
void Psem();
void Vsem();
void shutdown_substituidor ();
void obtem_estruturas_compartilhadas();
void executa_substituicao ();
