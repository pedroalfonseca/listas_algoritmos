/*
<Problem Statement>
Seu objetivo é fazer a multiplicação de dois inteiros sem utilizar os operadores de multiplicação e divisão.

<Input Specification>
Dois inteiros X e Y, separados por quebra de linha.

<Output Specification>
Um inteiro E, que é o produto entre X e Y, seguido por quebra de linha.
*/

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int x, y, e = 0;

    scanf("%d", &x);
    scanf("%d", &y);

    for (int i = 0; i < abs(y); i++)
    {
        e += x;
    }

    if ((x < 0 && y < 0) || (x > 0 && y < 0))
    {
        e = -e;
    }

    printf("%d\n", e);
}
