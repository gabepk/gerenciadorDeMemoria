struct numeros_resultado *ptr_result;
struct mensagem msg_fila_pids;
struct tabela *ptr_tabela;
struct sembuf op[2];
int id_tab, id_sem, id_num;
int fila_pids;

//Prototipos de funcoes
void Psem();
void Vsem();
void shutdown_substituidor ();
void obtem_estruturas_compartilhadas();
void executa_substituicao ();
