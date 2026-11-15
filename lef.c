// TAD Fila de prioridades (FPRIO)

#include <stdio.h>
#include <stdlib.h>
#include "lef.h"
#include "eventos.h"
#include "eventos_aux.h" //ev_tem_heroi

// Cria uma fila vazia.
// Retorna o ponteiro para a fila criada ou NULL em caso de erro.
struct lef_t *lef_cria ()
{
    struct lef_t *nova_lef = malloc(sizeof(struct lef_t));
    
    if (!nova_lef)
        return NULL;

    nova_lef->prim = NULL;
    nova_lef->fim = NULL;
    nova_lef->num = 0;

    return nova_lef;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL
struct lef_t *lef_destroi (struct lef_t *f)
{
    if (!f)
        return NULL;

    struct lefnodo_t *atual = f->prim;
    struct lefnodo_t *proximo;

    while (atual) {
        proximo = atual->prox;
        free(atual->item);
        free(atual);
        atual = proximo;
    }

    free(f);
    return NULL;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes. 
// Respeita a politica FIFO.
// Retorna o numero de itens na fila ou -1 em caso de erro.
int lef_insere (struct lef_t *f, void *item, int tipo, int prio)
{
    // Casos invalidos
    if (!f || !item)
        return -1;

    struct lefnodo_t *aux = f->prim;
    struct lefnodo_t *ant_ins = NULL;

    // Verifica se o ponteiro ja existe e encontra onde inserir (apos o "ant_ins")
    while (aux) {
        if (aux->item == item)
            return -1;

        if (aux->prio <= prio)
            ant_ins = aux;

        aux = aux->prox;
    }

    // Alocacao do inserido
    struct lefnodo_t *ins = malloc(sizeof(struct lefnodo_t));
    if (!ins)
        return -1;

    ins->item = item;
    ins->tipo = tipo;
    ins->prio = prio;

    // Verifica o caso do inicio
    if (!ant_ins) {
        ins->prox = f->prim;
        f->prim = ins;

        if (!ins->prox)
            f->fim = ins;

    // Insercao no meio e no fim da fila
    } else {
        ins->prox = ant_ins->prox;
        ant_ins->prox = ins;

        if (ins->prox == NULL)
            f->fim = ins;
    }

    return ++f->num;
}

// Retira o primeiro item da fila e o devolve.
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *lef_retira (struct lef_t *f, int *tipo, int *prio)
{
    // Casos invalidos
    if (!f || !tipo || !prio || !f->prim)
        return NULL;

    *tipo = f->prim->tipo;
    *prio = f->prim->prio;

    struct lefnodo_t *nodo_ret = f->prim;
    void *item_ret = f->prim->item;

    // Atualiza o inicio
    f->prim = f->prim->prox;
    if(!f->prim)
        f->fim = NULL;
    free(nodo_ret);

    f->num--;
    return item_ret;
}

// Informa o numero de itens na fila.
// Retorno: N >= 0 ou -1 em caso de erro.
int lef_tamanho (struct lef_t *f)
{
    if (!f)
        return -1;

    return f->num;
}

// Imprime o conteudo da fila.
void lef_imprime (struct lef_t *f)
{
    if (!f || !f->prim)
        return;

    struct lefnodo_t *aux = f->prim;

    while (aux) {
        if (aux != f->prim)
            printf(" ");

        printf("(%d %d)", aux->tipo, aux->prio);
        aux = aux->prox;
    }
}

// Retira os eventos da LEF relacionados ao heroi
// tipo: -1 remove todos os eventos do heroi
void lef_retira_heroi (struct lef_t *f, int id_h, int tipo)
{
    if (!f)
        return;

    struct lefnodo_t *ant = NULL;
    struct lefnodo_t *atual = f->prim;
    int tipo_alvo;

    while (atual) {
        struct lefnodo_t *prox = atual->prox;
        struct evento *ev = atual->item;

        if (tipo == -1)
            tipo_alvo = ev_tem_heroi(atual->tipo);
        else
            tipo_alvo = atual->tipo == tipo;

        if (tipo_alvo && (ev->dado1 == id_h)) {
            if (ant)
                ant->prox = prox;
            else
                f->prim = prox;

            if (f->fim == atual)
                f->fim = ant;
            
            free(atual->item); // Libera o evento
            free(atual);       // Libera o nodo da lef
            f->num--;
        } else 
            ant = atual;

        atual = prox;
    }
}