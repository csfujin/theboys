#include <stdio.h>
#include <stdlib.h>
#include "eventos.h"
#include "eventos_aux.h"
#include "fila.h"
#include "lef.h"
#include "conjunto.h"
#include "entidades.h"
#include "mundo.h"
#include "auxiliares.h"

// Cria os eventos iniciais
int eventos_iniciais (struct mundo *w)
{
    // Agenda o evento CHEGA para cada herói
    for (int i = 0; i < w->num_herois; i++) {
        int t = aleatorio(0, 4320); // 3 dias max
        int b = aleatorio(0, w->num_bases - 1);

        // Dado1: ID heroi, Dado2: Id base.
        if(agendar_ev(w->lef, EV_CHEGA, t, i, b) < 0)
            return -1;
    }   

    // Agenda os eventos MISSAO
    for (int i = 0; i < w->num_missoes; i++) {
        int t = aleatorio(0, T_FIM_DO_MUNDO);

        // Dado 1: ID missao
        if(agendar_ev(w->lef, EV_MISSAO, t, i, -1) < 0)
            return -1;
    }

    // Agenda os eventos INVADE
    for (int i = 0; i < w->num_invasoes; i++) {
        int t = aleatorio((60*24*3)+1, T_FIM_DO_MUNDO);
        int b = w->invasoes[i]->id_base;

        // Dado 1: ID invasao, Dado 2: ID base
        if(agendar_ev(w->lef, EV_INVADE, t, i, b) < 0)
            return -1;
    }

    if(agendar_ev(w->lef, EV_FIM, T_FIM_DO_MUNDO, -1, -1) < 0)
        return -1;

    return 0;
}

// Heroi chega na base e decide se espera ou desiste.
// Recebe: ev->dado1 = ID heroi, ev->dado2 = base
void ev_chega (struct mundo *w, struct evento *ev)
{
    struct heroi *h = w->herois[ev->dado1];
    struct base *b = w->bases[ev->dado2];

    h->base = b->id; // Atualiza a base do heroi

    int n_presentes = cjto_card(b->presentes);
    int na_fila = fila_tamanho(b->fila_espera);
    int espera = 0;

    if (((n_presentes < b->lotacao) && (na_fila == 0)) || (h->paciencia > (10 * na_fila)))
        espera = 1;

    espera = espera && b->estado;

    agendar_ev(w->lef, espera ? EV_ESPERA : EV_DESISTE, w->relogio, h->id, b->id);
    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) %s\n", w->relogio, h->id, b->id, n_presentes, b->lotacao, espera ? "ESPERA" : "DESISTE");
}

// Heroi entra na fila e porteiro e avisado.
// Recebe: ev->dado1 = ID heroi, ev->dado2 = base
void ev_espera (struct mundo *w, struct evento *ev)
{
    struct heroi *h = w->herois[ev->dado1];
    struct base *b = w->bases[ev->dado2];

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", w->relogio, h->id, b->id, fila_tamanho(b->fila_espera));
    
    fila_insere(b->fila_espera, h);

    if(fila_tamanho(b->fila_espera) > est.maior_fila[b->id])
        est.maior_fila[b->id] = fila_tamanho(b->fila_espera);

    agendar_ev(w->lef, EV_AVISA, w->relogio, b->id, -1);
}

// Heroi escolhe outra base e viaja para ela.
// Recebe: ev->dado1 = ID heroi, ev->dado2 = base
void ev_desiste (struct mundo *w, struct evento *ev)
{
    struct heroi *h = w->herois[ev->dado1];
    struct base *b = w->bases[ev->dado2];
    
    printf("%6d: DESISTE HEROI %2d BASE %d\n", w->relogio, h->id, b->id);

    int b_destino = aleatorio(0, w->num_bases-1);
    agendar_ev(w->lef, EV_VIAJA, w->relogio, h->id, b_destino);
}

