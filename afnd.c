#include "afnd.h"


AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos){

    assert(nombre != NULL);
    assert(num_estados > 0);
    assert(num_simbolos > 0);
    AFND* afnd = (AFND*)malloc(sizeof(AFND));
    int i, j;

    afnd->nombre = (char*)malloc(sizeof(char)*(strlen(nombre)+1));
    strcpy(afnd->nombre, nombre);

    afnd->num_estados = num_estados;

    afnd->estados = (Estado**)malloc(sizeof(Estado*)*num_estados);
    for(i=0 ; i< num_estados ; i++)
        afnd->estados[i] = NULL;

    afnd->num_simbolos = num_simbolos;

    afnd->alfabeto = alfabetoNuevo("A", num_simbolos);

    afnd->palabra = palabraNueva();

    afnd->ftransicion = (VectorIndices**) malloc (num_estados*sizeof(VectorIndices*));
	for (i=0; i<num_estados; i++)
	{
		afnd->ftransicion[i] = (VectorIndices*) malloc (num_simbolos*sizeof(VectorIndices));
	}

	for (i=0; i < num_estados ; i++)
	{
		for (j=0; j < num_simbolos; j++)
		{
			afnd->ftransicion[i][j]= VectorIndicesNuevo(num_estados);
		}
	}

	afnd->estados_actual = VectorIndicesNuevo(num_estados);
	afnd->transicionlambda = relacionNueva("RL++*", num_estados);


    return afnd;
}


void AFNDElimina(AFND * p_afnd){

    assert(p_afnd != NULL);

    int i, j;

    for(i = 0; i < p_afnd->num_estados; i++){
        estadoElimina(p_afnd->estados[i]);
    }
    free(p_afnd->estados);
    alfabetoElimina(p_afnd->alfabeto);
    palabraElimina(p_afnd->palabra);
    for(i = 0; i < p_afnd->num_estados; i++){
        for(j = 0; j < p_afnd->num_simbolos; j++)
            VectorIndicesElimina(p_afnd->ftransicion[i][j]);
        free(p_afnd->ftransicion[i]);
    }

    free(p_afnd->ftransicion);
    VectorIndicesElimina(p_afnd->estados_actual);

    free(p_afnd->nombre);
    relacionElimina(p_afnd->transicionlambda);
    free(p_afnd);
}


void AFNDImprime(FILE * fd, AFND* p_afnd){

    assert(p_afnd != NULL);
    assert(fd != NULL);

    int i, j, k;
    fprintf(fd, "%s={\n", p_afnd->nombre);
    fprintf(fd, "\tnum_simbolos = %d\n\t", p_afnd->num_simbolos);
    alfabetoImprime(fd, p_afnd->alfabeto);
    fprintf(fd, "\tnum_estado = %d\n\tQ={", p_afnd->num_estados);

    for(i = 0; i < p_afnd->num_estados; i++){
        estadoImprime(fd, p_afnd->estados[i]);
    }
    fprintf(fd, "}\n");
    relacionImprime(fd, p_afnd->transicionlambda);
    fprintf(fd, "\tFuncion de Transicion = {\n");
    for(i = 0; i < p_afnd->num_estados; i++){
        for(j = 0; j < p_afnd->num_simbolos; j++){
            fprintf(fd, "\t\tf(%s,%s)={ ", estadoNombre(p_afnd->estados[i]), alfabetoSimboloEn(p_afnd->alfabeto, j));
            for(k = 0; k < p_afnd->num_estados; k++){
                if((p_afnd->ftransicion[i][j])[k] == 1){
                    fprintf(fd, "%s ",  estadoNombre(p_afnd->estados[k]));
                }
            }
            fprintf(fd, "}\n");
        }
    }
    fprintf(fd, "\t}\n");
    fprintf(fd, "}\n");

}


AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo){

    assert(p_afnd != NULL);
    assert(simbolo != NULL);

    p_afnd->alfabeto = alfabetoInsertaSimbolo(p_afnd->alfabeto, simbolo);
    return p_afnd;
}

AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo){

    assert(p_afnd != NULL);
    assert(nombre != NULL);
    assert(tipo > -1 && tipo < 4);

    int i;

    for(i = 0; i < p_afnd->num_estados; i++){
        if(p_afnd->estados[i] == NULL){
           p_afnd->estados[i] = estadoNuevo( nombre, tipo);
           break;
        }
    }

    return p_afnd;
}


AFND * AFNDInsertaTransicion(AFND * p_afnd,  char * nombre_estado_i, char * nombre_simbolo_entrada,  char * nombre_estado_f ){

    assert(p_afnd != NULL);
    assert(nombre_estado_i != NULL);
    assert(nombre_estado_f != NULL);
    assert(nombre_simbolo_entrada != NULL);

    int pos_estado_i, pos_estado_f, pos_simbolo;

   pos_estado_i = AFNDIndiceDeEstado(p_afnd, nombre_estado_i);
   pos_estado_f = AFNDIndiceDeEstado(p_afnd, nombre_estado_f);
   pos_simbolo = AFNDIndiceDeSimbolo(p_afnd, nombre_simbolo_entrada);

   (p_afnd->ftransicion[pos_estado_i][pos_simbolo])[pos_estado_f]=1;

   return p_afnd;
}


AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra){

    assert(p_afnd != NULL);
    assert(letra != NULL);

    p_afnd->palabra = palabraInsertaLetra(p_afnd->palabra, letra);

    return p_afnd;
}


void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd){

    assert(fd != NULL);
    assert(p_afnd != NULL);

    int i;
    fprintf(fd, "ACTUALMENTE EN {");
    for(i = 0; i < p_afnd->num_estados; i++){
        if(p_afnd->estados_actual[i] == 1){
            estadoImprime(fd, p_afnd->estados[i]);
        }

    }
    fprintf(fd, "}\n");;
}

void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd){

    assert(p_afnd != NULL);
    assert(fd != NULL);

    palabraImprime(fd, p_afnd->palabra);
}

AFND * AFNDInicializaEstado (AFND * p_afnd){

    assert(p_afnd != NULL);

    int i, j;
    /*Eliminamos posibles estados actuales de otra cadena*/
    for(i = 0; i < p_afnd->num_estados; i++)
        VectorIndicesUnsetI(p_afnd->estados_actual, i);

    for(i = 0; i < p_afnd->num_estados; i++){
        if(estadoTipo(p_afnd->estados[i]) == INICIAL || estadoTipo(p_afnd->estados[i]) == INICIAL_Y_FINAL){
            VectorIndicesSetI(p_afnd->estados_actual, i);
            for(j = 0; j < p_afnd->num_estados; j++){
                if(p_afnd->transicionlambda->cierre_relacion[i][j] == 1)
                    VectorIndicesSetI(p_afnd->estados_actual, j);
            }
        }
    }

    return p_afnd;
}



void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd){

    assert(p_afnd != NULL);
    assert(fd != NULL);

    int i;

    while ( palabraTamano(p_afnd->palabra) > 0 && VectorIndicesVacio(p_afnd->estados_actual, p_afnd->num_estados) != 0){
        AFNDImprimeConjuntoEstadosActual(fd, p_afnd);
        AFNDImprimeCadenaActual(fd, p_afnd);
        fprintf(fd, "\n");
        AFNDTransita(p_afnd);
    }
    AFNDImprimeConjuntoEstadosActual(fd, p_afnd);
    AFNDImprimeCadenaActual(fd, p_afnd);

    /* Decimos si la cadena ha sido reconocida o no */
    if(VectorIndicesVacio(p_afnd->estados_actual, p_afnd->num_estados) != 0){
        for(i = 0; i < p_afnd->num_estados; i++){
            if(VectorIndicesGetI(p_afnd->estados_actual, i) == 1){
                if(estadoTipo(p_afnd->estados[i]) == FINAL || estadoTipo(p_afnd->estados[i]) == INICIAL_Y_FINAL){
                    printf("LA CADENA HA SIDO ACEPTADA (SE RECONOCE LA CADENA)\n");
                    return;
                }
            }
        }
    }
    printf("LA CADENA HA SIDO RECHAZADA (NO SE RECONOCE LA CADENA)\n");

}

