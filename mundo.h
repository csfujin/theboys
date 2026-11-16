#ifndef MUNDO
#define MUNDO

#include "auxiliares.h"
#include "lef.h"

// Inicializacao do mundo virtual
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 5)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)
#define N_INVASOES (T_FIM_DO_MUNDO / 10000)

struct estatisticas
{
    int ev_tratados;               // Eventos totais concluidos
    int m_cumpridas;               // Missoes totais concluids      
    int invasoes;                  // Invasoes realizadas
    int mortes_totais;             // Total de herois mortos
    int maior_fila[N_BASES];       // Max de herois que ficaram na fila simultaneamente em cada base
    int missoes_base[N_BASES];     // Missoes cumpridas de cada base
    int tentativas[N_MISSOES];     // Tentativas de cada missao
};

extern struct estatisticas est;

struct mundo
{
    int num_herois;                       // Numero total de herois no mundo
    struct heroi **herois;                // Vetor representando todos os herois

    int num_bases;                        // Numero total de bases no mundo
    struct base **bases;                  // Vetor representando todas as bases

    int num_missoes;                      // Numero total de missoes a cumprir
    struct missao **missoes;              // Vetor representando todas as missoes

    int num_invasoes;                     // Numero total de invasoes
    struct invasao **invasoes;            // Vetor representando todas as invasoes

    int num_habilidades;                  // Numero de habilidades distintas possiveis
    
    struct coordenadas tamanho_mundo;     // Coordenadas maximas do plano cartesiano 
                                          
    int relogio;                          // Indica o tempo atual do mundo.

    struct lef_t *lef;                    // Lista de eventos futuros para a simulação.
};

// Inicializa o mundo e seus conteudos (herois, bases, missoes, invasoes, etc.). Retorna a struct mundo.
struct mundo *ini_mundo ();

// Loop de simulacao do mundo e eventos.
void simula_mundo (struct mundo *w);

// Destroi o mundo e seus conteudos (herois, bases, missoes, invasoes, etc.)
void destroi_mundo (struct mundo *w);

#endif  