/*
<Problem Statement>
Crie um programa que determina se um dado número é par ou ímpar.

<Input Specification>
O programa receberá um número

O número será um inteiro qualquer

<Output Specification>
Seu programa deve determinar se o número é par ou ímpar

Saídas possíveis: "Par" ou "Impar"
*/

#include <stdio.h>

int main(void)
{
    int n;
    scanf("%d", &n);

    if (n % 2 == 0)
    {
        printf("Par\n");
    }
    else
    {
        printf("Impar\n");
    }
}
