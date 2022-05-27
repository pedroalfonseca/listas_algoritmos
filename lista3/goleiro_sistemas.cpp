/*
<Problem Statement>
Um sistema de segurança e controle de redes de computadores coleta informações sobre o tráfego na forma de uma stream de dados em que vários "pontos" na forma

(X0,W1), (X1,W1), (X2,W2), ...

são obtidos em fluxo contínuo. Cada ponto (Xj, Wj) representa um pacote transmitido, sendo Xj um valor inteiro num intervalo [M]=0..M-1 que corresponde a um identficador único de um dispositivo, e Wj um inteiro positivo que corresponde ao no. de bytes transmitidos (payload) naquele pacote.

Uma atividade suspeita na rede é detectada quando um dispositivo transmite uma quantidade muito grande de informação. Assim, algumas consultas frequentes são:

WEI(X): qual a quantidade total de bytes transmitidos pelo dispositivo com id X
RNK(X): qual a quantidade total de bytes transmitidos pelos dispositivos com id < X
A primeira consulta inspeciona a atividade de um dispositivo em particular. Já a segunda consulta pode servir para estimar a distribuição do tráfego entre os dispositivos. Por exemplo, num tráfego uniformemente distribuído, a metade do total de bytes transmitidos na rede deveria ter sido originada pelos dispositivos com id < M/2.

Steve foi contratado pela empresa Goleiro Sistemas para implementar essas funcionalidades. Imediatamente veio-lhe à memória as aulas sobre Árvores AVL e uma maneira de usar essa estrutura para resolver o problema. A solução consiste em criar uma AVL ordenada pelos ids dos dispositivos. Além do campo ID com um certo valor X, cada nó N deve conter um campo WEI (peso) com a soma total dos bytes transmitidos pelo dispositivo X até o momento, ou seja, a resposta à consulta WEI(X), e um outro campo RNK com a soma total dos bytes transmitidos pelos dispositivos com id estritamente inferior a X que estão na sub-árvore à esquerda de N, ou seja, a soma dos valores dos campos WEI nessa sub-árvore.

Por exemplo, a árvore a seguir ilustra o caso para 7 dispositivos. Os nós estão na forma (ID,WEI,RNK).

                        (10,2,11)
           .-------------'     '------------.               
           |                                |
        (6,4,5)                          (14,2,5)
   .-----'   '-----.                .-----'    
   |               |                |                
(2,3,0)          (8,2,0)         (12,5,0)          
     '----.
          |
        (4,2,0)

Neste caso, a consulta por RNK(10) retorna o valor do campo RNK da raiz. A resposta para a consulta RNK(5) é obtida totalmente na sub-árvore à esquerda. Já a resposta a RNK(14) é obtida a partir da soma dos pesos dos valores menores que 14, isto é, a raiz e os elementos à sua esquerda, mais a soma dos pesos dos valores menores que 14 na sub-árvore direita.

<Input Specification>
A entrada é constituída por várias linhas, cada uma num dos formatos a seguir:
ADD X W: sinaliza a transmissão e um novo pacote de W bytes para o dispositivo com id X.
WEI X: consulta o total de pacotes transmitidos pelo dispositivo com id X até o momento.
RNK X: consulta o total de pacotes transmitidos pelo dispositivo com id < X até o momento.

A entrada termina com uma linha:
END

<Output Specification>
Para cada linha da entrada deve ser impressa uma saída correspondente como descrito a seguir:
ADD X W => adiciona a informação à AVL e imprime uma linha com um inteiro correspondente ao total global de bytes transmitidos por todos os dispositivos na rede até o momento.
WEI X => imprime uma linha com dois inteiros W D correspondentes, respectivamente, ao valor de WEI(X), e à profundidade do nó com id X na AVL (a raiz tem profundidade 0, os seus filhos têm profundidade 1, e assim sucessivamente) NOTA: se a máquina com id X ainda não transmitiu dados, deve ser impressa a linha 0 -1.
RNK X => imprime uma linha com um inteiro correspondente ao total de bytes transmitidos por dispositivos com id < X até o momento. NOTA: não necessariamente o dispositivo com id X transmitiu dados até o momento. Ou seja, a AVL pode não conter um nó com id X. Mesmo assim a consulta tem uma resposta correta, ainda que seja 0.
*/

#include <iostream>

struct _node
{
    struct _node* left;
    struct _node* right;
    int id, bf;
    int WEI, RNK;
};
typedef struct _node node;

typedef struct
{
    node* new_root;
    bool hc;
    int total_weight;
}insert_values;

typedef struct
{
    int weight, depth;
}search_values;

node* rotate_left(node* root);

node* rotate_right(node* root);