void AFNDTransita(AFND * p_afnd){

    assert(p_afnd != NULL);

    char* simbolo;
    int indice_simbolo;
    int i , j, k;
    VectorIndices siguientes;

    /*estados a los que vamos a pasar tras la transicion*/
    siguientes = VectorIndicesNuevo(p_afnd->num_estados);


    simbolo = palabraQuitaInicio(p_afnd->palabra);
    indice_simbolo = AFNDIndiceDeSimbolo(p_afnd, simbolo);

    for(i = 0; i < p_afnd->num_estados; i++){
        if(p_afnd->estados_actual[i] == 1){
            for(j = 0;j < p_afnd->num_estados; j++){
                if((p_afnd->ftransicion[i][indice_simbolo])[j] == 1){
                    VectorIndicesSetI(siguientes, j);
                    for(k = 0; k < p_afnd->num_estados; k++){
                        if(p_afnd->transicionlambda->cierre_relacion[j][k] == 1)
                            VectorIndicesSetI(siguientes, k);
                    }
                }
            }

        }
    }

    for(i = 0; i < p_afnd->num_estados; i++){
        VectorIndicesUnsetI(p_afnd->estados_actual, i);
    }
    for(i = 0; i < p_afnd->num_estados; i++){
        if(siguientes[i] == 1){
            VectorIndicesSetI(p_afnd->estados_actual, i);
        }
    }

    VectorIndicesElimina(siguientes);
    free(simbolo);
}

int AFNDIndiceDeEstado(AFND * p_afnd, char* nombre_estado){

    assert(p_afnd != NULL);
    assert(nombre_estado != NULL);
    int i;
    for(i = 0; i < p_afnd->num_estados; i++)
        if(strcmp(nombre_estado, estadoNombre(p_afnd->estados[i])) == 0)
            return i;

    return -1;
}

int AFNDIndiceDeSimbolo(AFND * p_afnd, char* nombre_simbolo){

    assert(p_afnd != NULL);
    assert(nombre_simbolo != NULL);
    return alfabetoIndiceDeSimbolo(p_afnd->alfabeto, nombre_simbolo);
}

AFND * AFNDInicializaCadenaActual (AFND * p_afnd ){

    assert(p_afnd != NULL);

    palabraElimina(p_afnd->palabra);
    p_afnd->palabra = palabraNueva();

    return p_afnd;
}

AFND * AFNDInsertaLTransicion(AFND * p_afnd , char * nombre_estado_i, char * nombre_estado_f){
    assert(p_afnd != NULL);
    assert(nombre_estado_i != NULL);
    assert(nombre_estado_f != NULL);

    int pos_estado_i, pos_estado_f;

    pos_estado_i = AFNDIndiceDeEstado(p_afnd, nombre_estado_i);
    pos_estado_f = AFNDIndiceDeEstado(p_afnd, nombre_estado_f);

    p_afnd->transicionlambda = relacionInserta(p_afnd->transicionlambda, pos_estado_i, pos_estado_f);

    return p_afnd;
}

AFND *AFNDCierraLTransicion(AFND * p_afnd){
    assert(p_afnd != NULL);

    p_afnd->transicionlambda = relacionCierreTransitivo(p_afnd->transicionlambda);
    return p_afnd;
}

