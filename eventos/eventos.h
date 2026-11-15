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

// CHEGA     -> d1: heroi, d2: base
// ESPERA    -> d1: heroi, d2: base
// DESISTE   -> d1: heroi, d2: base
// AVISA     -> d1: base, d2: -1
// ENTRA     -> d1: heroi, d2: base
// SAI       -> d1: heroi, d2: base
// VIAJA     -> d1: heroi, d2: base
// MORRE     -> d1: heroi, d2: missao
// MISSAO    -> d1: missão, d2: -1
// INVADE    -> d1: invasao, d2: base
// REC_HEROI -> d1: heroi, d2: -1
// REC_BASE  -> d1: base, d2: -1
// FIM       -> d1: -1, d2: -1

struct evento {
    int dado1;
    int dado2;
};

int eventos_iniciais (struct mundo *w);

void ev_chega (struct mundo *w, struct evento *ev);

void ev_espera (struct mundo *w, struct evento *ev);

void ev_desiste (struct mundo *w, struct evento *ev);

void ev_avisa (struct mundo *w, struct evento *ev);

void ev_entra (struct mundo *w, struct evento *ev);

void ev_sai (struct mundo *w, struct evento *ev);

void ev_viaja (struct mundo *w, struct evento *ev);

void ev_morre (struct mundo *w, struct evento *ev);

void ev_missao (struct mundo *w, struct evento *ev);

void ev_invade (struct mundo *w, struct evento *ev);

void ev_rec_heroi (struct mundo *w, struct evento *ev);

void ev_rec_base (struct mundo *w, struct evento *ev);

void ev_fim (struct mundo *w);

#endif