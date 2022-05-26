/*
<Problem Statement>
Num planeta de uma galáxia distante, os cemitérios são verticais, organizados em M andares numerados 0,...,M-1. A tradição local manda que qualquer edifício público, como os cemitérios, tenha uma quantidade ímpar de andares, no mínimo 13 (número da sorte local). Cada andar F=0,..,M-1 dispõe de uma quantidade limitada L de jazigos, rotulados F.0, F.1,..., F.(L-1).

Quando um indivíduo desse planeta morre, ele é levado para o cemitério da sua área de residência e colocado num jazigo com base numa chave numérica única K correspondente ao seu número de seguro social, de acordo com a seguinte política:

Primeiro, um andar escolhido de acordo com a função H(K) = K mod M;
Se houver ainda algum jazigo disponível no andar, o corpo é inserido de forma que os jazigos estejam ocupados em ordem crescente pelo no. de seguro social, isto é, para qualquer andar F, temos F.0.K < F.1.K < ... < F.B.K, onde B é o número de jazigos ocupados no andar e F.J.K indica o número do seguro social do ocupante do jazigo F.J
Se não houver mais jazigos disponíveis no andar original H(K), o próximo andar (H(K)+1) mod M é tentado, depois o seguinte (H(K)+2) mod M, e assim por diante. Assim que um andar com jazigo disponível é encontrado, o corpo é inserido conforme a regra 2 acima.
Se não houver mais nenhum jazigo disponível no cemitério, a lei local manda que um novo prédio de 2M+1 andares, com os mesmos L túmulos por andar, seja imediatamente construído no local (a tecnologia é avançadíssima e a construção é imediatamente feita por robôs) e todos os corpos de todos os jazigos, pela ordem, 0.1, 0.2,.., 0.(L-1),...,(M-1).0,...,(M-1).(L-1), são redistribuídos pelo novo prédio de acordo com esta mesma política. IMPORTANTE: durante a redistribuição, os túmulos vazios são ignorados. Depois que todos os corpos forem realocados, o novo corpo recém-chegado é acomodado.

É também muito comum pelos costumes locais que, após algum tempo, as famílias retirem os restos mortais dos seus entes queridos para cremá-los e jogarem as cinzas ao mar. Entretanto, a religião local proíbe que um mesmo jazigo seja utilizado por mais de uma pessoa e, portanto, uma vez removidos os despojos, o túmulo é lacrado e fica inutilizado. Após a retirada, o jazigo já não está de fato ocupado mas, para efeito de ordenação, esse túmulo mantém o no. do seguro social do seu ex-ocupante e continua indisponível.

A administração precisa então desenvolver um sistema informático que, entre outras coisas, permita descobrir rapidamente em qual jazigo repousa o corpo de um indivíduo dado seu número de seguro social, ou indicar que essa pessoa não está no cemitério.

<Input Specification>
A primeira linha da entrada é constituída por dois inteiros:
M L

correspondentes ao número inicial de andares e ao número de jazigos por andar do cemitério, respectivamente. Supõe-se que o cemitério esteja inicialmente vazio.

Seguem-se N linhas, cada linha correspondente a uma operação numa das formas a seguir:
ADD K = adiciona o indivíduo com chave K ao cemitério
REM K = remove o corpo com chave K do cemitério, se ele estiver no cemitério
QRY K = retorna o jazigo do corpo com chave K, se houver

A entrada termina com uma linha:
END

<Output Specification>
Para cada operação na entrada, deve ser impressa uma linha dois inteiros na forma:
F.J

onde F corresponde ao andar e J o no. do jazigo correspondente à operação:
ADD K --> jazigo no qual o indivíduo com chave K foi colocado
REM K --> jazigo do qual os despojos com chave K foram removidos
QRY K --> jazigo no qual encontra-se o corpo com chave K

No caso das operações REM ou QRY, caso o indivíduo com seguro social K não esteja no cemitério (incluindo o caso em que foi retirado para cremação), deve ser impressa uma linha:
?.?
*/

#include <iostream>
#include <assert.h>

typedef struct
{
    int key;
    bool was_taken;
}tomb;

typedef struct
{
    int size;
    tomb* arr;
}floor;

typedef struct
{
    int size;
    int floor_capacity;
    floor* arr;
}htable;

typedef struct
{
    int f, t;
}positions;

typedef struct
{
    int f, t;
    htable* new_table;
}positions_insert;

htable* new_table(int size, int floor_capacity);

positions_insert hash_insert(htable* table, int key);

