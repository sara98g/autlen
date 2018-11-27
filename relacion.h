#ifndef RELACION_H
#define RELACION_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _Relacion{
   char* nombre;
   int num_elementos;
   int ** relacion;
   int ** cierre_relacion;

} Relacion;

Relacion * relacionNueva(char * nombre, int num_elementos);

void relacionElimina(Relacion * p_r);

Relacion * relacionInserta(Relacion * p_r, int i, int j);

void relacionImprime(FILE * fd, Relacion * p_r);

Relacion * relacionCierreTransitivo(Relacion * p_r);

int ** multiplicaMatricesBinarias(int **matriz_potencia, int**matriz_inicial,int tam);

#endif
