#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "alfabeto.h"



Alfabeto* alfabetoNuevo(char * nombre, int tamano){
    
    assert ( nombre != NULL );
    assert ( strlen( nombre ) > 0 );
    
    Alfabeto* alfabeto=NULL;
    
    alfabeto         = (Alfabeto*)malloc(sizeof(Alfabeto));
    
    alfabeto->nombre = (char*)malloc(sizeof(char)*(strlen(nombre)+1));
    
    strcpy(alfabeto->nombre, nombre);
    alfabeto->tamano=0;
    alfabeto->maximo=tamano;
    alfabeto->simbolos=(char**)malloc(sizeof(char*)*tamano);
 
    
    return alfabeto;
    
}

void alfabetoElimina(Alfabeto* p_alfabeto){

    assert ( p_alfabeto != NULL );
    
    int i=0;
    
    free(p_alfabeto->nombre);
    
    for(i=0 ; i< p_alfabeto->tamano ;i++){
        free(p_alfabeto->simbolos[i]);          /*Error de memoria 1*/
    }
    
    free(p_alfabeto->simbolos);
    free(p_alfabeto);
    
    return;
    
}
/*Guarda en la colección de símbolos del alfabeto una copia en memoria nueva del argumento propocionado 
*/
Alfabeto * alfabetoInsertaSimbolo(Alfabeto * p_alfabeto, char * simbolo){
    
    assert ( p_alfabeto != NULL );
    assert ( simbolo != NULL );
    assert ( strlen( simbolo ) > 0 );
    assert ( p_alfabeto->tamano < p_alfabeto->maximo );
    
    p_alfabeto->simbolos[p_alfabeto->tamano] = (char*)malloc(sizeof(char)*(strlen(simbolo)+1));
    strcpy(p_alfabeto->simbolos[p_alfabeto->tamano], simbolo);
    p_alfabeto->tamano++;
    
    return p_alfabeto;
}

void alfabetoImprime(FILE * fd, Alfabeto * p_alf){
    
    assert ( fd != NULL );
    assert ( p_alf != NULL );
    
    int i = 0;
    
    fprintf(fd,"%s={ ",p_alf->nombre);
    
    for(i = 0 ; i < p_alf->tamano ; i++)
        fprintf(fd, "%s ",p_alf->simbolos[i]);
    
    fprintf(fd,"}\n");
    return;
}
/*
Devuelve el símbolo que está en la posición pasada como argumento en el alfabeto proporcionado como argumento.
*/
char* alfabetoSimboloEn(Alfabeto * p_alf, int i){
    
    assert ( p_alf != NULL );
    assert ( i < p_alf->tamano );
    
    return p_alf->simbolos[i];
}
/*Se devuelve la posición en la que está el alfabeto 
*/
int alfabetoIndiceDeSimbolo(Alfabeto * p_alf, char * simbolo){
    
    assert ( p_alf != NULL );
    assert ( simbolo != NULL );
    assert ( strlen( simbolo ) > 0 );
    
    int i =0;
    
    for(i=0 ; i< p_alf->tamano ; i++){
        if(strcmp(simbolo, p_alf->simbolos[i]) == 0)
            return i;
    }
        
    return -1;
    
}
