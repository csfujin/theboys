#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"
#include "entidades.h"
#include "conjunto.h"
#include "lef.h"
#include "fila.h"
#include "eventos.h"
#include "auxiliares.h"

#define PACI_MAX 100  // Paciencia maxima do heroi
#define VEL_MIN 50    // Velocidade minima do heroi
#define VEL_MAX 5000  // Velocidade maxima do heroi

// -------------- INICIALIZACOES DE ENTIDADES ----------------
// Inicializacao de cada heroi. Retorna -1 em erro 
int ini_heroi (struct heroi *h, int n)
{
    h->id = n;
    h->exp = 0;
    h->paciencia = aleatorio(0, PACI_MAX); // random (0..100)
    h->vel = aleatorio(VEL_MIN, VEL_MAX); // random (50..5000)
    h->estado = 1;
    h->base = -1; // Inicializa em nenhuma base

    int qnt_hab = aleatorio(1, 3);
    h->habilidades = cjto_aleat(qnt_hab, N_HABILIDADES);
    if(!h->habilidades)
        return -1;
    
    return 0;
}

// Inicializacao de cada base. Retorna -1 em erro
int ini_base (struct base *b, int n)
{
    b->id = n;
    b->local.x = aleatorio(0, N_TAMANHO_MUNDO-1);
    b->local.y = aleatorio(0, N_TAMANHO_MUNDO-1);
    b->lotacao = aleatorio(3,10);
    b->estado = 1;
    b->presentes = cjto_cria(N_HEROIS);
    b->fila_espera = fila_cria(); 
    if(!b->presentes || !b->fila_espera)
        return -1;

    return 0;
}

// Inicializacao de cada missao. Retorna -1 em erro
int ini_missao (struct missao *m, int n)
{
    m->id = n;
    m->local.x = aleatorio(0, N_TAMANHO_MUNDO-1);
    m->local.y = aleatorio(0, N_TAMANHO_MUNDO-1);
    m->perigo = aleatorio(0, 100);

    int qnt_hab = aleatorio(6,10);
    m->habilidades = cjto_aleat(qnt_hab, N_HABILIDADES);
    if(!m->habilidades)
        return -1;

    return 0;
}

// Inicializacao de cada invasao.
void ini_invasao (struct invasao *i, int n)
{
    i->id = n;
    i->id_base = aleatorio(0, N_BASES-1);
    i->t_penalidade_h = aleatorio(0, 4320); // Max = 3 dias.
    i->t_penalidade_b = aleatorio(0, 7200); // Max = 5 dias. 
}

struct mundo *falha_mundo (struct mundo *w)
{
    destroi_mundo(w);
    return NULL;
}
// -----------------------------------------------------------------

// Inicializa o mundo e os conteúdos.
struct mundo *ini_mundo ()
{
    struct mundo *w = calloc(1, sizeof(struct mundo));
    if (!w)
        return NULL;

    w->num_herois = N_HEROIS;
    w->num_bases = N_BASES;
    w->num_missoes = N_MISSOES;
    w->num_invasoes = N_INVASOES;
    w->num_habilidades = N_HABILIDADES;
    w->tamanho_mundo.x = N_TAMANHO_MUNDO;
    w->tamanho_mundo.y = N_TAMANHO_MUNDO;
    w->relogio = T_INICIO;

    w->lef = lef_cria();
    if(!w->lef)
        return falha_mundo(w);

    // Alocacao de herois
    w->herois = calloc(w->num_herois, sizeof(struct heroi *));
    if(!w->herois)
        return falha_mundo(w);

    for (int i = 0; i < w->num_herois; i++) {
        w->herois[i] = calloc(1, sizeof(struct heroi));
        if (!w->herois[i] || ini_heroi(w->herois[i], i) < 0)
            return falha_mundo(w);
    }

    // Alocacao de bases
    w->bases = calloc(w->num_bases, sizeof(struct base *));
    if(!w->bases)
        return falha_mundo(w);

