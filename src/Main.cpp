#include "../include/Shutdown.h"
#include "../include/AlocacaoDePagina.h"

#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define N 1

using namespace std;
pthread_mutex_t mutex;

void referenciaPagina(string i)
{

cout << endl << "Pagina: " << i << endl;
}

void* processoDeUsuario (void *id) {
    int pid = *((int *) id);
    ifstream arquivo;
    string paginas;

    // Le arquivo que contém paginas a serem referenciadas
    arquivo.open("pag_processo_" + (char)pid);
    if (arquivo.is_open())
    {
        getline(arquivo, paginas);
        arquivo.close();
    }
    else cout << "Erro ao abrir arquivo pag_processo_" << (char)pid << endl;

    // Chama primitiva para referenciar paginas
    //while ()
    {
        pthread_mutex_lock(&mutex); // Comeca secao critica
        referenciaPagina(paginas);
        pthread_mutex_unlock(&mutex); // Terminica secao critica
    }

}


int main () {
    pthread_t usuarios[N], alocadorDePagina;
    pthread_mutex_init(&mutex, NULL);
    int i;

    for (i = 0; i < N; i++)
    {
        pthread_create(&usuarios[i], NULL, processoDeUsuario, (void*) i);
    }

    return 0;
}
