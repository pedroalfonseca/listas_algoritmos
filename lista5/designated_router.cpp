/*
<Problem Statement>
Numa rede de dados, N nós (roteadores) são conectados entre si através de linhas comunicação. Um protocolo de roteamento é utilizado para determinar o "melhor" caminho (rota) que um pacote de dados deve seguir do nó emissor S até o nó receptor T da mensagem. Como o número de possíveis rotas entre dois nós cresce exponencialmente com a quantidade de pontos da rede, uma alternativa para simplificar a tarefa de roteamento consiste em eleger um nó da rede como roteador designado (Designated Router - DR) e fazer com que um pacote de S para T sempre seja encaminhado via DR, ou seja, a mensagem segue sempre a melhor rota de S até DR e, em seguida, de DR até T (mesmo que, no primeiro trecho S~>DR, ele já passe por T).

Cada linha (i,j) da rede possui tem uma largura de banda (bandwidth) B[i,j] medida em Kbps (Kilobits por segundo, 1Kbit = 2^10 bits). Para se calcular as melhores rotas, deve ser usado o Algoritmo Dijkstra atribuindo-se a cada linha um custo inversamente proporcional à sua largura de banda:
C[i,j] = 2^20 / B[i,j].

Ou seja, o custo é calculado como uma estimativa do tempo gasto para se transmitir 1Gigabit = 2^30 bits pela linha. Repare que esse custo deve ser computado como um valor inteiro. A rota ótima entre S e T é definida como o caminho S~~>DR~~>T de menor custo total.

<Input Specification>
A primeira linha da entrada contem um número
K
correspondente ao número de casos testados, numerados 0..K-1. Segue a especificação dos K casos, cada um correspondendo a uma configuração de uma rede com N nós, seguida da especificação de M mensagens. Portanto, cada caso tem a forma

N DR
D[0] E[0,0] B[0,0] E[0,1] B[0,1] ... E[0,D[0]] B[0,D[0]-1]
D[1] E[1,0] B[1,0] E[1,1] B[1,1] ... E[1,D[1]] B[1,D[1]-1]
...

D[N-1] E[N-1,0] B[N-1,0] E[N-1,1] B[N-1,1] ... E[N-1,D[N-1]] B[N-1,D[N-1]-1]
M
S[0] T[0] Z[0]
S[1] T[1] Z[1]
...

S[M-1] T[M-1] Z[M-1]
// linha em branco

onde:
N = número de nós da rede
DR = roteador designado
D[i] = número de nós adjacentes ao nó i
E[i,j] B[i,j] = especifica uma linha entre o nó i e o nó E[i,j] com largura de banda B[i,j]
M = quantidade de mensagens especificadas
S[i] = emissor da i-ésima mensagem
T[i] = receptor da i-ésima mensagem
Z[i] = tamanho da i-ésima mensagem em Kilobits

<Output Specification>
Para cada caso C=0..K-1 deve ser impressa a saída correspondente na forma:
caso C:
H[0] TT[0]
H[1] TT[1]
...
H[M-1] TT[M-1]
// linha em branco

onde:
H[i] = número de conexões entre nós adjacentes (hops = #arestas) pelos quais a mensagem passa na sua rota do emissor até o receptor.
TT[i] = estimativa do tempo de transmissão da mensagem em segundos.

O tempo de transmissão da mensagem é estimado como:
(SC * Z) / (2^20)

onde:
SC é a soma dos custos C[i,j] entre os nós na rota ótima escolhida pelo algoritmo (medida em Gigabits por segundo)
Z é o tamanho da mensagem em Kilobits

Por exemplo, suponha que a rota ótima escolhida do nó U ao nó W seja U-V-W (onde V é o roteador designado), sendo que o o link de U para V é um canal de 10Kbps, e o link de V a W é um canal de 20Kbps.

U <===== 10 Kbps ======> V <===== 20 Kbps ======> W

Nesse caso, teríamos C[U,V]=(2^20)/10 = 104857 Gb/s e C[V,W] = 52428 Gb/s. Assim, teríamos SC = 104857 + 52428 = 157285 Gb/s. Repare que essa seria uma estimativa do tempo para transmitir 1Gigabit de U para W pela rota ótima. Suponha que quiséssemos transmitir uma mensagem de 210 Kb de U para V. Nesse caso, o tempo estimado seria (157285 * 210) / 2^20 = 33029850 / 2^20 = 31s (repare nas divisões inteiras).
*/

#include <iostream>
#include <limits.h>

struct _node
{
    int vert, weight;
    _node* next;
};
typedef struct _node node;

typedef struct
{
    node* head;
    node* tail;
}queue;

typedef struct
{
    queue** arr;
    int verts;
}graph;

typedef struct
{
    int vert, weight;
}heap_entry;

typedef struct
{
    heap_entry* harr;
    int capacity, size;
}heap;

typedef struct
{
    heap* h;
    heap_entry v;
}outputs_he;

typedef struct
{
    int* distances;
    int* precursors;
}outputs_d;

queue* new_queue();

node* enqueue(node* tail, int vert, int weight);

graph* new_graph(int size);

heap* new_heap(int size);

heap* bubble_up(heap* h, int pos);

heap* bubble_down(heap* h, int pos);

heap* heap_insert(heap* h, int vert, int weight);

outputs_he heap_extract(heap* h);

heap* heap_update(heap* h, int vert, int weight);

