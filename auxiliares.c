#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "auxiliares.h"

// Gera um numero aleatorio entre (x, y).
int aleatorio(int min, int max)
{
    return min + rand() % (max - min + 1);
}

// Calcula a distancia cartesiana entre duas coordenadas
int dist_cartesiana(struct coordenadas c1, struct coordenadas c2)
{
    double dx = c2.x - c1.x;
    double dy = c2.y - c1.y;

    return (int) sqrt(dx * dx + dy * dy);
}
