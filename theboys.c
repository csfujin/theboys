// programa principal do projeto "The Boys - 2024/2"
// Autor: Myllena Roberto Farias, GRR 20263159

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "eventos.h"
#include "mundo.h"
#include "entidades.h"
#include "auxiliares.h"

struct estatisticas est;

int main ()
{
    srand(time(NULL));

    struct mundo *w = ini_mundo();
    if(!w)
        return 1;

    if (eventos_iniciais(w) < 0) {
        destroi_mundo(w);
        return 1;
    }

    simula_mundo(w);
    
    destroi_mundo(w);

    return 0;
}