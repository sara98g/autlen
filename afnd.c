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





/*****************INTENTO DEL DIA 24/11/2018******************/
/*****************HAY QUE REVISAR LOS INDICES DE ESTADOS Y SIMBOLOS********************/


/*****************ESTAS DEBERIAN ESTAE BIEN*******************/
AFND * AFND1ODeSimbolo( char * simbolo){
    assert(simbolos != NULL);
    
    AFND* p_afnd = NULL;
    
    /* Creamos el automata */
    p_afnd = AFNDNuevo(strcat("anfd_", simbolo), 2, 1);
    assert(p_afnd != NULL);
    
    /* Insertamos el simbolo de la ER */
    p_afnd = FNDInsertaSimbolo(p_afnd, simbolo);
    
    /* Creamos los estados para aceptar la ER */
    p_afnd = AFNDInsertaEstado(p_afnd,"q0",INICIAL);
    p_afnd = AFNDInsertaEstado(p_afnd,"qf",FINAL);
    
    /* Transicion entre los estados con el simbolo proporcionado */
    p_afnd = AFNDInsertaTransicion(p_afnd, "q0", simbolo, "qf");
    
    return p_afnd;
}


AFND * AFND1ODeLambda(){
    
    AFND* p_afnd = NULL;
    
    /* Creamos el automata */
    p_afnd = AFNDNuevo(strcat("anfd_", "lambda"), 2, 0);
    assert(p_afnd != NULL);
    
    
    /* Creamos los estados para aceptar la ER */
    p_afnd = AFNDInsertaEstado(p_afnd,"q0",INICIAL);
    p_afnd = AFNDInsertaEstado(p_afnd,"qf",FINAL);
    
    /* Transicion lambda entre los estados */
    p_afnd = AFND * AFNDInsertaLTransicion(p_afnd , "q0", "qf");
    
    
    return p_afnd;
}


AFND * AFND1ODeVacio(){
    
    AFND* p_afnd = NULL;
    
    /* Creamos el automata */
    p_afnd = AFNDNuevo(strcat("anfd_", "vacio"), 2, 0);
    assert(p_afnd != NULL);

    
    /* Creamos los estados para aceptar la ER */
    p_afnd = AFNDInsertaEstado(p_afnd,"q0",INICIAL);
    p_afnd = AFNDInsertaEstado(p_afnd,"qf",FINAL);
    
    
    return p_afnd;
}
/****************A PARTIR DE AQUI TODO HAY QUE REVISAR LOS PROBLEMAS********************/
/**********************************************************************************/


/***** FUNCIONES PARA FACILITAR VISUALIZACION *****/

/* Funcion que enlaza el nuevo estado inicial o final con los viejos */
void AFNDInicialFinalLambda(AFND* p_afnd, char * estado_ini, char* estado_fin){
    
    
    p_afnd = AFNDInsertaEstado(p_afnd, estado_fin, NORMAL);
    p_afnd = AFNDInsertaLTransicion(p_afnd, estado_ini, estado_fin);
    
            
}
/* El auxiliar para suplir la diferencia de numero de simbolos entre un automata y otro */
void AFNDCopiaSimbolos(AFND* nuevo, AFND* copia, int aux){
    
    int i;
    
    for(i = 0; i < copia->num_simbolos; i++)
        nuevo = AFNDInsertaSimbolo(nuevo, alfabetoSimboloEn(copia->alfabeto, i));
} 

/* El auxiliar para suplir la diferencia de numero de estados entre un automata y otro */
/* EL PROBLEMA ES QUE SE PASA EL NOMBRE DE LOS ESTADOS, NO LOS INDICES */
void AFNDCopiaTransiciones(AFND* nuevo, AFND* copia, int pos_estado_i, int aux_estados, int aux_simbolos){
    int j, k;
    
    for(j = 0; j < copia->num_simbolos; j++)
        for(k = 0; k < copia->num_estados; k++)
            if((copia->ftransicion[i][j])[k] == 1)
                nuevo = AFNDInsertaTransicion(nuevo, estadoNombre(copia->estados[i]) + aux_estados, alfabetoSimboloEn(copia->alfabeto, j),estadoNombre(copia->estados[k]));

}


/**************************************************/

