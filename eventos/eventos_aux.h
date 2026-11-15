#ifndef EVENTOS_AUX
#define EVENTOS_AUX

#include <stdio.h>
#include <stdlib.h>
#include "entidades.h"
#include "lef.h"
#include "mundo.h"
#include "conjunto.h"

int ev_tem_heroi (int tipo);

void imprime_cjto_aux (struct cjto_t *c);

void imprime_presentes (struct base *b);

void imprime_fila_espera (struct base *b);

void ordena_bases_dist (struct mundo *w, struct missao *m, int ids_b[], int dist[]);

struct cjto_t *uniao_hab_base (struct mundo *w, struct missao *m, struct base *b);

int busca_base (struct mundo *w, struct missao *m, struct cjto_t **uniao);

void cumpre_missao (struct mundo *w, struct missao *m, struct base *b);

int agendar_ev (struct lef_t *lef, int tipo, int tempo, int d1, int d2);

#endif