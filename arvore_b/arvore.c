#include "arvore.h"
/*TODO
Arquivo de implementação das funções
Como boa pratica de programação todas as funções devem ser
feitas em ordem de chamar como exemplo
encontra_folha tem que ser implementada primeiro e
acima da função de insere_arvore
*/

// TODO função que cria a arvore B
arvore_b *cria_arvore(int ordem)
{
    arvore_b *arvore = malloc(sizeof(arvore_b));
    arvore->raiz = NULL;
    arvore->ordem = ordem;
    arvore->altura = 0;
    return arvore;
}

/*
TODO função que cria a pagina e a lista duplamente encadeada cuja a sua função está no arquivo lista_dupla.c para salvar
os dados. Deverá criar a lista aqui na função de criação da pagina.
*/
pagina *cria_pagina()
{
    pagina *p = malloc(sizeof(pagina));
    p->folha = 1;
    p->qtd_chaves = 0;
    p->pai = NULL;
    p->lista = cria_lista();
    p->dir = NULL;
    return p;
}

// TODO criação da chave.
chave *cria_chave(int valor)
{
    chave *chave = malloc(sizeof(valor));
    chave->valor_chave = valor;
    chave->filho = NULL;
    return chave;
}

/*
TODO conversão das variaveis o metodo GET é
muito utilizado nas linguagens de orientação a objetos
Todas as funções com o metodo get será para conversão de
tipo de variavel.
*/
int getvalor(no *aux)
{
    return ((chave *)aux->info)->valor_chave;
}

pagina *getfilho(no *aux)
{
    return ((chave *)aux->info)->filho;
}

// TODO Função que faz a inserção e ordena os dados
void insere_ordenado(lista *l, chave *valor)
{
    no *aux = l->inicio;
    no *novo_no = NULL;

    // TODO verificanco se tem dados na lista para a primeira inserção
    if (l->inicio == NULL)
    {
        insere_inicio(valor, l);
    }
    /*
    TODO caso tenha valor salvo será feito a inserção
    já ordenando os valores
    */
    else
    {
        if (valor->valor_chave < getvalor(aux))
        {
            insere_inicio(valor, l);
        }
        else if (valor->valor_chave > getvalor(l->fim))
        {
            insere_fim(valor, l);
        }
        else
        {
            while (valor->valor_chave > getvalor(aux))
            {
                aux = aux->prox;
            }
            novo_no = cria_no(valor);
            novo_no->ant = aux->ant;
            novo_no->prox = aux;
            aux->ant = novo_no;
            novo_no->ant->prox = novo_no;
        }
    }
}
/*
TODO a função encontra folha é uma função de achar todas
as folhas para fazer a inserção.
primeiro verifico se a pagina é folha, depois percorro a
lista para achar um valor menor do que os valores que estão
já alocados na lista, caso não encontre a pagina é salva
a direita caso contrario a propria pagina salva o valor na lista dentro da pagina.
*/
pagina *encontra_folha(pagina *raiz, int valor)
{
    pagina *aux = raiz;
    if (raiz == NULL)
    {
        return NULL;
    }
    else
    {
        // TODO valor 1 para folha e 0 para não folha
        while (aux->folha != 1)
        {
            no *aux_lista = aux->lista->inicio;
            while (aux_lista != NULL && valor > getvalor(aux_lista))
            {
                aux_lista = aux_lista->prox;
            }
            if (aux_lista == NULL)
            {
                aux = aux->dir;
            }
            else
            {
                aux = getfilho(aux_lista);
            }
        }
        return aux;
    }
}

// TODO função de inserção na pagina
void insere_pagina(pagina *folha, chave *valor)
{
    insere_ordenado(folha->lista, valor);
    folha->qtd_chaves++;
}

/*
TODO função de retirada do ultimo dado de dentro da lista
depois da divisão da lista
*/

chave *ultima_chave(pagina *folha)
{
    no *ultima_chave = retira_fim(folha->lista);
    chave *valor = ultima_chave->info;
    free(ultima_chave);
    folha->qtd_chaves--;
    return valor;
}