insert_values avl_insert(node* root, int val, int weight, int total_weight);

search_values bst_search(node* root, int val, int depth);

int find_rank(node* root, int val);

void delete_tree(node* root);

int main(void)
{
    node* root = NULL;
    int total_weight = 0;

    while(true)
    {
        std::string command;
        std::cin >> command;

        if (command[0] == 'E')
        {
            break;
        }
        else if (command[0] == 'A')
        {
            int x, w;
            std::cin >> x;
            std::cin >> w;

            insert_values values = avl_insert(root, x, w, total_weight);
            root = values.new_root;
            total_weight = values.total_weight;

            std::cout << total_weight << std::endl;
        }
        else if (command[0] == 'W')
        {
            int x, weight, depth;
            std::cin >> x;

            search_values values = bst_search(root, x, 0);
            weight = values.weight;
            depth = values.depth;

            std::cout << weight << ' ' << depth << std::endl;
        }
        else
        {
            int x;
            std::cin >> x;

            int rank = find_rank(root, x);

            std::cout << rank << std::endl;
        }
    }

    delete_tree(root);
    root = NULL;
}

node* rotate_left(node* root)
{
    node* R = root->right;
    node* RL = R->left;
    
    R->left = root;
    root->right = RL;

    R->RNK = root->WEI + root->RNK + R->RNK;

    if (R->bf <= 0)
    {
        root->bf--;
    }
    else
    {
        root->bf = (root->bf-1) - R->bf;
    }

    if (root->bf > 0)
    {
        R->bf--;
    }
    else
    {
        R->bf = (R->bf-1) + root->bf;
    }

    return R;
}

node* rotate_right(node* root)
{
    node* L = root->left;
    node* LR = L->right;

    L->right = root;
    root->left = LR;

    root->RNK = root->RNK - L->WEI - L->RNK; 

    if (L->bf >= 0)
    {
        root->bf++;
    }
    else
    {
        root->bf = (root->bf+1) - L->bf;
    }

    if (root->bf < 0)
    {
        L->bf++;
    }
    else
    {
        L->bf = (L->bf+1) + root->bf;
    }

    return L;
}

insert_values avl_insert(node* root, int val, int weight, int total_weight)
{
    bool hc = 0;

    if (root == NULL)
    {
        node* N = new node;
        N->id = val;
        N->WEI = weight;
        N->RNK = 0;
        total_weight += N->WEI;

        return {N, 1, total_weight};
    }
    else if (val < root->id)
    {
        root->RNK += weight;
        insert_values values = avl_insert(root->left, val, weight, total_weight);
        root->left = values.new_root;
        hc = values.hc;
        total_weight = values.total_weight;
        root->bf = root->bf - hc;
    }
    else if (val > root->id)
    {
        insert_values values = avl_insert(root->right, val, weight, total_weight);
        root->right = values.new_root;
        hc = values.hc;
        total_weight = values.total_weight;
        root->bf = root->bf + hc;
    }
    else //val == root->id
    {
        root->WEI += weight;
        total_weight += weight;
        return {root, 0, total_weight};
    }

    if (hc == 0)
    {
        return {root, 0, total_weight};
    }
    else if (root->bf == 0)
    {
        return {root, 0, total_weight};
    }
    else if (root->bf == 1 or root->bf == -1)
    {
        return {root, 1, total_weight};
    }
    else
    {
        if (root->bf == -2)
        {
            if (root->left->bf == 1)
            {
                root->left = rotate_left(root->left);
            }

            return {rotate_right(root), 0, total_weight};
        }
        else
        {
            if (root->right->bf == -1)
            {
                root->right = rotate_right(root->right);
            }

            return {rotate_left(root), 0, total_weight};
        }
    }
}

search_values bst_search(node* root, int val, int depth)
{
    if (root == NULL)
    {
        return {0, -1};
    }

    if (val == root->id)
    {
        return {root->WEI, depth};
    }
    else if (val < root->id)
    {
        depth++;
        return bst_search(root->left, val, depth);
    }
    else
    {
        depth++;
        return bst_search(root->right, val, depth);
    }
}

int find_rank(node* root, int val)
{
    if (root == NULL)
    {
        return 0;
    }
    else if (val == root->id)
    {
        return root->RNK;
    }
    else if (val < root->id)
    {
        return find_rank(root->left, val);
    }
    else
    {
        return (find_rank(root->right, val) + root->WEI + root->RNK);
    }
}

void delete_tree(node* root)
{
    if (root != NULL)
    {
        delete_tree(root->left);
        delete_tree(root->right);
        delete root;

        if (root->left != NULL)
        {
            root->left = NULL;
        }
        if (root->right != NULL)
        {
            root->right = NULL;
        }
    }
}