positions hash_query(htable* table, int key);

positions hash_remove(htable* table, int key);

int get_postition(tomb* arr, int size, int x);

int floor_insert(floor* f, int key);

htable* rehashing(htable* table);

int binary_search(tomb arr[], int size, int x);

int main(void)
{
    int M, L;
    std::cin >> M;
    std::cin >> L;

    htable* table = new_table(M, L);

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
            int K;
            std::cin >> K;

            positions_insert pos_insert = hash_insert(table, K);

            table = pos_insert.new_table;

            std::cout << pos_insert.f << '.' << pos_insert.t << std::endl;
        }
        else if (command[0] == 'R')
        {
            int K;
            std::cin >> K;

            positions pos = hash_remove(table, K);

            if (pos.f < 0)
            {
                std::cout << "?.?" << std::endl;
            }
            else
            {
                std::cout << pos.f << '.' << pos.t << std::endl;
            }
        }
        else
        {
            int K;
            std::cin >> K;

            positions pos = hash_query(table, K);

            if (pos.f < 0)
            {
                std::cout << "?.?" << std::endl;
            }
            else
            {
                std::cout << pos.f << '.' << pos.t << std::endl;
            }
        }
    }

    for (int i = 0; i < table->size; i++)
    {
        delete[] table->arr[i].arr;
    }
    delete[] table->arr;
    delete table;
    table = NULL;
}

htable* new_table(int size, int floor_capacity)
{
    htable* table = new htable;
    table->size = size;
    table->floor_capacity = floor_capacity;
    table->arr = new floor[size];

    for (int i = 0; i < size; i++)
    {
        table->arr[i].size = 0;
        table->arr[i].arr = new tomb[floor_capacity];
    }

    return table;
}

positions_insert hash_insert(htable* table, int key)
{
    int f = (key % table->size);

    for (int i = 0; i < table->size; i++)
    {
        if (table->arr[f].size < table->floor_capacity)
        {
            int t = floor_insert(&table->arr[f], key);
            return {f, t, table};
        }

        f = ((f+1) % table->size);
    }

    htable* new_table = rehashing(table);
    return hash_insert(new_table, key);
}

positions hash_query(htable* table, int key)
{
    int f = (key % table->size);

    for (int i = 0; i < table->size; i++)
    {
        int pos = binary_search(table->arr[f].arr, table->arr[f].size, key);

        if (pos >= 0 && table->arr[f].arr[pos].was_taken) //found key
        {
            return {f, pos};
        }

        if (table->arr[f].size < table->floor_capacity) //found empty tomb
        {
            return {-1, -1};
        }

        f = ((f+1) % table->size);
    }

    return {-1, -1};
}

positions hash_remove(htable* table, int key)
{
    positions pos = hash_query(table, key);

    if (pos.f == -1)
    {
        return {-1, -1};
    }

    table->arr[pos.f].arr[pos.t].was_taken = false;
    return pos;
}

int get_postition(tomb* arr, int size, int x)
{
    int l = 0;
    int r = size-1;

    while (l <= r)
    {
        int m = (l+r)/2;

        if (x == arr[m].key)
        {
            return m;
        }
        else if (x < arr[m].key)
        {
            r = m-1;
        }
        else
        {
            l = m+1;
        }
    }

    assert(l == 0 || arr[l-1].key < x);
    assert(l == size || arr[l].key > x);

    return l;
}

int floor_insert(floor* f, int key)
{
    int pos = get_postition(f->arr, f->size, key);
    for (int i = f->size; i >= pos; i--)
    {
        f->arr[i] = f->arr[i - 1];
    }
    f->arr[pos].key = key;
    f->arr[pos].was_taken = true;
    f->size++;

    return pos;
}

htable* rehashing(htable* table)
{
    htable* new_htable = new_table(2*(table->size)+1, table->floor_capacity);

    for (int i = 0; i < table->size; i++)
    {
        for (int j = 0; j < table->floor_capacity; j++)
        {
            if (table->arr[i].arr[j].was_taken == true)
            {
                positions_insert pos = hash_insert(new_htable, table->arr[i].arr[j].key);
            }
        }
    }

    return new_htable;
}

int binary_search(tomb* arr, int size, int x)
{
    int l = 0;
    int r = size-1;

    while (l <= r)
    {
        int m = (l+r)/2;

        if (x == arr[m].key)
        {
            return m;
        }
        else if (x < arr[m].key)
        {
            r = m-1;
        }
        else
        {
            l = m+1;
        }
    }

    return -1;
}