chave *primeira_chave (pagina *folha)
{
    no *primeira_chave = retira_inicio(folha->lista);
    chave *valor = primeira_chave->info;
    free(primeira_chave);
    folha->qtd_chaves--;
    return valor;
}

/*TODO
Aqui temos a função de divisão da pagina
-> Será importante a biblioteca math.h para arredondar
a divisão para baixo para a quantidade de valores dentro
da lista;
*/

pagina *divide_pagina(pagina *p)
{
    pagina *nova_pagina = cria_pagina();
    int qtd_pagina = ceil(p->qtd_chaves / 2.0);
    lista *nova_lista = NULL;
    chave *ultima_chave = NULL;

    nova_lista = divide_lista(p->lista, qtd_pagina);
    nova_pagina->lista = nova_lista;
    nova_pagina->qtd_chaves = p->qtd_chaves - qtd_pagina;
    nova_pagina->pai = p->pai;
    nova_pagina->folha = p->folha;
    nova_pagina->dir = p->dir;
    p->qtd_chaves = qtd_pagina;

    ultima_chave = p->lista->fim->info;

    if (p->pai != NULL)
    {
        no *aux = p->pai->lista->inicio;
        while (aux != NULL && getfilho(aux) != p)
        {
            aux = aux->prox;
        }
        /*TODO caso não tenha estourado a pagina,
        as novas paginas são salvas nos filhos
        caso contrario será necessario utilizar a
        variavel direito da pagina.
        */
        if (aux != NULL)
        {
            ((chave *)aux->info)->filho = nova_pagina;
        }
        else
        {
            p->pai->dir = nova_pagina;
        }
    }
    /*TODO caso a pagina não seja folha
    pecorremos a lista para salvar a pagina nova no pai
    */
    if (p->folha == 0)
    {
        no *aux = nova_pagina->lista->inicio;
        while (aux != NULL)
        {
            getfilho(aux)->pai = nova_pagina;
            aux = aux->prox;
        }
        nova_pagina->dir->pai = nova_pagina;
        p->dir = ultima_chave->filho;
    }
    ultima_chave->filho = p;
    return nova_pagina;
}

/*TODO função para criar uma nova raiz*/

pagina *cria_raiz(pagina *folha, pagina *nova_pagina, chave *valor)
{
    pagina *nova_raiz = cria_pagina();
    nova_raiz->folha = 0;
    insere_pagina(nova_pagina, valor);
    valor->filho = folha;
    nova_raiz->dir = nova_pagina;
    nova_pagina->pai = nova_raiz;
    valor->filho->pai = nova_raiz;
    return nova_raiz;
}

/*TODO função para fazer inserção na arvore B*/
void insere_arvore_b(arvore_b *a, int valor)
{
    pagina *pag_aux = a->raiz;
    // TODO o spoj não reconhece a biblioteca stdbool.h
    int valor_insere = 1;
    chave *novo = cria_chave(valor);
    pagina *folha = encontra_folha(pag_aux, valor);
    pagina *nova_pagina = NULL;

    if (folha == NULL)
    {
        a->raiz = cria_pagina();
        insere_pagina(a->raiz, novo);
    }
    else
    {
        while (valor_insere)
        {
            // TODO entrada dos valores dentro da lista
            insere_pagina(folha, novo);
            /*TODO a quantidade de dados dentro da lista
            tem que ser menor que a ordem da arvore, aqui
            temos a verificação da quantidade, caso esse
            valor passe da ordem será necessario chamar a
            função de divisão da lista criada e implementa
            nos arquivos da lista duplamente encadeada.
            */
            if (folha->qtd_chaves < a->ordem)
            {
                valor_insere = 0;
            }
            // TODO estourou a pagina, será necessario a divisão de dados
            else
            {
                nova_pagina = divide_pagina(folha);
                novo = ultima_chave(folha);
                if (a->raiz == folha)
                {
                    a->raiz = cria_raiz(folha, nova_pagina, novo);
                    valor_insere = 0;
                }
                else
                {
                    folha = folha->pai;
                }
            }
        }
    }
}

