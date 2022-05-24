/*
<Problem Statement>
Buendía está tentando acessar a conta de seu jogo favorito online, infelizmente ele esqueceu a senha, e pra piorar o site não possui 'esqueci a senha'. Embora ele não se lembre da senha, Buendía se lembra que a conta era formada por números e algumas das palavras escritas no verso do seu caderno.

Além disso ele tem um conjunto de possíveis formatos para a senha (disposição de números e palavras), onde:

-X representa uma palavra que pertence ao conjunto das palavras no verso do caderno.
-# representa um número entre 0 e 9.

Você deve ajuda-lo a gerar todas as possíveis senhas, que devem ser listadas seguindo uma determinada ordem.

<Input Specification>
O primeiro valor é um inteiro N, informando o número de palavras no verso do caderno.
As próximas N linhas irão conter uma string referente a uma palavra do caderno.
Seguem Q linhas contendo um formato cada.

<Output Specification>
Para cada um dos Q formatos, deve-se imprimir todas as possíveis senhas. Onde cada # do formato irá variar entre a primeira e a última palavra escritas no caderno. Bem como cada X deve variar de 0 a 9.

Cada possível senha deve ser seguida de quebra de linha.
*/

#include <iostream>

void pattern(std::string str, int index, std::string prefix);

std::string nums[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
std::string notebook[100];
int notebook_size;

int main(void)
{
    std::cin >> notebook_size;

    for (int i = 0; i < notebook_size; i++)
    {
        std::cin >> notebook[i];
    }

    std::string formato;
    while (std::cin >> formato)    
    {
        std::string prefix = "";
        pattern(format, 0, prefix);
    }
}

void pattern(std::string str, int index, std::string prefix)
{
    int m = (str[index] == '#') ? 10 : (notebook_size);

    for (int i = 0; i < m; i++)
    {
        std::string prefix2 = prefix + ((str[index] == '#')? nums[i]: notebook[i]);

        if (index == str.length() - 1)
        {
            std::cout << prefix2 << std::endl;
        }
        else
        {
            pattern(str, index + 1, prefix2);
        }
    }
}
