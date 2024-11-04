#include "arvore_b/arvore.h"

int main()
{
    int num,deletar,ordem;
    arvore_b *raiz = NULL;
    scanf("%i", &ordem);
    raiz = cria_arvore(ordem);
    do
    {
        scanf("%i",&num);
        if(num != -1)
        {
            insere_arvore_b(raiz,num);
        }
    } while (num != -1);
    
    return 0;
}