void em_ordem(pagina *raiz)
{
    no *aux = NULL;
    if (raiz != NULL)
    {
        aux = raiz->lista->inicio;
        while (aux != NULL)
        {
            em_ordem(((chave *)aux->info)->filho);
            printf("%i ", ((chave *)aux->info)->valor_chave);
            aux = aux->prox;
        }
        em_ordem(raiz->dir);
    }
}

/*
TODO
a)   uma função que retorne um ponteiro para a folha irmã adjacente da folha onde está
a chave k que tenha o maior número de chaves. Se a folha com o maior número de chaves
tiver somente o mínimo de chaves (ou seja, não pode perder chaves) a função deve retornar NULL.
Se as duas folhas irmãs adjacentes tiverem o mesmo número de chaves
(e que esse número permita a remoção), sempre use a folhar irmã da esquerda.
*/

pagina *doacao(pagina *pai, int num, int ordem)
{
    // percorrer a lista do pai da folha    
    no *aux = pai->lista->inicio;
    // pecorrer cada lista dos filhos para não salvar a pagina onde tem o número para retirada
    no *aux_2 = NULL;
    int flag = 0;
    pagina *doador = NULL;
    no *aux_dir = pai->dir->lista->inicio;

    while (aux != NULL)
    {
        //percorro a lista do filho, causa o numero para remover estiver, eu vou para o proximo filho 
        aux_2 = getfilho(aux)->lista->inicio;
        while (aux_2 != NULL)
        {            
            if (getvalor(aux_2) == num)
            {
                flag = 1; // o numero está nessa pagina
            }
            aux_2 = aux_2->prox;
        }
        // um possivel doador
        if(flag != 1 && getfilho(aux)->qtd_chaves > ceil(ordem/2))
        {
            doador = getfilho(aux);
        }
        aux = aux->prox;
    }

    flag = 0;
    /*TODO
    caso as outras paginas não conseguem doar, temos que verificar a situação do 
    filho que se encontra na direita
    */
    if(doador == NULL)
    {
        while(aux_dir != NULL)
        {
            if(getvalor(aux_dir) == num)
            {
                flag = 1;
            }
            aux_dir = aux_dir->prox;
        }
        if(flag != 1 && pai->dir->qtd_chaves > ceil(ordem/2) && aux_dir != NULL)
        {
            doador = getfilho(aux_dir);
        }
    }
    //caso ninguem consegue doar, o número é so apagado.
    return doador != NULL? doador:NULL;    
}

/*TODO
b) uma função que dados a chave k a ser removida em uma folha, o ponteiro da folha, 
o ponteiro da folha irmã adjacente com possibilidade de perder chaves (se ela existir)
, remova a chave k e reorganize o que for necessário,
redistribuindo as chaves restantes.
*/

void remocao(lista *l,int num)
{
        no *ant = NULL;
        no *aux = l->inicio;
        while(aux != NULL)
        {
            ant = aux;
            aux = aux->prox;
        }
        if(aux->ant == NULL)
        {
            retira_inicio(l);
        }
        else if(aux->prox == NULL)
        {
            retira_fim(l);
        }
        else 
        {
            ant->prox = aux->prox;
            aux->prox->ant = ant;
            free(aux);
            aux = NULL;
        }
}

void delete(pagina *folha,int num,int ordem)
{    
    pagina *pai = folha->pai;
    pagina *doador = doacao(pai,num,ordem);

    if(doador == NULL)
    {
        remocao(folha->lista,num);
    }
    else
    {
        remocao(folha->lista,num);
        insere_pagina(folha,primeira_chave(pai));
        insere_pagina(pai,primeira_chave(doador));
    }
}

/*TODO
c)    Uma função com o objetivo de remover a chave k, que determine se k 
está numa folha e que chame as funções anteriores para concluir a remoção.
*/

void tudo(pagina *raiz,int num,int ordem)
{
    pagina *folha = encontra_folha(raiz,num);
    delete(folha,num,ordem);
}