    for (int i = 0; i < w->num_bases; i++) {
        w->bases[i] = calloc(1, sizeof(struct base));
        if(!w->bases[i] || ini_base(w->bases[i], i) < 0)
            return falha_mundo(w);
    }

    // Alocacao de missoes
    w->missoes = calloc(w->num_missoes, sizeof(struct missao *));
    if(!w->missoes)
        return falha_mundo(w);

    for (int i = 0; i < w->num_missoes; i++) {
        w->missoes[i] = malloc(sizeof(struct missao));
        if(!w->missoes[i] || ini_missao(w->missoes[i], i) < 0)
            return falha_mundo(w);
    }
    
    // Alocacao de invasoes
    w->invasoes = calloc(w->num_invasoes, sizeof(struct invasao *));
    if(!w->invasoes)
        return falha_mundo(w);

    for (int i = 0; i < w->num_invasoes; i++) {
        w->invasoes[i] = calloc(1, sizeof(struct invasao));
        if(!w->invasoes[i])
            return falha_mundo(w);

        ini_invasao(w->invasoes[i], i);
    }

    return w;
}

// Loop de simulacao de mundo e eventos.
void simula_mundo (struct mundo *w) 
{

    struct evento *ev_atual;
    int ev_tipo, ev_tempo;
    int fim_simul = 0;

    while(!fim_simul) {
        ev_atual = lef_retira(w->lef, &ev_tipo, &ev_tempo);
        if(!ev_atual)
            break;

        w->relogio = ev_tempo;
        est.ev_tratados++;

        switch (ev_tipo)
        {
            case EV_CHEGA:
                ev_chega(w, ev_atual);
                break;

            case EV_ESPERA:
                ev_espera(w, ev_atual);
                break;

            case EV_DESISTE:
                ev_desiste(w, ev_atual);
                break;

            case EV_AVISA:
                ev_avisa(w, ev_atual);
                break;

            case EV_ENTRA:
                ev_entra(w, ev_atual);
                break;

            case EV_SAI:
                ev_sai(w, ev_atual);
                break;

            case EV_VIAJA:
                ev_viaja(w, ev_atual);
                break;

            case EV_MORRE:
                ev_morre(w, ev_atual);
                break;

            case EV_MISSAO:
                ev_missao(w, ev_atual);
                break;

            case EV_INVADE:
                ev_invade(w, ev_atual);
                break;

            case EV_REC_HEROI:
                ev_rec_heroi(w, ev_atual);
                break;

            case EV_REC_BASE:
                ev_rec_base(w, ev_atual);
                break;

            case EV_FIM:
                ev_fim(w);
                fim_simul = 1;
                break;
        }
        free(ev_atual);
    }  
}

// Loops para aceitar um mundo que nao esteja completo.
// Destroi entidades, filas, conjuntos e por fim, o mundo.
void destroi_mundo(struct mundo *w)
{
    if(!w)
        return;
    
    lef_destroi(w->lef);

    if (w->herois) {
        for(int i = 0; i < w->num_herois; i++) {
            struct heroi *h = w->herois[i];

            if (!h)
                continue;

            if(h->habilidades)
                cjto_destroi(h->habilidades);

            free(h);
        }
        free(w->herois);
    }

    if (w->bases) {
        for(int i = 0; i < w->num_bases; i++) {
            struct base *b = w->bases[i];

            if(!b)
                continue;

            if(b->presentes)
                cjto_destroi(b->presentes);

            fila_destroi(b->fila_espera);
            free(b);
        }
        free(w->bases);
    }

    if (w->missoes) {
        for (int i = 0; i < w->num_missoes; i++) {
            struct missao *m = w->missoes[i];

            if (!m)
                continue;

            if(m->habilidades)
                cjto_destroi(m->habilidades);
            free(m);
        }
        free(w->missoes);
    }
   
    if (w->invasoes) {
        for (int i = 0; i < w->num_invasoes; i++)
            free(w->invasoes[i]);
        free(w->invasoes);
    }

    free(w);
}