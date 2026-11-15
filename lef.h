// Lista de Eventos Futuros (LEF)
// Carlos Maziero, DINF/UFPR, Out 2024
// Definição da interface

// NÃO ALTERAR
#ifndef LEF
#define LEF

// descreve um nodo da fila de prioridades
struct lefnodo_t
{
	void *item ;			// item associado ao nodo
	int   tipo ;			// tipo do item
	int   prio ;			// prioridade do item
	struct lefnodo_t *prox;	// próximo nodo
};

// descreve uma fila de prioridades
struct lef_t
{
	struct lefnodo_t *prim ;	// primeiro nodo da fila
	struct lefnodo_t *fim;		// último nodo da fila
	int num ;			// número de itens na fila
} ;

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct lef_t *lef_cria ();

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct lef_t *lef_destroi (struct lef_t *f);

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int lef_insere (struct lef_t *f, void *item, int tipo, int prio);

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *lef_retira (struct lef_t *f, int *tipo, int *prio);

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int lef_tamanho (struct lef_t *f);

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void lef_imprime (struct lef_t *f);

// Retira os eventos da LEF relacionados ao heroi
// tipo: -1 remove todos os eventos do heroi
void lef_retira_heroi (struct lef_t *f, int id_h, int tipo);

#endif
