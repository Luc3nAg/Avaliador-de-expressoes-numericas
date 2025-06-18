#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expressao.h"

int main(){
    char Str[] = "20 10 3 4 6 + / ^ raiz";
    Expressao *ex = malloc(sizeof(Expressao)); 
    if (ex == NULL) {
        printf("Erro de alocação!\n");
        return 1;
    }
    char *resultado = getFormaInFixa(Str);
    strcpy(ex->inFixa, resultado);
    printf("Infixa: %s\n", ex->inFixa);
    free(resultado);
    free(ex);
    return 0;
}