// Avisa o porteiro para permitir entrada dos herois enquanto houver vagas.
// Recebe: ev->dado1 = ID base, ev->dado2 = -1
void ev_avisa (struct mundo *w, struct evento *ev)
{
    struct base *b = w->bases[ev->dado1];
    if (!b->estado)
        return;

    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA ", w->relogio, b->id, cjto_card(b->presentes) , b->lotacao);
    imprime_fila_espera(b);
    printf("\n"); 

    while((cjto_card(b->presentes) < b->lotacao) && (fila_tamanho(b->fila_espera) > 0)) {
        struct heroi *h = fila_retira(b->fila_espera);
        if (!h)
            break;

        cjto_insere(b->presentes, h->id);
        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", w->relogio, b->id, h->id);

        agendar_ev(w->lef, EV_ENTRA, w->relogio, h->id, b->id);
    }
}

// Heroi entra e decide quanto tempo fica.
// Recebe: ev->dado1 = ID heroi, ev->dado2 = base
void ev_entra (struct mundo *w, struct evento *ev) 
{
    struct heroi *h = w->herois[ev->dado1];
    struct base *b = w->bases[ev->dado2];
   
    // Calcula tempo de permanência na base
    int tpb = 15 + h->paciencia * aleatorio(1,20);
    int t_saida = w->relogio + tpb;

    agendar_ev(w->lef, EV_SAI, t_saida, h->id, b->id);

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", w->relogio, h->id, b->id, cjto_card(b->presentes), b->lotacao, t_saida);
}

// Herois sai e viaja para outra base.
// Recebe: ev->dado1 = ID heroi, ev->dado2 = base
void ev_sai (struct mundo *w, struct evento *ev) 
{
    struct heroi *h = w->herois[ev->dado1];
    struct base *b = w->bases[ev->dado2];

    // so sai quem esta na base
    if(cjto_pertence(b->presentes, h->id) != 1)
        return;

    cjto_retira(b->presentes, h->id);

    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", w->relogio, h->id, b->id, cjto_card(b->presentes), b->lotacao);

    int b_destino = aleatorio(0, w->num_bases - 1);

    agendar_ev(w->lef, EV_VIAJA, w->relogio, ev->dado1, b_destino);
    agendar_ev(w->lef, EV_AVISA, w->relogio, ev->dado2, -1);
}

// Recebe: ev->dado1 = ID heroi, ev->dado2 = ID base (destino)
void ev_viaja (struct mundo *w, struct evento *ev) 
{
    struct heroi *h = w->herois[ev->dado1];
    struct base *dest = w->bases[ev->dado2];

    // Calcula duração da viagem
    int distancia = dist_cartesiana(w->bases[h->base]->local, dest->local);
    int duracao = distancia / h->vel;
    int t_chegada = w->relogio + duracao;

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", w->relogio, h->id, h->base, dest->id, distancia, h->vel, t_chegada);

    agendar_ev(w->lef, EV_CHEGA, w->relogio + duracao, h->id, dest->id);
}

// O heroi morre no instante T
// Recebe: ev->dado1 = ID heroi, ev->dado2 = missao
void ev_morre (struct mundo *w, struct evento *ev) 
{
    struct heroi *h = w->herois[ev->dado1];
    struct base *b = w->bases[h->base];

    cjto_retira(b->presentes, h->id);
    est.mortes_totais++;
    h->base = -1; // Identifica heroi morto

    printf("%6d: MORRE HEROI %2d MISSAO %d\n", w->relogio, h->id, ev->dado2);

    lef_retira_heroi(w->lef, h->id, -1);
    agendar_ev(w->lef, EV_AVISA, w->relogio, b->id, -1);
}

// Recebe: ev->dado1 = ID missao, ev->dado2 = -1
void ev_missao (struct mundo *w, struct evento *ev)
{
    struct missao *m = w->missoes[ev->dado1];
    struct cjto_t *uniao_hab = NULL;

    est.tentativas[m->id]++;

    printf("%6d: MISSAO %d TENT %d HAB REQ: ", w->relogio, m->id, est.tentativas[m->id]);
    imprime_cjto_aux(m->habilidades);
    printf("\n");

    int bmp = busca_base(w, m, &uniao_hab);

    if (bmp < 0) {
        printf("%6d: MISSAO %d IMPOSSIVEL\n", w->relogio, m->id);
        agendar_ev(w->lef, EV_MISSAO, w->relogio + (24 * 60), m->id, -1);
        return;
    }

    printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: ", w->relogio, m->id, bmp);
    imprime_cjto_aux(uniao_hab);
    printf("\n");
    cjto_destroi(uniao_hab);

    cumpre_missao(w, m, w->bases[bmp]);
}

