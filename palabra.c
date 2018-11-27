#include "palabra.h"

Palabra * palabraNueva(){
    Palabra* palabra;

    palabra = (Palabra*)malloc(sizeof(Palabra));
    
    palabra->tamano = 0;
    palabra->letras = (char**)malloc(sizeof(char*));
    
    return palabra;

}

void palabraElimina(Palabra * p_p){
    
    assert(p_p != NULL);
    
    int i;
    for(i = 0; i < p_p->tamano; i++)
        free(p_p->letras[i]);
    
    free(p_p->letras);
    free(p_p);
}


Palabra * palabraInsertaLetra(Palabra * p_p, char * letra){
    
    assert(p_p != NULL);
    assert(letra != NULL);
    
    p_p->tamano ++;
    p_p->letras = (char**)realloc(p_p->letras, (p_p->tamano) * sizeof(char*));
    p_p->letras[p_p->tamano-1] = (char*)malloc((strlen(letra) + 1) * sizeof(char));
    strcpy( p_p->letras[p_p->tamano-1], letra);
    
    return p_p;
    
}

void palabraImprime(FILE * fd, Palabra * p_p){
    
    assert(p_p != NULL);
    assert(fd != NULL);
    
    int i;
    
    fprintf(fd, "[ (%d) ", p_p->tamano);
    for(i = 0; i < p_p->tamano; i++){
        fprintf(fd, " %s ", p_p->letras[i]);
    }
    fprintf(fd, "]\n");
    
}

char * palabraQuitaInicio(Palabra * p_p){
    
    assert(p_p != NULL);
    
    char* inicio;
    int i;
    
    if(p_p->tamano == 0)
        return NULL;
        
    inicio = p_p->letras[0];
    
    for(i = 1; i < p_p->tamano; i++)
        p_p->letras[i-1] = p_p->letras[i];
        
    p_p->tamano--;
    p_p->letras = (char**)realloc(p_p->letras, (p_p->tamano) * sizeof(char*));
    
    return inicio;
    
}

int palabraTamano(Palabra * p_p){
    
    assert(p_p != NULL);
    
    return p_p->tamano;
}

