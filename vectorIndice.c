#include "vectorIndice.h"


VectorIndices VectorIndicesNuevo(int tamano){
    
    assert(tamano > 0);
    
    int i;
    VectorIndices vector = (int*)malloc(sizeof(int)*tamano);
    for(i = 0; i < tamano; i++){
        vector[i] = 0;
    }
    return vector;
    
}

void VectorIndicesElimina(VectorIndices vi){
    
    assert(vi != NULL);

    free(vi);
}

void VectorIndicesImprime(FILE *fd,VectorIndices vi,int tam){
    
    assert(vi != NULL);
    assert(fd != NULL);
    assert(tam > -1);
    
    int i;
    for(i=0 ; i< tam ; i++){
        fprintf(fd, "%d ", vi[i]);
	}

	fprintf(fd, "\n");
    return;
}

void VectorIndicesSetI(VectorIndices estados,int i){
    
    assert(estados != NULL);
    assert(i > -1);
    
    estados[i] = 1;
}

void VectorIndicesUnsetI(VectorIndices estados,int i){
    assert(estados != NULL);
    assert(i > -1);
    
    estados[i] = 0;
}

int VectorIndicesGetI(VectorIndices estados,int i){
    assert(estados != NULL);
    assert(i > -1);
    
    return estados[i];
}


int VectorIndicesVacio(VectorIndices estados, int i){
    
    assert(estados != NULL);
    assert(i > -1);
    
    int j, cont = 0;
    for(j = 0; j < i; j++){
        if(estados[j] == 1)
            cont++;
    }
    return cont;
}
