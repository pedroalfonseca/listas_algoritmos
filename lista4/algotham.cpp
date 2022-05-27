/*
<Problem Statement>
O serviço secreto de Algotham adotou um novo sistema de segurança para a comunicação entre os seus agentes. A partir de agora cada agente possui um identificador numérico e um código gráfico (similar a um QR-code) na forma de uma matriz bidimensional de NxN pixels, sendo cada pixel 0 (branco) ou 1 (preto). Na realidade, esse código representa uma matriz de adjacências de um grafo (não-dirigido) de N vértices. Os grafos são aleatórios, porém cada um deles possui exatamente um K-clique, ou seja, subconjunto de K vértices todos interligados uns aos outros por um total K*(K-1)/2 arestas. Esse K-clique é um certificado do código e está disfarçado em meio a várias outras arestas, não sendo trivial detectá-lo apenas olhando para o código.

Dois agentes só poderão se comunicar entre si se seus códigos forem compatíveis. Assim, antes de iniciar a comunicação, os agentes tentam estabelecer uma conexão através dos seus códigos que são transmitidos por um canal exclusivo de satélite para a central de inteligência, onde são verificados. O teste de compatibilidade é simples: dois códigos são compatíveis se, e somente se, eles possuem o mesmo certificado/K-clique.

<Input Specification>
A entrada inicia-se por uma linha com quatro inteiros:
A N K Q

onde:
A indica o número de agentes, ou seja, os IDs dos agentes variam de 0..A-1
N o número de vértices de cada grafo/código
K o tamanho do clique de cada grafo
Q indica a quantidade de tentativas de conexão a serem processadas.
Em seguida temos Q tentativas de conexão entre agentes. A especificação de cada tentativa de conexão começa com uma linha:
X Y
onde X e Y indicam os IDs dos agentes. Seguem-se as especificações dos códigos dos agentes X e Y, nessa ordem, sendo cada código normalmente representado por N-1 linhas

E[1,0]
E[2,0]E[2,1]
E[3,0]E[3,1]E[3,2]
...
E[N-1,0]  ...  E[N-1,N-2]

onde:
E[i][j]= valor do pixel da linha i coluna j. Esse valor será = 1, se existe uma aresta (i,j), ou 0 caso contrário.
Ou seja, está representada apenas parte triangular inferior (abaixo da diagonal principal) da matriz de adjacências, sendo a matriz simétrica.

Importante
Para economizar espaço, o código de um agente só será explicitamente colocado na entrada na primeira vez que ele for usado numa tentativa de conexão. Nas tentativas subsequentes, o mesmo código deve ser utilizado. Cada agente 0..A-1 aparecerá em pelo menos uma tentativa de conexão, porém normalmente em várias.

Encontrar o K-clique de um código é uma operação muito custosa e só deverá ser feita no máximo uma vez por agente. Caso contrário, a central demorará muito para responder e não haverá tempo para processar todas as tentativas de conexão. Entretanto, caso o certificado de um dos códigos seja conhecido, testar se outro código é compatível com ele é bem menos custoso. Mais simples ainda, se soubermos que um código X é compatível com um código Y, e que o código Y é compatível com Z, então saberemos que X é compatível com Z sem necessidade de conhecer seus códigos/certificados.

<Output Specification>
Para cada tentativa de conexão
X Y

deve ser impressa uma linha na forma:
FAIL
se os certificados (K-cliques) C(X) e C(Y) dos códigos X e Y forem diferentes; OU

SUCC C(X)
se os certificados de X e Y forem idênticos. O certificado C(X) é impresso como a sequência dos vértices que formam o K-clique do código X em ordem crescente, separados por espaços.
*/

#include <iostream>

typedef struct
{
    int** matrix;
    int* clique;
    bool has_matrix;
}agent;

bool is_clique(int arr[], int K, agent* gordon);

void combinations(int N, int K, agent* gordon);

void find_clique(int N, int K, agent* gordon);

bool compare(agent* gordon, agent* dent, int K);

