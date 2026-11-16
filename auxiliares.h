#ifndef AUXILIARES
#define AUXILIARES

#include <stdio.h>
#include <stdlib.h>

// Coordenadas no plano cartesiano (x, y).
struct coordenadas
{
    int x;
    int y;
};

// Gera um numero aleatorio entre (x, y).
int aleatorio(int min, int max);

// Calcula a distancia cartesiana entre duas coordenadas.
int dist_cartesiana(struct coordenadas, struct coordenadas);

#endif