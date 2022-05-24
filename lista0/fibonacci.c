/*
<Problem Statement>
Stuart leu sobre a famosa sequência de Fibonacci e descobriu as seguintes informações: 
Fibonacci é uma das sequências mais interessantes da matemática, ela é definida da seguinte forma: 
	0 e 1 são os primeiros termos da sequência, tomemos eles como sendo o 1º e o 2º termo respectivamente.
	Se quisermos achar o próximo termo, nesse caso o 3º termo, então devemos somar os dois termos anteriores, nesse caso o próximo termo será 1. Se escrevermos no papel os 3 termos que temos até agora vamos obter:
	0, 1, 1 
	Agora se quisermos encontrar o próximo termo (o 4º termo) , devemos somar os 2 últimos até agora, que são 1 e 1, e obteremos o 2,  ficando com:
	0, 1 ,1, 2 
	Em seguida somamos 1 e 2 e obtemos o 3,  ficando com:
	0, 1, 1, 2, 3…
E aqui estão os 10 primeiros termos da sequência:
	0 ,1 , 1, 2, 3, 5, 8, 13, 21, 34
	Repare que todo número, com exceção dos dois primeiros, são iguais a soma dos dois números anteriores a ele. 
	Sua tarefa é escrever um programa que rebe um número inteiro N e imprime o N-ésimo termo da sequência de Fibonacci. 

<Input Specification>
Um número inteiro N

Limite: O N-ésimo fibonacci pode ser armazenado em um inteiro de 32 bits.

<Output Specification>
O N-ésimo termo da sequência de Fibonacci.
*/

#include <stdio.h>

int fibonacci(int n);

int main(void)
{
    int n;
    scanf("%d", &n);

    printf("%d\n", fibonacci(n));
}

int fibonacci(int n)
{
    if (n <= 1)
    {
        return 0;
    }
    else if (n == 2)
    {
        return 1;
    }
    else
    {
        return fibonacci(n-1) + fibonacci(n-2);
    }
}
