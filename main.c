#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include "expressao.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Ajusta a entrada para garantir espaços adequados entre tokens e funções
void ajustaParenteses(char *entrada, char *saida) {
    int i = 0, j = 0;
    while (entrada[i] != '\0') {
        if ((strncmp(&entrada[i], "sen", 3) == 0) ||
            (strncmp(&entrada[i], "cos", 3) == 0) ||
            (strncmp(&entrada[i], "tg", 2) == 0)  ||
            (strncmp(&entrada[i], "log", 3) == 0) ||
            (strncmp(&entrada[i], "raiz", 4) == 0)) {

            if (j > 0 && saida[j-1] != ' ') saida[j++] = ' ';
            if (strncmp(&entrada[i], "sen", 3) == 0) {
                strcpy(&saida[j], "sen"); j += 3; i += 3;
            } else if (strncmp(&entrada[i], "cos", 3) == 0) {
                strcpy(&saida[j], "cos"); j += 3; i += 3;
            } else if (strncmp(&entrada[i], "tg", 2) == 0) {
                strcpy(&saida[j], "tg"); j += 2; i += 2;
            } else if (strncmp(&entrada[i], "log", 3) == 0) {
                strcpy(&saida[j], "log"); j += 3; i += 3;
            } else if (strncmp(&entrada[i], "raiz", 4) == 0) {
                strcpy(&saida[j], "raiz"); j += 4; i += 4;
            }
            saida[j++] = ' ';
            continue;
        }
        if (entrada[i] == '(' || entrada[i] == ')' ||
            entrada[i] == '+' || entrada[i] == '-' ||
            entrada[i] == '*' || entrada[i] == '/' ||
            entrada[i] == '^' || entrada[i] == '%') {
            if (j > 0 && saida[j-1] != ' ') saida[j++] = ' ';
            saida[j++] = entrada[i];
            if (entrada[i+1] != ' ' && entrada[i+1] != '\0') saida[j++] = ' ';
        } else if (entrada[i] != ' ') {
            saida[j++] = entrada[i];
        }
        i++;
    }
    saida[j] = '\0';
}

// Função para remover espaços extras e normalizar a expressão infixa
void normalizaEspacos(char *entrada, char *saida) {
    int i = 0, j = 0;
    int ultimoEspaco = 1;
    while (entrada[i]) {
        if (isspace(entrada[i])) {
            if (!ultimoEspaco) {
                saida[j++] = ' ';
                ultimoEspaco = 1;
            }
        } else {
            saida[j++] = entrada[i];
            ultimoEspaco = 0;
        }
        i++;
    }
    // Remove espaço final, se houver
    if (j > 0 && saida[j-1] == ' ') j--;
    saida[j] = '\0';
}

int validaExpressao(char *Str, int tipo);

int main() {
    setlocale(LC_ALL, "Portuguese");

    Expressao expr;
    int opcao;

    do {
        printf("\n==== AVALIADOR DE EXPRESSAO ====");
        printf("\n1 - Inserir expressao na forma INFIXA");
        printf("\n2 - Inserir expressao na forma POS-FIXA");
        printf("\n0 - Sair");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Digite a expressao INFIXA:\n> ");
                fgets(expr.inFixa, 512, stdin);
                expr.inFixa[strcspn(expr.inFixa, "\n")] = 0;

                char infixaAjustada[512];
                ajustaParenteses(expr.inFixa, infixaAjustada);

                char infixaNormalizada[512];
                normalizaEspacos(infixaAjustada, infixaNormalizada);

                strcpy(expr.inFixa, infixaNormalizada);

                if (!validaExpressao(expr.inFixa, 0)) {
                    printf("\nExpressão invalida. Tente novamente!\n");
                    continue;
                }

                strcpy(expr.posFixa, getFormaPosFixa(expr.inFixa));
                expr.Valor = getValorInFixa(expr.inFixa);

                printf("\nINFIXA: %s\n", expr.inFixa);
                printf("POS-FIXA: %s\n", expr.posFixa);
                printf("VALOR: %.6f\n", expr.Valor);
                break;

            case 2:
                printf("Digite a expressao POS-FIXA:\n> ");
                fgets(expr.posFixa, 512, stdin);
                expr.posFixa[strcspn(expr.posFixa, "\n")] = 0;

                if (!validaExpressao(expr.posFixa, 1)) {
                    printf("\nExpressão invalida. Tente novamente!\n");
                    continue;
                }

                strcpy(expr.inFixa, getFormaInFixa(expr.posFixa));
                expr.Valor = getValorPosFixa(expr.posFixa);

                printf("\nPOS-FIXA: %s\n", expr.posFixa);
                printf("INFIXA: %s\n", expr.inFixa);
                printf("VALOR: %.6f\n", expr.Valor);
                break;

            case 0:
                printf("\nSaindo...\n");
                break;

            default:
                printf("\nOpção inválida.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}