#include "relacion.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


Relacion * relacionNueva(char * nombre, int num_elementos){
    Relacion * relacion;
    int i, j;
    assert(nombre != NULL);
    assert(num_elementos>0);
    relacion = (Relacion*)malloc(sizeof(Relacion));
    
 
    relacion->nombre = (char*)malloc((strlen(nombre)+1)*sizeof(char));
    strcpy( relacion->nombre, nombre);
    assert(relacion->nombre != NULL);
    
    relacion->relacion = (int**) malloc (num_elementos *sizeof(int*));
    assert(relacion->relacion != NULL);
    
    relacion->cierre_relacion = (int**) malloc (num_elementos *sizeof(int*));
    assert(relacion->cierre_relacion != NULL);
    
    relacion->num_elementos = num_elementos;
    
    for (i=0; i<num_elementos; i++)
    {
            relacion->relacion[i] = (int*) malloc (num_elementos*sizeof(int));
            relacion->cierre_relacion[i] = (int*) malloc (num_elementos*sizeof(int));
    }

    for (i=0; i < num_elementos ; i++)
    {
            for (j=0; j < num_elementos; j++)
            {
                relacion->relacion[i][j]=0;
                relacion->cierre_relacion[i][j]=0;
            }
    }    
    
    return relacion;
}

void relacionElimina(Relacion * p_r){
    
    assert(p_r != NULL);
    int i;
    
    for (i=0; i<p_r->num_elementos; i++){
        free (p_r->relacion[i]);
        free(p_r->cierre_relacion[i]);
    }
    
    free(p_r->relacion);
    free(p_r->cierre_relacion);
    free(p_r->nombre);
    free(p_r);

}

Relacion * relacionInserta(Relacion * p_r, int i, int j){
    assert(p_r != NULL);
    assert(i > -1 && i < p_r->num_elementos);
    assert(j > -1 && j < p_r->num_elementos);
    
    p_r->relacion[i][j] = 1;
    p_r->cierre_relacion[i][j] = 1;

    return p_r;
}

void relacionImprime(FILE * fd, Relacion * p_r){
    
    int i, j;
    assert(fd != NULL);
    assert(p_r != NULL);
    
    fprintf(fd, "\t%s={\n\t\t\t", p_r->nombre);
    for(i = 0; i < p_r->num_elementos; i++)
        fprintf(fd, "[%d]\t",i);
        
    fprintf(fd, "\n");
    fprintf(fd, "\t\tRELACION INICIAL\n");
    
    for(i = 0; i < p_r->num_elementos; i++){
        fprintf(fd, "\t\t[%d]",i);
        for(j = 0; j < p_r->num_elementos; j++)
            fprintf(fd, "\t%d", p_r->relacion[i][j]);
            
        fprintf(fd, "\n");
        
    }
    fprintf(fd, "\n");
    fprintf(fd, "\t\tCIERRE\n");
    for(i = 0; i < p_r->num_elementos; i++){
        fprintf(fd, "\t\t[%d]",i);
        for(j = 0; j < p_r->num_elementos; j++)
            fprintf(fd, "\t%d", p_r->cierre_relacion[i][j]);
            
        fprintf(fd, "\n");
    }
    fprintf(fd, "}\n\n");
}

Relacion * relacionCierreTransitivo(Relacion * p_r){
    assert(p_r!= NULL);
    
    int i;
    
    for(i = 0; i < p_r->num_elementos; i++){
       p_r->cierre_relacion =multiplicaMatricesBinarias(p_r->cierre_relacion, p_r->relacion, p_r->num_elementos);
    }
    

   
   return p_r;
}

int ** multiplicaMatricesBinarias(int **matriz_potencia, int **matriz_inicial,int tam){
    assert(matriz_potencia!= NULL);
    assert(matriz_inicial!=NULL);
    assert(tam > 0);
    
    int i,j, contador = 0, fila=0, columna=0;
    int** temporal;
    temporal = (int**) malloc (tam *sizeof(int*));
    
    for (i=0; i<tam; i++)
        temporal[i] = (int*) malloc (tam*sizeof(int));
    
    
    for(i=0; i<tam; i++)
        for(j=0; j<tam; j++)
            temporal[i][j] = matriz_potencia[i][j];
        
    

    
    for(fila = 0; fila < tam; fila++){
        for(i = 0; i < tam; i++){
            contador = 0;
            for(columna = 0; columna < tam; columna++){
                if(matriz_inicial[fila][columna] == 1 && temporal[columna][i]== 1)
                    contador++;
            }
            if(contador > 0)
                matriz_potencia[fila][i] |= 1;
            else
                matriz_potencia[fila][i] |=0;
        }
    }
    
    for (i=0; i<tam; i++){
        free (temporal[i]);
    }
    
    free(temporal);
    return matriz_potencia;
    
}

