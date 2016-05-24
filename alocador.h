
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

struct mensagem msg_fila_1, msg_fila_2;
struct tabela *ptr_tabela;
struct sembuf op[2];
int id_sem, id_mem;
int fila_1, fila_2, fila_3;
long msg_fila_3[7];

int numero_page_faults_total;
int numero_exec_substituicao;

//Prototipos das funcoes
void Psem();
void Vsem();
void cria_estruturas_compartilhadas();
void exclui_estruturas_compartilhadas();
void imprime_tabela();
void shutdown_alocador();
void inicializa_tabela();
bool aloca_frame(mensagem *msg);
void executa_alocacao();
