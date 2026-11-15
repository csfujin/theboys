// TAD Fila

#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

// Cria uma fila vazia.
// Retorna ponteiro para a lista ou NULL em caso de erro.
struct fila_t *fila_cria ()
{
    struct fila_t *nova_fila = malloc (sizeof(struct fila_t));

    if (!nova_fila)
        return NULL;

    nova_fila->num = 0;
    nova_fila->prim = NULL;
    nova_fila->fim = NULL;

    return nova_fila;
}

// Remove todos os itens da lista e libera a memoria.
// Retorna NULL.
struct fila_t *fila_destroi (struct fila_t *f)
{
    if (!f)
        return NULL;

    struct fila_nodo_t *atual = f->prim;
    struct fila_nodo_t *proximo;

    while (atual) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }

    free(f);
    return NULL;
}

// Insere o item na fila.
// Retorna o numero de itens na fila apos a operacao ou -1 em caso de erro.
int fila_insere (struct fila_t *f, void *item)
{
    // Casos invalidos
    if (!f || !item)
        return -1;

    struct fila_nodo_t *aux = f->prim;

    // Verifica se o item ja existe
    while (aux) {
        if (aux->item == item)
            return -1;
        aux = aux->prox;
    }

    struct fila_nodo_t *ins = malloc(sizeof(struct fila_nodo_t));
    if (!ins)
        return -1;

    ins->item = item;
    ins->prox = NULL;

    if (!f->prim) {
        f->prim = ins;
    } else {
        f->fim->prox = ins;
    }
    f->fim = ins;

    return ++f->num;
}

// Retira o primeiro item da fila e o devolve.
// Retorna um ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fila_retira (struct fila_t *f)
{
    if (!f || !f->prim)
        return NULL;

    void *item_ret = f->prim->item;

    struct fila_nodo_t *aux = f->prim;
    f->prim = aux->prox;
    free(aux);

    if (!f->prim)
        f->fim = NULL;

    f->num--;
    return item_ret;
}

// Informa o numero de itens na fila.
// Retorna N >= 0 ou -1 em caso de erro.
int fila_tamanho (struct fila_t *f)
{
    if (!f)
        return -1;

    return f->num;
}

// Imprime o conteudo da fila.
void fila_imprime (struct fila_t *f)
{
    if(!f)
        return;

    struct fila_nodo_t *aux = f->prim;

    while (aux) {
        if (aux != f->prim)
            printf(" ");

        printf(" %p", aux->item);
        aux = aux->prox;
    }
}