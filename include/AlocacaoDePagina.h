#ifndef ALOCACAODEPAGINA_H
#define ALOCACAODEPAGINA_H

#include "SubstituicaoDePagina.h"
#include <string>

#define OCUPACAO_OK 8
#define MAX_OCUPACAO 9
#define NUMERO_FRAMES 10

using namespace std;

int pageFaultsTotal;
string numDaPagina[NUMERO_FRAMES];
string tempoDeReferencia[NUMERO_FRAMES];

class AlocacaoDePagina
{
    public:
        AlocacaoDePagina();
        virtual ~AlocacaoDePagina();
    protected:
    private:
};

#endif // ALOCACAODEPAGINA_H
