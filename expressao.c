#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"
#include <ctype.h>
#include <locale.h>

#define MAX 512

typedef struct {
    float items[MAX];
    int top;
} StackFloat;

typedef struct {
    char items[MAX][50];
    int top;
} StackString;

// Inicializa pilha de float
void initFloat(StackFloat *s) {
    s->top = -1;
}

// Verifica se a pilha de float está vazia
int isEmptyFloat(StackFloat *s) {
    return s->top == -1;
}

// Empilha um valor float
void pushFloat(StackFloat *s, float value) {
    s->items[++(s->top)] = value;
}

// Desempilha um valor float
float popFloat(StackFloat *s) {
    return s->items[(s->top)--];
}

// Inicializa pilha de strings
void initString(StackString *s) {
    s->top = -1;
}

// Empilha uma string
void pushString(StackString *s, char *value) {
    strcpy(s->items[++(s->top)], value);
}

// Desempilha uma string
char* popString(StackString *s) {
    return s->items[(s->top)--];
}

// Verifica se é operador
int isOperator(char *token) {
    return (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || strcmp(token, "*") == 0 ||
            strcmp(token, "/") == 0 || strcmp(token, "%") == 0 || strcmp(token, "^") == 0);
}

// Verifica se é função matemática
int isFunction(char *token) {
    return (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 ||
            strcmp(token, "log") == 0 || strcmp(token, "raiz") == 0);
}

// Define prioridade dos operadores
int precedence(char c) {
    if (c == '^') return 3;
    if (c == '*' || c == '/' || c == '%') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

// Converte expressão INFIXA para PÓS-FIXA
char* getFormaPosFixa(char *Str) {
    static char output[MAX] = "";
    char stackOp[MAX][50];
    int top = -1, k = 0;

    output[0] = '\0';
    char token[50];
    int i = 0, j = 0;

    while (i <= strlen(Str)) {
        char c = Str[i];

        if (isspace(c)) {
            i++;
            continue;
        }

        if (isalnum(c)) {
            j = 0;
            while (isalnum(Str[i])) {
                token[j++] = Str[i++];
            }
            token[j] = '\0';

            if (isFunction(token)) {
                strcpy(stackOp[++top], token);
            } else {
                strcat(output, token);
                strcat(output, " ");
            }
            continue;
        }

        if (c == '(') {
            strcpy(stackOp[++top], "(");
        } else if (c == ')') {
            while (top >= 0 && strcmp(stackOp[top], "(") != 0) {
                strcat(output, stackOp[top--]);
                strcat(output, " ");
            }
            if (top >= 0 && strcmp(stackOp[top], "(") == 0) {
                top--;
            }
            if (top >= 0 && isFunction(stackOp[top])) {
                strcat(output, stackOp[top--]);
                strcat(output, " ");
            }
        } else {
            char op[2] = {c, '\0'};
            while (top >= 0 && !isFunction(stackOp[top]) && strcmp(stackOp[top], "(") != 0 &&
                   precedence(stackOp[top][0]) >= precedence(c)) {
                strcat(output, stackOp[top--]);
                strcat(output, " ");
            }
            strcpy(stackOp[++top], op);
        }
        i++;
    }

    while (top >= 0) {
        strcat(output, stackOp[top--]);
        strcat(output, " ");
    }

    return output;
}

// Converte expressão PÓS-FIXA para INFIXA
char* getFormaInFixa(char *Str) {
    static char result[MAX] = "";
    StackString stack;
    initString(&stack);

    char buffer[MAX];
    strcpy(buffer, Str);
    char *token = strtok(buffer, " ");

    while (token) {
        if (isOperator(token)) {
            char b[50], a[50], expr[150];
            strcpy(b, popString(&stack));
            strcpy(a, popString(&stack));
            sprintf(expr, "(%s %s %s)", a, token, b);
            pushString(&stack, expr);
        } else if (isFunction(token)) {
            char a[50], expr[100];
            strcpy(a, popString(&stack));
            sprintf(expr, "%s(%s)", token, a);
            pushString(&stack, expr);
        } else {
            pushString(&stack, token);
        }
        token = strtok(NULL, " ");
    }

    strcpy(result, popString(&stack));
    return result;
}

// Calcula valor de expressão POS-FIXA
float getValorPosFixa(char *StrPosFixa) {
    StackFloat stack;
    initFloat(&stack);

    char expr[MAX];
    strcpy(expr, StrPosFixa);
    char *token = strtok(expr, " ");

    while (token) {
        if (isOperator(token)) {
            float b = popFloat(&stack);
            float a = popFloat(&stack);

            if (strcmp(token, "+") == 0) pushFloat(&stack, a + b);
            else if (strcmp(token, "-") == 0) pushFloat(&stack, a - b);
            else if (strcmp(token, "*") == 0) pushFloat(&stack, a * b);
            else if (strcmp(token, "/") == 0) pushFloat(&stack, a / b);
            else if (strcmp(token, "%") == 0) pushFloat(&stack, (int)a % (int)b);
            else if (strcmp(token, "^") == 0) pushFloat(&stack, powf(a, b));

        } else if (isFunction(token)) {
            float a = popFloat(&stack);

            if (strcmp(token, "sen") == 0) pushFloat(&stack, sinf(a * M_PI / 180));
            else if (strcmp(token, "cos") == 0) pushFloat(&stack, cosf(a * M_PI / 180));
            else if (strcmp(token, "tg") == 0) pushFloat(&stack, tanf(a * M_PI / 180));
            else if (strcmp(token, "log") == 0) pushFloat(&stack, log10f(a));
            else if (strcmp(token, "raiz") == 0) pushFloat(&stack, sqrtf(a));

        } else {
            pushFloat(&stack, atof(token));
        }

        token = strtok(NULL, " ");
    }

    return popFloat(&stack);
}

// Calcula valor de expressão INFIXA convertendo para pós-fixa antes
float getValorInFixa(char *StrInFixa) {
    char posFixa[MAX];
    strcpy(posFixa, getFormaPosFixa(StrInFixa));
    return getValorPosFixa(posFixa);
}

// Valida se a expressão é infixa ou pós-fixa e se está correta
int validaExpressao(char *expr) {
    int contParenteses = 0;
    int numOperandos = 0;
    int numOperadores = 0;
    char buffer[512];
    strcpy(buffer, expr);
    char *token = strtok(buffer, " ");

    while (token != NULL) {
        if (strcmp(token, "(") == 0) {
            contParenteses++;
        } else if (strcmp(token, ")") == 0) {
            contParenteses--;
            if (contParenteses < 0) return 0;
        } else if (isOperator(token) || isFunction(token)) {
            numOperadores++;
        } else {
            // Normaliza vírgula para ponto antes de verificar se é número
            char tokenCopia[50];
            strncpy(tokenCopia, token, 49);
            tokenCopia[49] = '\0';

            // Substitui ',' por '.'
            for (int i = 0; tokenCopia[i]; i++) {
                if (tokenCopia[i] == '.') tokenCopia[i] = ',';
            }

            // Testa se é número válido
            char *endptr;
            strtof(tokenCopia, &endptr);
            if (*endptr == '\0') {
                numOperandos++;
            } else {
                return 0; // token inválido
            }
        }

        token = strtok(NULL, " ");
    }

    if (contParenteses != 0) return 0;
    if (numOperandos == 0) return 0;
    if (numOperandos == 1 && numOperadores == 0) return 0;
    if (numOperadores == 0 && numOperandos > 1) return 0;

    return 1;
}