/*** HABRIA QUE VER COMO HACERLA MEJOR ***/
AFND * AFNDAAFND1O(AFND * p_afnd){
    
    assert(p_afnd != NULL);
    AFND* nuevo_inicial = NULL;
    AFND* nuevo_final = NULL;
    
    int i, j, k, contador_ini = 0, contador_fin = 0;
    
    /* Comprobamos que solo tenga un estado inicial y uno final (diferentes) */
    for(i = 0; i < p_afnd->num_estados; i++){
        
        /*** Debemos suponer que el automata recibido no posee estado INICIAL_Y_FINAL? ***/

        if(estadoTipo(p_afnd->estados[i][j]) == INICIAL)
            contador_ini++;
            
        if(estadoTipo(p_afnd->estados[i][j]) == FINAL)
            contador_fin++;
    }
    
    /* Si hay mas de un estado inicial y final, creamos los dos nuevos*/
    if(contador_ini > 0 && contador_fin > 0){
        
        /* Estados + 2 para el nuevo estado inicial y final*/
        nuevo_inicial = AFNDNuevo(p_afnd->nombre, p_afnd->num_estados+2, p_afnd->num_simbolos);
        assert(nuevo_inicial != NULL);
        
        /* Insercion de los simbolos */
        AFNDCopiaSimbolos(nuevo_inicial, p_afnd);
        
        nuevo_inicial = AFNDInsertaEstado(nuevo_inicial, "ini_new", INICIAL);
        nuevo_inicial = AFNDInsertaEstado(nuevo_inicial, "end_new", FINAL);
        
        /* Cambiamos el tipo de los antiguos estados iniciales a NORMAL, insercion 
        de estados */
        for(i = 0; i < p_afnd->num_estados; i++){
            if(estadoTipo(p_afnd->estados[i]) == INICIAL)
                AFNDInicialFinalLambda(nuevo_inicial, "ini_new", estadoNombre(p_afnd->estados[i]));
            
            else
                 nuevo_inicial = AFNDInsertaEstado(nuevo_inicial, estadoNombre(p_afnd->estados[i]), estadoTipo(p_afnd->estados[i]));
            /* Insercion de transiciones */
            AFNDCopiaTransiciones(nuevo_inicial, p_afnd, i);
        }
        
    }
    
    /* Nos aseguramos que solo haya un estado final (tenieno en cuenta si ya 
    habiamos hecho cambios con respecto al estado inicial) */
    if(contador_fin > 0){
        if(nuevo_inicial == NULL){
            nuevo_final = AFNDNuevo(p_afnd->nombre, p_afnd->num_estados+1, p_afnd->num_simbolos);
            assert(nuevo_final != NULL);
            
            /* Insercion de los simbolos */
            AFNDCopiaSimbolos(nuevo_final, p_afnd);

            /* Cambiamos el tipo de los antiguos estados iniciales a NORMAL, insercion 
            de estados*/
            nuevo_final = AFNDInsertaEstado(nuevo_final, "end_new", FINAL);
            for(i = 0; i < p_afnd->num_estados; i++){
                if(estadoTipo(p_afnd->estados[i]) == FINAL)
                    AFNDInicialFinalLambda(nuevo_final, "end_new", estadoNombre(p_afnd->estados[i]));
                
                else
                    nuevo_final = AFNDInsertaEstado(nuevo_final, estadoNombre(p_afnd->estados[i]), estadoTipo(p_afnd->estados[i]));
                /* Inserciones de transiciones */
                AFNDCopiaTransiciones(nuevo_final, p_afnd, i);

            }
            
        }
        
        else{
            nuevo_final = AFNDNuevo(nuevo_inicial->nombre, nuevo_inicial->num_estados+1, nuevo_inicial->num_simbolos);
            assert(nuevo_final != NULL);
            
            /* Insercion de los simbolos */
            AFNDCopiaSimbolos(nuevo_final, nuevo_inicial);
            
            nuevo_final = AFNDInsertaEstado(nuevo_final, "end_new", FINAL);
            for(i = 0; i < nuevo_inicial->num_estados; i++){
                if(estadoTipo(nuevo_inicial->estados[i]) == FINAL)
                    AFNDInicialFinalLambda(nuevo_final, "end_new", estadoNombre(nuevo_inicial->estados[i]));

                else
                     nuevo_inicial = AFNDInsertaEstado(nuevo_final, estadoNombre(nuevo_inicial->estados[i]), estadoTipo(nuevo_inicial->estados[i]));
            
                /* Inserciones de transiciones */
                AFNDCopiaTransiciones(nuevo_final, nuevo_inicial, i);

            }
            
        }
        

    }
    
    /***  Es necesario eliminar el automata de entrada? ***/
    
    if(nuevo_inicial == NULL){
        if(nuevo_final == NULL)
            return p_afnd;
        else{
            AFNDElimina(p_afnd);
            return nuevo_final;
        }
    }
    else{
        if(nuevo_final == NULL){
            AFNDElimina(p_afnd);
            return nuevo_inicial
        }
        else{
            AFNDElimina(p_afnd);
            AFNDElimina(nuevo_inicial);
            return nuevo_final;
        }
    }
    
    return NULL;
}


