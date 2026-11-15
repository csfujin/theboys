#ifndef ENTIDADES
#define ENTIDADES

#include "conjunto.h"
#include "fila.h"
#include "auxiliares.h"

// Representa o herói, suas habilidades e caracteristicas.
struct heroi
{
    int id;                               // Identificador do heroi
    struct cjto_t *habilidades;           // Conjunto de habilidades que o heroi possui
    int estado;                           // Estado do heroi (Ativo ou inativo (1 ou 0))
    int paciencia;                        // Afeta decisoes de permanencia em bases e filas
    int vel;                              // Velocidade de deslocamento
    int exp;                              // Numero de missoes que o heroi ja participou
    int base;                             // Id da base onde o heroi esta
};

// Representa a base e suas caracteristicas.
struct base
{
    int id;                               // Identificador da base
    int lotacao;                          // Numero maximo de herois na base
    struct cjto_t *presentes;             // Conjunto de IDs dos herois na base atualmente
    int estado;                           // Indica o estado da base (ativa ou inativa (1 ou 0))
    struct fila_t *fila_espera;           // Fila onde os herois esperam para entrar na base
    struct coordenadas local;             // Localizacao da base
};

// Representa cada missao, habilidades necessarias e caracteristicas.
struct missao
{
    int id;                               // Identificador da missao
    struct cjto_t *habilidades;           // Conjunto de habilidades necessarias para a missao
    int perigo;                           // Nivel de perigo da missao
    struct coordenadas local;             // Localizacao da base
};

// Representa cada invasao, tempos de penalidade e caracteristicas.
struct invasao
{
    int id;                               // Identificador da invasao
    int id_base;                          // Identificador da base onde ocorrera a invasao
    int t_penalidade_b;                   // Tempo que a base permanece inativa apos uma invasao
    int t_penalidade_h;                   // Tempo que os herois permanecem inativos apos uma invasao
};


#endif