
#include "estado.h"

Estado * estadoNuevo( char * nombre, int tipo){
    
    assert(nombre != NULL);
    assert(tipo > -1 && tipo < 4);
    
    Estado* estado = (Estado*)malloc(sizeof(Estado));
    
    estado->nombre = (char*)malloc((strlen(nombre)+1)*sizeof(char));
    strcpy(estado->nombre, nombre);
    estado->tipo = tipo;
    
    return estado;

}

void estadoElimina( Estado * p_s){
    
    assert(p_s != NULL);
    
    free(p_s->nombre);
    free(p_s);
}

void estadoImprime( FILE * fd, Estado * p_s){
    
    assert(p_s != NULL);
    assert(fd != NULL);
    
    if(p_s->tipo == INICIAL)
        fprintf(fd, "->%s ", p_s->nombre);
        
        
    else if (p_s->tipo == INICIAL_Y_FINAL)
        fprintf(fd, "->%s* ", p_s->nombre);
        
    else if (p_s->tipo == FINAL)
        fprintf(fd, "%s* ", p_s->nombre);
        
    else
        fprintf(fd, "%s ", p_s->nombre);
    
}

int estadoEs(Estado * p_s, char * nombre){
    
    assert(p_s != NULL);
    assert(nombre != NULL);
    
    if(p_s == NULL || nombre  == NULL)
        return 0;
    if(strcmp(p_s->nombre, nombre) == 0)
        return 1;
    
    else
        return 0;
}

char * estadoNombre(Estado * p_s){
    
    assert(p_s != NULL);
    
    return p_s->nombre;
}

int estadoTipo(Estado * p_s){
    
    assert(p_s != NULL);
    
    return p_s->tipo;
}
