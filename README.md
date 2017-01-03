# Gerenciador de Memória

## Introdução

Simulação de gerenciamento de memória de um sistema operacional utilizando algoritmo NFU (Not Frequently Used) para substituição de páginas.

## Execução em Background

$ cd src

$ chmod 777 compila.sh

$ ./compila.sh


$ ./alocador & <ENTER>

$ ./substituidor & <ENTER>



$ ./usuario paginas/pag_processo_0.txt & <ENTER>

$ ./usuario paginas/pag_processo_1.txt & <ENTER>

...

$ ./usuario paginas/pag_processo_4.txt & <ENTER>


$ ./shutdown

## Observações

Assumimos que:
* Numero de frames maxima é 10
* Numero de usuários maximo é 5
* Arquivo texto que acompanha ./usuario deve sempre terminar com um digito antes de ".txt", pois este será seu pid_logico no codigo

Gerenciamento de memória compartilhada:
* Semaforos para bloquear a escrita na tabela invertida, no alocador e no substituidor.
* Fila de mensagens (fila 1 e 2) entre alocador e usuario, para solicitar a pagina e receber resposta (page fault ou não) , respectivamente.
* Fila de mensagens (fila pids) para que os processos enviem seus pids para o shutdown
* Memoria compartilhada para a tabela invertida, no alocador, substituidor e no shutdown (para a impressão)
* Memoria compartilhada para o vetor de número de page faults e o numero de execuções do substituidor, para a impressao no shutdown.

## Membros

Gabriella de Oliveira Esteves - 110118995

Naiara Watanabe - 090126785

## Configurações
 
Ubuntu - versao 14.04 LTS - GCC 4.8.4

MV Ubuntu - versao 14.10 - GCC 4.9.2
