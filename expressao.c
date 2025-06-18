#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "expressao.h"

No * inicializarPilha(){
    No * no = malloc(sizeof(No));

    no->anterior = NULL;

    return no;
}

No * Empilhar(char X[100], No * topo){
    No * no = malloc(sizeof(No));
    if (no == NULL){
        printf("ERRO DE ALOCAÇÂO");
        return NULL;
    }

    strcpy(no->expressao, X);
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
        printf("%s, ", topo->expressao);        //retirar essa função depois, fiz ela só pra testar
        topo = topo->anterior;
    }
}

char * getFormaInFixa(char *Str){       // Retorna a forma inFixa de Str (posFixa)
    No * pilha = NULL;  
    char *token = strtok(Str, " ");     // passa por cada elemento separando por " "

    while (token != NULL) {
        if (isdigit(token[0])){         //identifica se o token é um número
            pilha = Empilhar(token, pilha);
        }

        else if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || strcmp(token, "*") || strcmp(token, "/") || strcmp(token, "^") == 0 || strcmp(token, "%") == 0 || strcmp(token, "raiz") == 0) {
            char aux1[100], aux2[200], novaexpressao[100];  //caso for qualquer uma das operações a cima, ele:
            strcpy(aux1, pilha->expressao);     //guarda o ultimo elemento em uma variavel local;
            Desempilhar(&pilha);                //desempilha 
            strcpy(aux2, pilha->expressao);     //guarda o outro elemento em outra variavel local
            Desempilhar(&pilha);                //desempilha 

            sprintf(novaexpressao, "(%s %s %s)", aux2, token, aux1);    //cria uma nova string que junta as 2 junto com o operador identificado

            pilha = Empilhar(novaexpressao, pilha);         //salva na pilha 
        }else if (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "tan") == 0 || strcmp(token, "tg") == 0 || strcmp(token, "log") == 0) {
            char aux[100], novaexpressao[110];
            strcpy(aux, pilha->expressao);
            Desempilhar(&pilha);                             //essa parte ainda n esta funcionando, n sei pq mas quando    
            sprintf(novaexpressao, "(%s%s)", token, aux);    //a operação tem log ou qualquer outra das acima ela n identifica   
            pilha = Empilhar(novaexpressao, pilha);
        }
            token = strtok(NULL, " ");
        }

        char *infixa = malloc(521);
        strcpy(infixa, pilha->expressao);       //copia o ultimo elemento restante da pilha no ponteiro 
        
        return infixa;                          //retorna o ponteiro
    }

/*
char *getFormaPosFixa(char *Str){
    
}
float getValorPosFixa(char *StrPosFixa){
    
}
float getValorInFixa(char *StrInFixa){

}*/