void AFNDADot(AFND * p_afnd){
    assert(p_afnd != NULL);

    int i,j,k;
    FILE* f;
    char*nombre = (char*)malloc(sizeof(char)*(strlen(p_afnd->nombre)+5));
    strcpy(nombre, p_afnd->nombre);
    strcat(nombre, ".dot");

    f = fopen(nombre, "w");

    fprintf(f, "digraph %s { rankdir=LR;\n", p_afnd->nombre);
    fprintf(f, "\t_invisible [style=\"invis\"];\n");
    for(i = 0; i < p_afnd->num_estados; i++){
        fprintf(f, "\t%s", estadoNombre(p_afnd->estados[i]));
        if(estadoTipo(p_afnd->estados[i]) == FINAL || estadoTipo(p_afnd->estados[i]) == INICIAL_Y_FINAL)
            fprintf(f, " [penwidth=\"2\"];\n");
        else
            fprintf(f, ";\n");

        if(estadoTipo(p_afnd->estados[i]) == INICIAL || estadoTipo(p_afnd->estados[i]) == INICIAL_Y_FINAL)
            fprintf(f, "\t_invisible -> %s ;\n",estadoNombre(p_afnd->estados[i]));
    }

     for(i = 0; i < p_afnd->num_estados; i++){
        for(j = 0; j < p_afnd->num_simbolos; j++){
            for(k = 0; k < p_afnd->num_estados; k++){
                if((p_afnd->ftransicion[i][j])[k] == 1){
                    fprintf(f, "\t%s -> %s [label=\"%s\"];\n", estadoNombre(p_afnd->estados[i]), estadoNombre(p_afnd->estados[k]), alfabetoSimboloEn(p_afnd->alfabeto, j));
                }
            }
        }
    }

     for(i = 0; i <  p_afnd->num_estados; i++){
        for(j = 0; j <  p_afnd->num_estados; j++)
            if(p_afnd->transicionlambda->relacion[i][j] == 1)
                fprintf(f, "\t%s -> %s [label=\"&lambda;\"];\n", estadoNombre(p_afnd->estados[i]), estadoNombre(p_afnd->estados[j]) );


    }

    fprintf(f, "}");
    free(nombre);
    fclose(f);


}



AFND * AFND1ODeSimbolo( char * simbolo){
    assert(simbolo != NULL);

    AFND* p_afnd = NULL;
    char *nombre = NULL;
    nombre = (char*)malloc(sizeof(char)*(strlen(simbolo)+6));
    strcpy(nombre, "afnd_");
    strcat(nombre, simbolo);

    /* Creamos el automata */
    p_afnd = AFNDNuevo(nombre, 2, 1);
    assert(p_afnd != NULL);

    /* Insertamos el simbolo de la ER */
    p_afnd = AFNDInsertaSimbolo(p_afnd, simbolo);

    /* Creamos los estados para aceptar la ER */
    p_afnd = AFNDInsertaEstado(p_afnd,"q0",INICIAL);
    p_afnd = AFNDInsertaEstado(p_afnd,"qf",FINAL);

    /* Transicion entre los estados con el simbolo proporcionado */
    p_afnd = AFNDInsertaTransicion(p_afnd, "q0", simbolo, "qf");
    
    free(nombre);
    return p_afnd;
}


AFND * AFND1ODeLambda(){

    AFND* p_afnd = NULL;
    char nombre[] = {"afnd_lambda"};
    
    /* Creamos el automata */
    p_afnd = AFNDNuevo(nombre, 1, 0);
    assert(p_afnd != NULL);

    /* Creamos los estados para aceptar la ER */
    p_afnd = AFNDInsertaEstado(p_afnd,"q0",INICIAL_Y_FINAL);

    return p_afnd;
}


AFND * AFND1ODeVacio(){

    AFND* p_afnd = NULL;
    char nombre[] = {"afnd_vacio"};

    /* Creamos el automata */
    p_afnd = AFNDNuevo(nombre, 2, 0);
    assert(p_afnd != NULL);


    /* Creamos los estados para aceptar la ER */
    p_afnd = AFNDInsertaEstado(p_afnd,"q0",INICIAL);
    p_afnd = AFNDInsertaEstado(p_afnd,"qf",FINAL);


    return p_afnd;
}