int main(void)
{
    int A, N, K, Q, X, Y;
    std::string input;

    std::cin >> A;
    std::cin >> N;
    std::cin >> K;
    std::cin >> Q;

    agent* agents[A];
    for (int i = 0; i < A; i++)
    {
        agents[i] = new agent;
        agents[i]->matrix = new int*[N];
        agents[i]->clique = new int[K];
        
        for (int j = 0; j < N; j++)
        {
            agents[i]->matrix[j] = new int[N];
        }
        agents[i]->clique[0] = -1;
        agents[i]->has_matrix = false;
    }

    for (int i = 0; i < Q; i++)
    {
        std::cin >> X;
        std::cin >> Y;

        if (agents[X]->has_matrix == false)
        {
            agents[X]->matrix[N-1][N-1] = 0;
            for (int j = 0; j < N-1; j++)
            {
                agents[X]->matrix[j][j] = 0;
                std::cin >> input;
                for (int k = 0; k < input.length(); k++)
                {
                    if (input[k] == '0')
                    {
                        agents[X]->matrix[j+1][k] = 0;
                        agents[X]->matrix[k][j+1] = 0;
                    }
                    else
                    {
                        agents[X]->matrix[j+1][k] = 1;
                        agents[X]->matrix[k][j+1] = 1;
                    }
                }
                agents[X]->has_matrix = true;
            }
        }

        if (agents[Y]->has_matrix == false)
        {
            agents[Y]->matrix[N-1][N-1] = 0;
            for (int j = 0; j < N-1; j++)
            {
                agents[Y]->matrix[j][j] = 0;
                std::cin >> input;
                for (int k = 0; k < input.length(); k++)
                {
                    if (input[k] == '0')
                    {
                        agents[Y]->matrix[j+1][k] = 0;
                        agents[Y]->matrix[k][j+1] = 0;
                    }
                    else
                    {
                        agents[Y]->matrix[j+1][k] = 1;
                        agents[Y]->matrix[k][j+1] = 1;
                    }
                }
                agents[Y]->has_matrix = true;
            }
        }

        if (agents[X]->clique[0] == -1 && agents[Y]->clique[0] != -1)
        {
            if (is_clique(agents[Y]->clique, K, agents[X]))
            {
                for (int i = 0; i < K; i++)
                {
                    agents[X]->clique[i] = agents[Y]->clique[i];
                }

                std::cout << "SUCC ";
                for (int i = 0; i < K; i++)
                {
                    if (i < K-1)
                    {
                        std::cout << agents[Y]->clique[i] << ' ';
                    }
                    else
                    {
                        std::cout << agents[Y]->clique[i] << std::endl;
                    }
                }
            }
            else
            {
                std::cout << "FAIL" << std::endl;
            }
        }
        else if (agents[X]->clique[0] != -1 && agents[Y]->clique[0] == -1)
        {
            if (is_clique(agents[X]->clique, K, agents[Y]))
            {
                for (int i = 0; i < K; i++)
                {
                    agents[Y]->clique[i] = agents[X]->clique[i];
                }

                std::cout << "SUCC ";
                for (int i = 0; i < K; i++)
                {
                    if (i < K-1)
                    {
                        std::cout << agents[X]->clique[i] << ' ';
                    }
                    else
                    {
                        std::cout << agents[X]->clique[i] << std::endl;
                    }
                }
            }
            else
            {
                std::cout << "FAIL" << std::endl;
            }
        }
        else if (agents[X]->clique[0] != -1 && agents[Y]->clique[0] != -1)
        {
            if (compare(agents[X], agents[Y], K))
            {
                std::cout << "SUCC ";
                for (int i = 0; i < K; i++)
                {
                    if (i < K-1)
                    {
                        std::cout << agents[Y]->clique[i] << ' ';
                    }
                    else
                    {
                        std::cout << agents[Y]->clique[i] << std::endl;
                    }
                }
            }
            else
            {
                std::cout << "FAIL" << std::endl;
            }
        }
        else
        {
            find_clique(N, K, agents[X]);

            if (is_clique(agents[X]->clique, K, agents[Y]))
            {
                for (int i = 0; i < K; i++)
                {
                    agents[Y]->clique[i] = agents[X]->clique[i];
                }

                std::cout << "SUCC ";
                for (int i = 0; i < K; i++)
                {
                    if (i < K-1)
                    {
                        std::cout << agents[X]->clique[i] << ' ';
                    }
                    else
                    {
                        std::cout << agents[X]->clique[i] << std::endl;
                    }
                }
            }
            else
            {
                std::cout << "FAIL" << std::endl;
            }
        }
    }

    for (int i = 0; i < A; i++)
    {
        for (int j = 0; j < N; j++)
        {
            delete[] agents[i]->matrix[j];
        }
        delete[] agents[i]->matrix;
        delete[] agents[i]->clique;
        delete agents[i];
    }
}

bool is_clique(int arr[], int K, agent* gordon)
{
    for (int i = 0; i < K; i++)
    {
        for (int j = i+1; j < K; j++)
        {
            if (gordon->matrix[arr[i]][arr[j]] == 0)
            {
                return false;
            }
        }
    }
    return true;
}

void combinations(int N, int K, agent* gordon)
{
    int P = K-1;

    while (P >= 0 && gordon->clique[P] == (P - K + N))
    {
        P = P-1;
    }

    if (P < 0)
    {
        return;
    }

    gordon->clique[P] = gordon->clique[P]+1;

    for (int i = P + 1; i < K; i++)
    {
        gordon->clique[i] = gordon->clique[i-1]+1;
    }
    return;
}

void find_clique(int N, int K, agent* gordon)
{
    for (int i = 0; i < K; i++)
    {
        gordon->clique[i] = i;
    }

    while (not is_clique(gordon->clique, K, gordon))
    {
        combinations(N, K, gordon);
    }
}

bool compare(agent* gordon, agent* dent, int K)
{
    for (int i = 0; i < K; i++)
    {
        if (gordon->clique[i] != dent->clique[i])
        {
            return false;
        }
    }
    return true;
}
