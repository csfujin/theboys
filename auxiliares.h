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

int aleatorio(int min, int max);

int dist_cartesiana(struct coordenadas, struct coordenadas);

#endif