#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include "expressao.h"

void ajustaParenteses(char *entrada, char *saida) {
    int i = 0, j = 0;
    while (entrada[i] != '\0') {
        if (isspace(entrada[i])) {
            i++;
            continue;
        }

        // Verifica se é função matemática
        if (strncmp(&entrada[i], "sen", 3) == 0 ||
            strncmp(&entrada[i], "cos", 3) == 0 ||
            strncmp(&entrada[i], "log", 3) == 0) {
            
            if (j > 0 && saida[j-1] != ' ') saida[j++] = ' ';
            strncpy(&saida[j], &entrada[i], 3);
            j += 3; i += 3;
            saida[j++] = ' ';
            continue;

        } else if (strncmp(&entrada[i], "tg", 2) == 0) {
            if (j > 0 && saida[j-1] != ' ') saida[j++] = ' ';
            strncpy(&saida[j], &entrada[i], 2);
            j += 2; i += 2;
            saida[j++] = ' ';
            continue;

        } else if (strncmp(&entrada[i], "raiz", 4) == 0) {
            if (j > 0 && saida[j-1] != ' ') saida[j++] = ' ';
            strncpy(&saida[j], &entrada[i], 4);
            j += 4; i += 4;
            saida[j++] = ' ';
            continue;
        }

        // Se for operador ou parêntese
        if (strchr("()+-*/^%", entrada[i])) {
            if (j > 0 && saida[j-1] != ' ') saida[j++] = ' ';
            saida[j++] = entrada[i++];
            saida[j++] = ' ';
            continue;
        }

        // É número ou variável
        if (isalnum(entrada[i]) || entrada[i] == '.') {
            if (j > 0 && saida[j-1] != ' ') saida[j++] = ' ';
            while (isalnum(entrada[i]) || entrada[i] == '.') {
                saida[j++] = entrada[i++];
            }
            saida[j++] = ' ';
            continue;
        }

        // Caso contrário, apenas ignora o caractere
        i++;
    }

    // Remove espaço final se houver
    if (j > 0 && saida[j-1] == ' ') j--;
    saida[j] = '\0';
}

int validaExpressao(char *Str);

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
                strcpy(expr.inFixa, infixaAjustada);

                if (!validaExpressao(expr.inFixa)) {
                    printf("\nExpressao invalida. Tente novamente!\n");
                    continue;
                }

                strcpy(expr.posFixa, getFormaPosFixa(expr.inFixa));
                expr.Valor = getValorInFixa(expr.inFixa);

                printf("\nINFIXA: %s\n", expr.inFixa);
                printf("POS-FIXA: %s\n", expr.posFixa);
                printf("VALOR: %.6f\n", expr.Valor);
                break;

            case 2:
                printf("Digite a expressao PÓS-FIXA:\n> ");
                fgets(expr.posFixa, 512, stdin);
                expr.posFixa[strcspn(expr.posFixa, "\n")] = 0;

                if (!validaExpressao(expr.posFixa)) {
                    printf("\nExpressao invalida. Tente novamente!\n");
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
                printf("\nOpcao invalida.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}