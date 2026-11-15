#include <stdio.h>
#include <stdlib.h>
#include "eventos_aux.h"
#include "eventos.h"
#include "fila.h"
#include "lef.h"
#include "conjunto.h"
#include "entidades.h"
#include "mundo.h"
#include "auxiliares.h"

// Retorna 1 caso o tipo de evento envolva heróis
int ev_tem_heroi (int tipo)
{
    if (tipo == EV_CHEGA || tipo == EV_ESPERA || tipo == EV_DESISTE || tipo == EV_ENTRA || tipo == EV_SAI || tipo == EV_VIAJA || tipo == EV_MORRE || tipo == EV_REC_HEROI)
        return 1;
    return 0;
}

// Imprime o conjunto com "[ ]""
void imprime_cjto_aux (struct cjto_t *c)
{
    printf("[ ");
    cjto_imprime(c);
    printf(" ]");
}

// Imprime os IDs dos herois presentes com "[ ]"
void imprime_presentes (struct base *b)
{
    printf("[");
    for (int i = 0; i < b->presentes->cap; i++) {
        if (cjto_pertence(b->presentes, i))
            printf(" %d", i);
    }
    printf(" ]");
}

void imprime_fila_espera (struct base *b)
{
    struct fila_nodo_t *atual = b->fila_espera->prim;

    printf("[");

    while (atual) {
        struct heroi *h = atual->item;
        printf(" %2d", h->id);
        atual = atual->prox;
    }
    
    printf(" ]");
}

// Ordena as distancias por insercao
void ordena_bases_dist (struct mundo *w, struct missao *m, int ids_b[], int dist[]) 
{
    for (int i = 0; i < w->num_bases; i++) {
        ids_b[i] = i;
        dist[i] = dist_cartesiana(m->local, w->bases[i]->local);
    }

    // Insertion sort
    for (int i = 1; i < w->num_bases; i++) {
        int id = ids_b[i];
        int d = dist[i];
        int aux = i - 1;

        while (aux >= 0 && dist[aux] > d) {
            ids_b[aux + 1] = ids_b[aux];
            dist[aux + 1] = dist[aux];
            aux--;
        }
        ids_b[aux + 1] = id;
        dist[aux + 1] = d;
    }
}

struct cjto_t *uniao_hab_base (struct mundo *w, struct missao *m, struct base *b)
{
    struct cjto_t *u = cjto_cria(w->num_habilidades);
    if(!u)
        return NULL;

    for (int i = 0; i < w->num_herois; i++) {
        if (!cjto_pertence(b->presentes, i))
            continue;

        struct heroi *h = w->herois[i];
        if (!h->estado)
            continue;

        // depuracao
        printf("%6d: MISSAO %d HAB HEROI %2d: ", w->relogio, m->id, h->id);
        imprime_cjto_aux(h->habilidades);
        printf("\n");

        for (int j = 0; j < w->num_habilidades; j++)
            if (cjto_pertence(h->habilidades, j))
                cjto_insere(u, j);
    }

    //depuracao
    printf("%6d: MISSAO %d UNIAO HAB BASE %d: ", w->relogio, m->id, b->id);
    imprime_cjto_aux(u);
    printf("\n");

    return u;
}

int busca_base (struct mundo *w, struct missao *m, struct cjto_t **uniao)
{
    int ids_b[N_BASES];
    int dist[N_BASES];

    ordena_bases_dist(w, m, ids_b, dist);

    for (int i = 0; i < w->num_bases; i++) {
        struct base *b = w->bases[ids_b[i]];
        
        if(!b->estado)
            continue;

        //depuracao
        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS ", w->relogio, m->id, b->id, dist[i]);
        imprime_presentes(b);
        printf("\n");

        struct cjto_t *u = uniao_hab_base(w, m, b);
            
        if (cjto_contem(u, m->habilidades)) {
            *uniao = u;
            return b->id;
        }
        cjto_destroi(u);
    }

    return -1;
}

void cumpre_missao (struct mundo *w, struct missao *m, struct base *b)
{
    est.m_cumpridas++;
    est.missoes_base[b->id]++;

    for (int i = 0; i < w->num_herois; i++) {
        if (!cjto_pertence(b->presentes, i))
            continue;

        struct heroi *h = w->herois[i];
        if (!h->estado)
            continue;

        int risco = m->perigo / (h->paciencia + h->exp + 1);
        
        if (risco > aleatorio(0, 30))
            agendar_ev(w->lef, EV_MORRE, w->relogio, h->id, m->id);
        else
            h->exp++;
    }
}

// Agenda um evento.
// Retorna -1 em erro e 0 em sucesso
int agendar_ev (struct lef_t *lef, int tipo, int tempo, int d1, int d2)
{
    struct evento *ev = malloc(sizeof(struct evento));
    if (!ev)
        return -1;

    ev->dado1 = d1;
    ev->dado2 = d2;

    if (lef_insere(lef, ev, tipo, tempo) < 0) {
        free(ev);
        return -1;
    }

    return 0;
}