#ifndef AFND_H
#define AFND_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alfabeto.h"
#include "estado.h"
#include "palabra.h"
#include "vectorIndice.h"
#include "assert.h"
#include "relacion.h"

typedef struct _AFND{
    char* nombre;
    int num_simbolos;
    int num_estados;
    Estado** estados;
    VectorIndices estados_actual;
    Alfabeto* alfabeto; /*simbolos*/
    Palabra* palabra; /*cadena que se esta procesando*/
    VectorIndices** ftransicion;
    Relacion* transicionlambda;


}AFND;



AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos);

void AFNDElimina(AFND * p_afnd);

void AFNDImprime(FILE * fd, AFND* p_afnd);

AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo);

AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo);

AFND * AFNDInsertaTransicion(AFND * p_afnd,
                             char * nombre_estado_i,
                             char * nombre_simbolo_entrada,
                             char * nombre_estado_f );

AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra);

void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd);

void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd);

AFND * AFNDInicializaEstado (AFND * p_afnd);

void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd);

void AFNDTransita(AFND * p_afnd);

int AFNDIndiceDeEstado(AFND * p_afnd, char* nombre_estado);

int AFNDIndiceDeSimbolo(AFND * p_afnd, char* nombre_simbolo);

AFND * AFNDInicializaCadenaActual (AFND * p_afnd );

AFND * AFNDInsertaLTransicion(AFND * p_afnd , char * nombre_estado_i, char * nombre_estado_f);

AFND *AFNDCierraLTransicion(AFND * p_afnd);

void AFNDADot(AFND * p_afnd);

AFND * AFND1ODeSimbolo( char * simbolo);

AFND * AFND1ODeLambda();

AFND * AFND1ODeVacio();

AFND * AFNDAAFND1O(AFND * p_afnd);

AFND * AFND1OUne(AFND * p_afnd1O_1, AFND * p_afnd1O_2);

AFND * AFND1OConcatena(AFND * p_afnd1O_1, AFND * p_afnd1O_2);

AFND * AFND1OEstrella(AFND * p_afnd_origen);

AFND * AFND1OUneLTransicion(AFND * p_afnd_destino ,AFND * p_afnd_origen, char * nombre_estado_i, char * nombre_estado_f, int offset_estados);

AFND * AFND1OInsertaSimbolosAFND(AFND * p_afnd_destino, AFND * p_afnd_origen);

AFND * AFND1OInsertaEstadosTransicionesAFND(AFND * p_afnd_destino, AFND * p_afnd_origen, char * prefijo_estados, int offset_estados);

AFND * AFND1OConcatenaLTransicion(AFND * p_afnd_destino, AFND * p_afnd_1 ,AFND * p_afnd_2, char * nombre_estado_i, char * nombre_estado_f);

void AFND10_CalculaNumSimbolos(AFND* p_afnd_1, AFND* p_afnd_2, int* num);


#endif
