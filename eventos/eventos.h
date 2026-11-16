#ifndef EVENTOS
#define EVENTOS

#include <stdlib.h>
#include "entidades.h"
#include "fila.h"
#include "lef.h"
#include "mundo.h"

// Definição do int associado a cada evento
#define EV_CHEGA 1
#define EV_ESPERA 2
#define EV_DESISTE 3
#define EV_AVISA 4
#define EV_ENTRA 5
#define EV_SAI 6
#define EV_VIAJA 7
#define EV_MORRE 8
#define EV_MISSAO 9
#define EV_INVADE 10
#define EV_REC_HEROI 11
#define EV_REC_BASE 12
#define EV_FIM 13

// MISSAO    -> d1: missão, d2: -1
// INVADE    -> d1: invasao, d2: base
// REC_HEROI -> d1: heroi, d2: -1
// REC_BASE  -> d1: base, d2: -1
// FIM       -> d1: -1, d2: -1

struct evento {
    int dado1;
    int dado2;
};

// Agenda os eventos iniciais. Retorna -1 em erro e 0 em sucesso.
int eventos_iniciais (struct mundo *w);

// Heroi chega na base e decide se espera ou desiste. Agenda o ESPERA ou DESISTE.
// Recebe: ev->dado1 = ID heroi, ev->dado2 = base
void ev_chega (struct mundo *w, struct evento *ev);

// Heroi entra na fila e porteiro e avisado. Agenda o AVISA.
// Recebe: ev->dado1 = ID heroi, ev->dado2 = base
void ev_espera (struct mundo *w, struct evento *ev);

// Heroi escolhe outra base e viaja para ela. Agenda o VIAJA.
// Recebe: ev->dado1 = ID heroi, ev->dado2 = base
void ev_desiste (struct mundo *w, struct evento *ev);

// Avisa o porteiro para permitir entrada dos herois enquanto houver vagas. Agenda o ENTRA.
// Recebe: ev->dado1 = ID base, ev->dado2 = -1
void ev_avisa (struct mundo *w, struct evento *ev);

// Heroi entra e decide quanto tempo fica. Agenda o SAI.
// Recebe: ev->dado1 = ID heroi, ev->dado2 = base
void ev_entra (struct mundo *w, struct evento *ev);

// Herois sai e viaja para outra base. Agenda o VIAJA e AVISA.
// Recebe: ev->dado1 = ID heroi, ev->dado2 = base
void ev_sai (struct mundo *w, struct evento *ev);

// O heroi se desloca para uma base destino, pondendo ser a mesma onde já está. Agenda o CHEGA.
// Recebe: ev->dado1 = ID heroi, ev->dado2 = ID base (destino)
void ev_viaja (struct mundo *w, struct evento *ev);

// O heroi morre e retirado da base e seus eventos sao retirados da lef. Agenda o AVISA.
// Recebe: ev->dado1 = ID heroi, ev->dado2 = missao
void ev_morre (struct mundo *w, struct evento *ev);

// Procura base adequada e cumpre a missao se possivel, ou reagenda para 24h depois.
// Recebe: ev->dado1 = ID missao, ev->dado2 = -1
void ev_missao (struct mundo *w, struct evento *ev);

// Base e herois presentes ficam no estado inativo. Agenda o SAI, REC_HEROI e REC_BASE.
// Recebe ev->dado1 = ID da invasao, ev->dado2 = ID base
void ev_invade (struct mundo *w, struct evento *ev);

// O heroi volta a ser ativo.
void ev_rec_heroi (struct mundo *w, struct evento *ev);

// A base volta a estar ativa. Agenda o AVISA.
void ev_rec_base (struct mundo *w, struct evento *ev);

// Encerra a simulacao e imprime estatisticas do mundo.
void ev_fim (struct mundo *w);

#endif