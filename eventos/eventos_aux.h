#ifndef EVENTOS_AUX
#define EVENTOS_AUX

#include <stdio.h>
#include <stdlib.h>
#include "entidades.h"
#include "lef.h"
#include "mundo.h"
#include "conjunto.h"

// Retorna 1 caso o tipo de evento envolva heróis, se não, retorna 0.
int ev_tem_heroi (int tipo);

// Imprime o conjunto entre "[ ]".
void imprime_cjto_aux (struct cjto_t *c);

// Imprime os IDs dos herois presentes na base na forma "[ a b c ]".
void imprime_presentes (struct base *b);

// Imprime os IDs dos herois da fila de espera da base na forma "[ a b c ]".
void imprime_fila_espera (struct base *b);

// Ordena as distancias das bases em relacao a missao, por insercao.
void ordena_bases_dist (struct mundo *w, struct missao *m, int ids_b[], int dist[]);

// Une as habilidades dos herois ativos de uma base. Retorna o conjunto união ou NULL em erro.
struct cjto_t *uniao_hab_base (struct mundo *w, struct missao *m, struct base *b);

// Busca uma base que atenda as necessidades da missao. Retorna o ID da base ou -1 caso não encontre.
int busca_base (struct mundo *w, struct missao *m, struct cjto_t **uniao);

// Cumpre a missao, calculando o risco, possivel mortes ou atribuindo experiencias.
void cumpre_missao (struct mundo *w, struct missao *m, struct base *b);

// Agenda um evento, inserindo os dados dentro da struct evento e depois inserindo o ev na lef.
// Retorna -1 em erro e 0 em sucesso.
int agendar_ev (struct lef_t *lef, int tipo, int tempo, int d1, int d2);

#endif