/*
<Problem Statement>
Um casal deseja levar o seu filho único ao parque de diversões e divertí-lo o máximo possível. O parque possui N brinquedos, numerados de 0 a N-1, cada um com um custo diferente de C[i] créditos por corrida. Os atencioso casal percebeu que o seu filho possui lá suas preferências e atribuiu a cada brinquedo um índice inicial de diversão S[i] para i=0,...,N-1. A criança pode andar mais de uma vez em cada brinquedo, mas eles também perceberam que a empolgação vai diminuindo rapidamente a cada corrida. Eles associaram a cada brinquedo um fator de aborrecimento B[i], e estimaram que a diversão obtida na t-ésima corrida no brinquedo i decai para F(i,t)=S[i]-(t-1)²*B[i], para t=1,2,3,..., sendo a diversão considerada nula assim que esse valor de F(i,t) torna-se <=0. Sabendo que o casal só dispõe de um cartão com uma quantidade limitada de créditos K, sua tarefa é ajudá-los a escolher os brinquedos que resultem na maior diversão possível.

<Input Specification>
A entrada é iniciada por uma linha contendo um inteiro 1 <= N <= 100 correspondente ao número de brinquedos do parque.

Seguem-se N linhas:
S[0] B[0] C[0]
S[1] B[1] C[1]
...

S[N-1] B[N-1] C[N-1]

onde:
S[i] = índice de diversão do brinquedo i(0 < S[i] <= 1000)
B[i] = fator de aborrecimento do brinquedo i (0 < B[i] <= 1000)
C[i] = custo em créditos por corrida do brinquedo i (0 < C[i] <= 25000)
Logo em seguida, temos uma linha com um inteiro 1 <= V <= 1000 indicando o número de visitas ao parque.

Seguem-se V linhas:
K[0] 
K[1] 
...

K[V-1]

onde:
K[j] = total de créditos disponíveis na j-ésima visita (1<=K[j]<=25000).

<Output Specification>
Para cada visita j=0,...,V-1, imprimir uma linha na forma:
j: MF[j]

onde:
MF[j] corresponde à máxima diversão total possível na visita j.
*/

#include <stdio.h>
#include <stdlib.h>

int** knapsack(int* weights, int* values, int* B, int n, int K);

int main(void)
{
    int N;
    scanf("%d", &N);
    
    int S[N], B[N], C[N];
    for (int i = 0; i < N; i++)
    {
        scanf("%d %d %d", &S[i], &B[i], &C[i]);
    }

    int V;
    scanf("%d", &V);

    int K[V];
    int greatest_K = 0;
    for (int i = 0; i < V; i++)
    {
        scanf("%d", &K[i]);
        greatest_K = (K[i] > greatest_K ? K[i] : greatest_K);
    }

    int MF[V];
    int** tmp_matrix = knapsack(C, S, B, N, greatest_K);

    for (int i = 0; i < V; i++)
    {
        MF[i] = tmp_matrix[N][K[i]];
        printf("%d: %d\n", i, MF[i]);
    }
    
    for (int i = 0; i < N+1; i++)
    {
    	free(tmp_matrix[i]);
    }
    free(tmp_matrix);
}

int** knapsack(int* weights, int* values, int* B, int n, int K)
{
    int** R = (int**) malloc((n+1)*sizeof(int*));
    for (int i = 0; i < n+1; i++)
    {
        R[i] = (int*) malloc((K+1)*sizeof(int));
        for (int j = 0; j < K+1; j++)
        {
            R[i][j] = 0;
        }
    }

    for (int m = 1; m < n+1; m++)
    {
        for (int k = 1; k < K+1; k++)
        {
            if (k < weights[m-1])
            {
                R[m][k] = R[m-1][k];
            }
            else
            {
                int i = 0;
                int greatest = R[m-1][k];
                while (i*weights[m-1] <= k)
                {
                    int F = 0;
                    for (int t = 1; t < i+1; t++)
                    {
                        F += values[m-1] - (t-1)*(t-1)*B[m-1];
                    }
                    greatest = ((F + R[m-1][k-i*weights[m-1]]) > greatest ? (F + R[m-1][k-i*weights[m-1]]) : greatest);
                    i++;
                }
                R[m][k] = greatest;
            }
        }
    }

    return R;
}
