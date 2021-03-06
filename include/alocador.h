#include <stdbool.h>
#include <sys/sem.h>

struct mensagem msg_fila_1, msg_fila_2, msg_fila_pids;
struct numeros_resultado *ptr_result;
struct tabela *ptr_tabela;
struct sembuf op[2];
int id_sem, id_tab, id_num;
int fila_1, fila_2, fila_pids;

//Prototipos das funcoes
void Psem();
void Vsem();
void cria_estruturas_compartilhadas();
void exclui_estruturas_compartilhadas();
void envia_pid_shutdown();
void shutdown_alocador();
void inicializa_memorias_compartilhadas();
bool aloca_frame(struct mensagem *msg);
void executa_alocacao();
