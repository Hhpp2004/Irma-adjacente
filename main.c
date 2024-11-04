#include "arvore_b/arvore.h"

int main()
{
    int num,deletar,ordem;
    arvore_b *raiz = NULL;
    scanf("%i", &ordem);
    raiz = cria_arvore(ordem);
    scanf("%i",&deletar);
    do
    {
        scanf("%i",&num);
        if(num != -1)
        {
            insere_arvore_b(raiz,num);
        }
    } while (num != -1);    
    printf("\n");
    tudo(raiz->raiz,deletar,ordem);
    printf("\n");
    em_ordem(raiz->raiz);
    printf("\n");
    return 0;
}