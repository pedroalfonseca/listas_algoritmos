/*
<Problem Statement>
Elliot Alderson planeja mais um hack contra a Evil Corp e, para isso, precisa fazer uma série de pesquisas. Dessa forma, ele planeja usar o seu próprio navegador, de modo que as informações fiquem mais organizadas e seguras.

O navegador de Elliot mantém a sequência de páginas abertas num histórico. Cada nova página visitada é acrescentada ao final do histórico e, a cada instante, é possível voltar da página atual à página visitada anteriormente, fazendo com que a página atual seja removida do histórico.

Infelizmente, Elliot está sendo perseguido e precisa ter um comando de emergência em caso de risco de ser espionado por um agente do Dark Army. Esse comando apaga imediatamente todo o histórico do navegador.

Além disso, é possível salvar uma página corrente para ser lida posteriormente. As páginas salvas são copiadas para uma memória criptografada à parte, e podem ser lidas com segurança posteriormente na mesma ordem em que foram salvas.

<Input Specification>
A entrada é constituída por várias linhas, cada um com um comando de navegação:

OPN U: Abre (visita) a url U, acrescentando-a ao histórico. Essa passa a ser a página corrente.
CUR: Imprime a url da página corrente do navegador
BCK: Remove a página corrente do histórico e volta à página anterior.
PAN: Comando de "panico" que limpa o histórico do navegador
SAV: Salva a página atual para leitura posterior.
REA: Lê a próxima página salva ainda não lida, retirando-a da lista de páginas salvas.
END: Fim da entrada

-O navegador inicia com um histórico vazio
-Cada url terá no máximo 10 caracteres.

<Output Specification>
Para cada comando da entrada, deve ser impressa a saída especificada a seguir:

OPN: Imprime uma linha com a quantidade de páginas no histórico, incluindo a página aberta por este comando.
CUR: Imprime uma linha com a url da página corrente do navegador.
BCK: Imprime uma linha com a url da página anterior para a qual voltamos. Caso não haja página anterior, imprimir uma linha com a palavra NULL
PAN: Imprime a quantidade de páginas apagadas do histórico.
SAV: Imprime uma linha com a quantidade de página salvas ainda não lidas, incluindo a página salva por esse comando.
REA: Imprime uma linha com a url da página lida e apagada da lista de páginas salvas. Caso não haja página salva para leitura, imprimir uma linha com a palavra NULL
END: Fim da entrada
*/

#include <iostream>
#include <string>

struct _node
{
    std::string url;
    struct _node* next;
};
typedef struct _node node;

node* list_insert(node* cur, std::string new_url);

node* list_delete(node* cur);

node* enqueue(node* tail, std::string new_url);

int counter(node* head);

int main(void)
{
    node* top;
    top = new node;
    top->next = NULL;

    node* head;
    head = new node;
    head->next = NULL;

    node* tail = head;

    int counter_history = 0;
    int counter_pocket = 0;

    std::string command;
    while (true)
    {
        getline(std::cin, command);

        if (command[0] == 'O')
        {
            std::string url;
            int pos = command.find(" ");

            url = command.substr(pos+1);

            top = list_insert(top, url);

            counter_history++;

            std::cout << counter_history << std::endl;
        }
        else if (command[0] == 'C')
        {
            if (top->next == NULL)
            {
                std::cout << "NULL" << std::endl;
            }
            else
            {
                std::cout << top->next->url << std::endl;
            }
        }
        else if (command[0] == 'B')
        {
            top = list_delete(top);

            if (counter_history > 0)
            {
                counter_history--;
            }
            
            if (top->next == NULL)
            {
                std::cout << "NULL" << std::endl;
            }
            else
            {
                std::cout << top->next->url << std::endl;
            }
        }
        else if (command[0] == 'P')
        {
            std::cout << counter_history << std::endl;
            top->next = NULL;

            counter_history = 0;
        }
        else if (command[0] == 'S')
        {
            if (top->next == NULL)
            {
                std::cout << counter_pocket << std::endl;
            }            
            else
            {
                tail = enqueue(tail, top->next->url);

                counter_pocket++;

                std::cout << counter_pocket << std::endl;
            }
        }
        else if (command[0] == 'R')
        {
            if (head->next == NULL)
            {
                std::cout << "NULL" << std::endl;
            }
            else
            {
                std::cout << head->next->url << std::endl;
                head = list_delete(head);

                if (counter_pocket > 0)
                {
                    counter_pocket--;
                }

                if (head->next == NULL)
                {
                    tail = head;
                }
            }
        }
        else if (command[0] == 'E')
        {
            break;
        }
    }

    node* cur = head;
    node* next = NULL;
    while (cur != NULL)
    {
        next = cur->next;
        delete cur;
        cur = next;
    }
    head = NULL;
    tail = NULL;

    cur = top;
    next = NULL;
    while (cur != NULL)
    {
        next = cur->next;
        delete cur;
        cur = next;
    }
    top = NULL;
}

node* list_insert(node* cur, std::string new_url)
{
    node* N;
    N = new node;
    N->url = new_url;
    N->next = cur->next;
    cur->next = N;

    return cur;
}

node* list_delete(node* cur)
{
    if (cur->next != NULL)
    {
        node* p = cur->next;
        cur->next = p->next;
        delete p;
    }
    
    return cur;
}

node* enqueue(node* tail, std::string new_url)
{
    node* r = list_insert(tail, new_url);

    return r->next;
}

int counter(node* head)
{
    node* tmp = head;
    int count = 0;

    while (tmp->next != NULL)
    {
        count++;
        tmp = tmp->next;
    }

    return count;
}