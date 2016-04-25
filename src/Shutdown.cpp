#include "Shutdown.h"

Shutdown::Shutdown()
{
    cout << "Numero de page faults total: " << AlocacaoDePagina::pageFaultsTotal << endl;
    cout << "Numero de page faults do processo: " << SubstituicaoDePagina::numeroSubstituicoes << endl;
    cout << "Configuracao final da memoria:" << endl;
    cout << "\tPagina\t|\tTempo de referencia" << endl;
    for (int posicao = 0; posicao < AlocacaoDePagina::NUMERO_FRAMES; posicao++)
    {
        cout << "\t" << AlocacaoDePagina::numDaPagina[posicao] << "\t|\t" << AlocacaoDePagina::tempoDeReferencia[posicao];
    }
}

