#ifndef PALABRA_H
#define PALABRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"


typedef struct _Palabra{
    int tamano;
    char** letras;
}Palabra;

Palabra * palabraNueva();

void palabraElimina(Palabra * p_p);

Palabra * palabraInsertaLetra(Palabra * p_p, char * letra);

void palabraImprime(FILE * fd, Palabra * p_p);

char * palabraQuitaInicio(Palabra * p_p);

int palabraTamano(Palabra * p_p);

#endif
