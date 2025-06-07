#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "expressao.h"

No * inicializarPilha(){
    No * no = malloc(sizeof(No));

    no->anterior = NULL;
    no->valor = 0;

    return no;
}

No * Empilhar(int X, No * topo){
    No * no = malloc(sizeof(No));
    if (no == NULL){
        printf("ERRO DE ALOCAÇÂO");
        return NULL;
    }

    no->valor = X;
    no->anterior = topo;
    return no;
}

No * Desempilhar(No **topo){
    if (*topo == NULL){
        printf("PILHA VAZIA");
        return NULL;
    }

    No *remover = *topo;
    *topo = remover->anterior;
    return *topo;
}

void imprimirFila(No *topo){
    while (topo){
        printf("%.2f, ", topo->valor);
        topo = topo->anterior;
    }
}

void getFormaInFixa(char *Str){
    No * pilha = NULL;
    char *token = strtok(Str, " ");

    while (token != NULL) {
        if (isdigit(token[0])){
            float Num = atof(token);
            pilha = Empilhar(Num, pilha);
            printf("NUMERO EMPILHADO: %.2f\n", pilha->valor);
        }

        else if (strcmp(token, "+") == 0) {
            printf("SOMA\n");
        } else if (strcmp(token, "-") == 0) {
            printf("SUB\n");
        } else if (strcmp(token, "*") == 0) {
            printf("MULTI\n");
        } else if (strcmp(token, "/") == 0) {
            printf("DIV\n");
        } else if (strcmp(token, "%") == 0){
            printf("MOD\n");
        } else if (strcmp(token, "^") == 0){
            printf("ELEVADO\n");
        }else if (strcmp(token, "raiz") == 0){
            printf("RAIZ\n");
        }else if (strcmp(token, "sen") == 0){
            printf("SEN\n");
        }else if (strcmp(token, "cos") == 0){
            printf("COS\n");
        }else if (strcmp(token, "tg") == 0){
            printf("TG\n");
        }else if (strcmp(token, "log") == 0){
            printf("LOG\n");
        }
            token = strtok(NULL, " ");
        }

        printf("\nPILHA COMPLETA: ");
        imprimirFila(pilha);
        
    }

/*
char *getFormaPosFixa(char *Str){
    
}
float getValorPosFixa(char *StrPosFixa){
    
}
float getValorInFixa(char *StrInFixa){

}*/