AFND * AFND1OUneLTransicion(AFND * p_afnd_destino ,AFND * p_afnd_origen, char * nombre_estado_i, char * nombre_estado_f, int offset_estados){

    int i;
    
    for(i = 0; i < p_afnd_origen->num_estados; i++){
        if(estadoTipo(p_afnd_origen->estados[i]) == INICIAL)
            p_afnd_destino = AFNDInsertaLTransicion(p_afnd_destino, nombre_estado_i, estadoNombre(p_afnd_destino->estados[i+offset_estados]));
    
        else if(estadoTipo(p_afnd_origen->estados[i]) == FINAL)
            p_afnd_destino = AFNDInsertaLTransicion(p_afnd_destino, nombre_estado_f, estadoNombre(p_afnd_destino->estados[i+offset_estados]));

        else if(estadoTipo(p_afnd_origen->estados[i]) == INICIAL_Y_FINAL){
            p_afnd_destino = AFNDInsertaLTransicion(p_afnd_destino, nombre_estado_i, estadoNombre(p_afnd_destino->estados[i+offset_estados]));
            p_afnd_destino = AFNDInsertaLTransicion(p_afnd_destino, nombre_estado_f, estadoNombre(p_afnd_destino->estados[i+offset_estados]));
            
        }
    }
    

    return p_afnd_destino;
}


AFND * AFND1OInsertaSimbolosAFND(AFND * p_afnd_destino, AFND * p_afnd_origen){

    int i;

    for(i = 0; i < p_afnd_origen->num_simbolos; i++){
        /* Comprobamos que el simbolo no fomre parte ya del alfabeto del automata */
        if(alfabetoIndiceDeSimbolo(p_afnd_destino->alfabeto, alfabetoSimboloEn(p_afnd_origen->alfabeto, i)) == -1)
            p_afnd_destino = AFNDInsertaSimbolo(p_afnd_destino, alfabetoSimboloEn(p_afnd_origen->alfabeto, i));
    }

      return p_afnd_destino;
}

AFND * AFND1OInsertaEstadosTransicionesAFND(AFND * p_afnd_destino, AFND * p_afnd_origen, char * prefijo_estados, int offset_estados){

  int i,j,k;


  
  for(i = 0; i < p_afnd_origen->num_estados; i++){
      
    char *nombre = NULL;
    nombre = (char*)malloc(sizeof(char)*(strlen(estadoNombre(p_afnd_origen->estados[i]))+strlen(prefijo_estados)+1));
    strcpy(nombre, estadoNombre(p_afnd_origen->estados[i]));
    strcat(nombre, prefijo_estados);
    p_afnd_destino = AFNDInsertaEstado(p_afnd_destino, nombre, NORMAL);
    free(nombre);
  }

    for(i = 0; i < p_afnd_origen->num_estados; i++)
        for(j = 0; j < p_afnd_origen->num_simbolos; j++)
            for(k = 0; k < p_afnd_origen->num_estados; k++)
                if((p_afnd_origen->ftransicion[i][j])[k] == 1)
                    p_afnd_destino = AFNDInsertaTransicion(p_afnd_destino, estadoNombre(p_afnd_destino->estados[i+offset_estados]), alfabetoSimboloEn(p_afnd_destino->alfabeto, j),estadoNombre(p_afnd_destino->estados[k+offset_estados]));

    for(i = 0; i < p_afnd_origen->num_estados; i++)
        for(j = 0; j < p_afnd_origen->num_estados; j++)
            if(p_afnd_origen->transicionlambda->relacion[i][j] == 1){
                p_afnd_destino = AFNDInsertaLTransicion(p_afnd_destino , estadoNombre(p_afnd_destino->estados[i+offset_estados]), estadoNombre(p_afnd_destino->estados[j+offset_estados]));
            }
    
    return p_afnd_destino; 

}