AFND * AFND1OUne(AFND * p_afnd1O_1, AFND * p_afnd1O_2);{
    
    assert(p_afnd1O_1 != NULL);
    assert(p_afnd1O_2 != NULL);
    
    int i, j, k, num_estados, num_simbolos;
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
    
    /* Insercion de simbolos */
    AFNDCopiaSimbolos(afnd_une, p_afnd1O_1);
    AFNDCopiaSimbolos(afnd_une, p_afnd1O_2);
    
    /* Creacion de nuevos estado inicial y final */
    AFNDInsertaEstado(afnd_une, "ini_new", INICIAL);
    AFNDInsertaEstado(afnd_une, "end_new", FINAL);
    
    /* Estados y transiciones del primer automata */
    for(i = 0; i < p_afnd1O_1->num_estados; i++){
        if(estadoTipo(p_afnd1O_1->estados[i]) == INICIAL)
            AFNDInicialFinalLambda(afnd_une, "ini_new", estadoNombre(p_afnd->estados[i]));

        
        else if(p_afnd1O_1->estados[i]) == FINAL)
            AFNDInicialFinalLambda(afnd_une, "end_new", estadoNombre(p_afnd->estados[i]));
       
        else
            afnd_une = AFNDInsertaEstado(afnd_une, estadoNombre(p_afnd1O_1->estados[i]), estadoTipo(p_afnd1O_1->estados[i]));

        /* Insertamos las transiciones */ 
        /** ESTO HAY QUE REVISARLO, AL SER DOS AUTOMATAS EN UNO **/
        AFNDCopiaTransiciones(afnd_une, p_afnd1O_1, i);

        
    }
    
    /* Estados y transiciones del segundo automata */
    for(i = 0; i < p_afnd1O_2->num_estados; i++){
        if(estadoTipo(p_afnd1O_2->estados[i]) == INICIAL){
            afnd_une = AFNDInsertaEstado(afnd_une, estadoNombre(p_afnd1O_2->estados[i]), NORMAL);
            afnd_une = AFNDInsertaLTransicion(afnd_une, "ini_new", estadoNombre(p_afnd1O_2->estados[i]));
        }
        
        else if(p_afnd1O_2->estados[i]) == FINAL){
            afnd_une = AFNDInsertaEstado(afnd_une, estadoNombre(p_afnd1O_2->estados[i]), NORMAL);
            afnd_une = AFNDInsertaLTransicion(afnd_une, "ini_new", estadoNombre(p_afnd1O_2->estados[i]));
        }
        else
            afnd_une = AFNDInsertaEstado(afnd_une, estadoNombre(p_afnd1O_2->estados[i]), estadoTipo(p_afnd1O_2->estados[i]));

        /* Insertamos las transiciones */ 
        for(j = 0; j < p_afnd1O_2->num_simbolos; j++)
            for(k = 0; k < p_afnd1O_2->num_estados; k++)
                if((p_afnd1O_2->ftransicion[i][j])[k] == 1)
                    afnd_une = AFNDInsertaTransicion(afnd_une, estadoNombre(p_afnd1O_2->estados[i]), alfabetoSimboloEn(p_afnd1O_2->alfabeto, j),estadoNombre(p_afnd1O_2->estados[k]));
    }
    
    
    free(nombre);
    /*** Es necesario eliminar los automatas recibidos? ***/
    
    
}