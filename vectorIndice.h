#ifndef VECTORINDICE_H
#define VECTORINDICE_H


#include <stdio.h>
#include <stdlib.h>
#include "assert.h"


typedef int* VectorIndices;

VectorIndices VectorIndicesNuevo(int tamano);
void VectorIndicesElimina(VectorIndices vi);
void VectorIndicesImprime(FILE *fd,VectorIndices vi,int tam);
void VectorIndicesSetI(VectorIndices estados,int i);
void VectorIndicesUnsetI(VectorIndices estados,int i);
int VectorIndicesGetI(VectorIndices estados,int i);
int VectorIndicesVacio(VectorIndices estados, int i);

#endif