AFND * AFNDAAFND1O(AFND * p_afnd){
    
    assert(p_afnd != NULL);
    AFND* nuevo = NULL;
    
    /* Estados + 2 para el nuevo estado inicial y final*/
    char *nombre = NULL;
    nombre = (char*)malloc(sizeof(char)*(strlen(p_afnd->nombre)+8));
    strcpy(nombre, p_afnd->nombre);
    strcat(nombre, "_AFND10");
    nuevo = AFNDNuevo(nombre, p_afnd->num_estados+2, p_afnd->num_simbolos);
    assert(nuevo != NULL);
    
    
    /* Insercion de los simbolos */
    nuevo = AFND1OInsertaSimbolosAFND(nuevo, p_afnd);
    
    /* Insercion estados y transiciones */
    nuevo = AFND1OInsertaEstadosTransicionesAFND(nuevo, p_afnd, "_AFND10", 0);
    
    AFNDInsertaEstado(nuevo, "q0", INICIAL);
    AFNDInsertaEstado(nuevo, "qf", FINAL);
    
    nuevo = AFND1OUneLTransicion(nuevo, p_afnd, "q0", "qf", 0);

	AFNDCierraLTransicion(nuevo);

    free (nombre);
    return nuevo;
}


AFND * AFND1OUne(AFND * p_afnd1O_1, AFND * p_afnd1O_2){
    
    assert(p_afnd1O_1 != NULL);
    assert(p_afnd1O_2 != NULL);
    
    int  num_estados, num_simbolos;
    AFND* afnd_une = NULL;
    //+4 para meter _U_ entre medias
    char *nombre = (char*)malloc(sizeof(char)*(strlen(p_afnd1O_1->nombre)+strlen(p_afnd1O_2->nombre)+4)); 
    strcpy(nombre, p_afnd1O_1->nombre);
    strcat(nombre, "_U_");
    strcat(nombre, p_afnd1O_2->nombre);
    
    //+2 para añadir el nuevo inicial y el nuevo final
    num_estados = p_afnd1O_1->num_estados + p_afnd1O_2->num_estados + 2;
    num_simbolos = p_afnd1O_1->num_simbolos + p_afnd1O_2->num_simbolos;
    
    afnd_une = AFNDNuevo(nombre, num_estados, num_simbolos);
    
    /* Insercion de los simbolos */
    afnd_une = AFND1OInsertaSimbolosAFND(afnd_une, p_afnd1O_1);  
    afnd_une = AFND1OInsertaSimbolosAFND(afnd_une, p_afnd1O_2);  
    
    /* Insercion estados y transiciones */
    afnd_une = AFND1OInsertaEstadosTransicionesAFND(afnd_une, p_afnd1O_1, p_afnd1O_1->nombre, 0);
    afnd_une = AFND1OInsertaEstadosTransicionesAFND(afnd_une, p_afnd1O_2, p_afnd1O_2->nombre, p_afnd1O_1->num_estados);

    
    AFNDInsertaEstado(afnd_une, "q0", INICIAL);
    AFNDInsertaEstado(afnd_une, "qf", FINAL);
    
    afnd_une = AFND1OUneLTransicion(afnd_une, p_afnd1O_1, "q0", "qf", 0);
    afnd_une = AFND1OUneLTransicion(afnd_une, p_afnd1O_2, "q0", "qf", p_afnd1O_1->num_estados);

    AFNDCierraLTransicion(afnd_une);
    
    
    free(nombre);
    
    return afnd_une;
}

AFND * AFND1OConcatena(AFND * p_afnd_origen1, AFND * p_afnd_origen2){
    
    return NULL;
}

AFND * AFND1OEstrella(AFND * p_afnd_origen){
    return NULL;
}