// Recebe ev->dado1 = ID da invasao, ev->dado2 = ID base
void ev_invade (struct mundo *w, struct evento *ev)
{
    struct invasao *inv = w->invasoes[ev->dado1];
    struct base *b = w->bases[ev->dado2];

    if (!b->estado)
        return;

    est.invasoes++;
    printf("%6d: INVADE BASE %d INVADE %d\n", w->relogio, b->id, inv->id);
    printf("%6d: INVADE %d BASE %d HEROIS INATIVOS ", w->relogio, inv->id, b->id);
    imprime_presentes(b);
    printf("\n");

    for (int i = 0; i < w->num_herois; i++) {
        if(cjto_pertence(b->presentes, i)) {
            struct heroi *h = w->herois[i];

            h->estado = 0;
            agendar_ev(w->lef, EV_REC_HEROI, w->relogio + inv->t_penalidade_h, h->id, -1);

            lef_retira_heroi(w->lef, h->id, EV_SAI);
            agendar_ev(w->lef, EV_SAI, w->relogio, h->id, b->id);
        }
    }

    b->estado = 0;
    agendar_ev(w->lef, EV_REC_BASE, w->relogio + inv->t_penalidade_b, b->id, -1);
    printf("%6d: INVADE %d BASE %d INATIVA\n", w->relogio, inv->id, b->id);
}

// Recebe ev->dado1 = ID heroi, ev->dado2 = -1;
void ev_rec_heroi (struct mundo *w, struct evento *ev)
{
    w->herois[ev->dado1]->estado = 1;
}

// Recebe ev->dado1 = ID base, ev->dado2 = -1;
void ev_rec_base (struct mundo *w, struct evento *ev)
{
    w->bases[ev->dado1]->estado = 1;
    agendar_ev(w->lef, EV_AVISA, w->relogio, ev->dado1, -1);
}


void ev_fim (struct mundo *w)
{
    printf("%6d: FIM\n", w->relogio);

    for (int i = 0; i < w->num_herois; i++) {
        struct heroi *h = w->herois[i];

        // Se base =-1, significa que o heroi esta morto, pois todos sao inicializados em alguma base.
        printf("HEROI %2d %s PAC %3d VEL %4d EXP %4d HABS ", h->id, h->base == -1 ? "MORTO" : "VIVO", h->paciencia, h->vel, h->exp);
        imprime_cjto_aux(h->habilidades);
        printf("\n");
    }

    for (int i = 0; i < w->num_bases; i++) {
        struct base *b = w->bases[i];
        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", b->id, b->lotacao, est.maior_fila[i], est.missoes_base[i]);
    }

    int min = 0;
    int max = 0;
    int soma_tent = 0;

    for (int i = 0; i < w->num_missoes; i++) {
        int tent = est.tentativas[i];
        soma_tent += tent;

        if (tent > 0 && (!min || tent < min)) {
            min = tent;
        if (tent > max)
            max = tent;
        }
    }

    printf("EVENTOS TRATADOS: %d\n", est.ev_tratados);

    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", est.m_cumpridas, w->num_missoes, 100.0 * est.m_cumpridas / w->num_missoes);

    printf("INVASOES REALIZADAS: %d/%d (%.1f%%)\n", est.invasoes, w->num_invasoes, 100.0 * est.invasoes / w->num_invasoes);

    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", min, max, (float) soma_tent / w->num_missoes);

    printf("TAXA MORTALIDADE: %.1f%%\n", 100.0 * est.mortes_totais / w->num_herois);   
}