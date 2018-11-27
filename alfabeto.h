#ifndef ALFABETO_H
#define ALFABETO_H

typedef struct _Alfabeto{
   char* nombre;
   int tamano;
   int maximo;
   char** simbolos;
} Alfabeto;


Alfabeto * alfabetoNuevo(char * nombre, int tamano);

void alfabetoElimina(Alfabeto * p_alfabeto);

Alfabeto * alfabetoInsertaSimbolo(Alfabeto * p_alfabeto, char * simbolo);

void alfabetoImprime(FILE * fd, Alfabeto * p_alf);
char* alfabetoSimboloEn(Alfabeto * p_alf, int i);
int alfabetoIndiceDeSimbolo(Alfabeto * p_alf, char * simbolo);

#endif