outputs_d dijkstra(graph* g, int vert);

int edges(int* arr, int destination, int origin);

int main(void)
{
    int K;
    std::cin >> K;

    for (int i = 0; i < K; i++)
    {
        int N, DR, D, E, B, M, S, T, L;

        std::cin >> N;
        std::cin >> DR;

        graph* G = new_graph(N);

        for(int j = 0; j < N; j++)
        {
            std::cin >> D;

            for (int k = 0; k < D; k++)
            {
                std::cin >> E;
                std::cin >> B;

                G->arr[j]->tail = enqueue(G->arr[j]->tail, E, (1<<20)/B);
            }
        }

        std::cout << "caso " << i << ':' << std::endl;

        std::cin >> M;
        int H, TT;
        outputs_d routes = dijkstra(G, DR);

        for (int j = 0; j < M; j++)
        {
            std::cin >> S;
            std::cin >> T;
            std::cin >> L;

            int dist_SDR = routes.distances[S];
            int dist_DRT = routes.distances[T];

            H = edges(routes.precursors, S, DR) + edges(routes.precursors, T, DR);
            TT = ((dist_SDR + dist_DRT)*L)/(1<<20);

            std::cout << H << ' ' << TT << std::endl;
        }
        std::cout << std::endl;

        for (int i = 0; i < N; i++)
        {
            node* cur = G->arr[i]->head;
            node* next = NULL;
            while (cur != NULL)
            {
                next = cur->next;
                delete cur;
                cur = next;
            }
            G->arr[i]->head = NULL;
            G->arr[i]->tail = NULL;
        }
    }
}

queue* new_queue()
{
    queue* ret = new queue;
    ret->head = new node;
    ret->head->next = NULL;
    ret->tail = ret->head;
    ret->head->vert = -1;
    ret->head->weight = -1;

    return ret;
}

node* enqueue(node* tail, int vert, int weight)
{
    node* N = new node;
    N->vert = vert;
    N->weight = weight;
    N->next = NULL;
    tail->next = N;

    return N;
}

graph* new_graph(int size)
{
    graph* ret = new graph;
    ret->arr = new queue*[size];
    for (int i = 0; i < size; i++)
    {
        ret->arr[i] = new_queue();
    }
    ret->verts = size;

    return ret;
}

heap* new_heap(int capacity)
{
    heap* ret = new heap;
    ret->harr = new heap_entry[capacity];
    ret->capacity = capacity;
    ret->size = 0;

    return ret;
}

heap* bubble_up(heap* h, int pos)
{
    int i = pos;
    while (i > 0 && h->harr[i].weight < h->harr[(i-1)/2].weight)
    {
        heap_entry tmp = h->harr[i];
        h->harr[i] = h->harr[(i-1)/2];
        h->harr[(i-1)/2] = tmp;
        i = (i-1)/2;
    }

    return h;
}

heap* bubble_down(heap* h, int pos)
{
    int l = 2*pos + 1;
    int r = 2*pos + 2;
    int m = pos;

    if (l < h->size && h->harr[l].weight < h->harr[m].weight)
    {
        m = l;
    }

    if (r < h->size && h->harr[r].weight < h->harr[m].weight)
    {
        m = r;
    }

    if (m != pos)
    {
        heap_entry tmp = h->harr[m];
        h->harr[m] = h->harr[pos];
        h->harr[pos] = tmp;

        return bubble_down(h, m);
    }

    return h;
}

heap* heap_insert(heap* h, int vert, int weight)
{
    h->harr[h->size].vert = vert;
    h->harr[h->size].weight = weight;
    h = bubble_up(h, h->size);
    h->size++;

    return h;
}

outputs_he heap_extract(heap* h)
{
    heap_entry r = h->harr[0];
    h->harr[0] = h->harr[h->size-1];
    h->harr[h->size-1] = r;
    h->size--;
    bubble_down(h, 0);

    return {h, r};
}

heap* heap_update(heap* h, int vert, int weight)
{
    for (int i = 0; i < h->size; i++)
    {
        if (h->harr[i].vert == vert)
        {
            h->harr[i].weight = weight;
            h = bubble_up(h, i);

            return h;
        }
    }
    h = heap_insert(h, vert, weight);

    return h;
}

outputs_d dijkstra(graph* g, int vert)
{
    int* distances = new int[g->verts];
    int* precursors = new int[g->verts];
    for (int i = 0; i < g->verts; i++)
    {
        distances[i] = INT_MAX;
        precursors[i] = -1;
    }
    distances[vert] = 0;
    
    heap* h = new_heap(g->verts);
    h = heap_insert(h, vert, distances[vert]);

    for (int i = 0; i < g->verts; i++)
    {
        outputs_he d_u = heap_extract(h);
        heap_entry u = d_u.v;
        node* e = g->arr[u.vert]->head->next;

        while (e != NULL)
        {
            int v = e->vert;
            int w = e->weight;

            if (distances[u.vert] + w < distances[v])
            {
                distances[v] = distances[u.vert] + w;
                precursors[v] = u.vert;

                h = heap_update(h, v, distances[v]);
            }
            e = e->next;
        }
    }

    return {distances, precursors};
}

int edges(int* arr, int destination, int origin)
{
    int ret = 0;
    int i = destination;
    while (i != origin)
    {
        i = arr[i];
        ret++;
    }

    return ret;
}