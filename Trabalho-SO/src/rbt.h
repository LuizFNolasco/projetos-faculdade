/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#pragma once

typedef struct TRBTree TRBTree;

/*
Função: 
    RBT_create
Descrição: 
    Cria e inicializa uma nova árvore rubro-negra (Red-Black Tree)
Entrada: 
    int (*cmp)(const void *, const void *): função de comparação para ordenação dos elementos
Saída: 
    TRBTree*: ponteiro para a árvore criada ou NULL em caso de falha
*/
TRBTree *RBT_create(int (*cmp)(const void *, const void *));

/*
Função: 
    RBT_destroy
Descrição: 
    Destroi a árvore rubro-negra e libera todos os recursos associados
Entrada: 
    TRBTree* tree: ponteiro para a árvore a ser destruída
Saída: 
    void
*/
void RBT_destroy(TRBTree *tree);

/*
Função: 
    RBT_insert
Descrição: 
    Insere um novo elemento na árvore rubro-negra
Entrada: 
    TRBTree* tree: ponteiro para a árvore
    void* key: ponteiro para o elemento a ser inserido
Saída: 
    void
*/
void RBT_insert(TRBTree *tree, void *key);

/*
Função: 
    RBT_search
Descrição: 
    Busca um elemento na árvore rubro-negra
Entrada: 
    TRBTree* tree: ponteiro para a árvore
    void* key: ponteiro para o elemento a ser buscado
Saída: 
    void*: ponteiro para o elemento encontrado ou NULL se não existir
*/
void *RBT_search(TRBTree *tree, void *key);

/*
Função: 
    CFS_min
Descrição: 
    Retorna o elemento mínimo da árvore (para uso no escalonador CFS)
Entrada: 
    TRBTree* tree: ponteiro para a árvore
Saída: 
    void*: ponteiro para o elemento mínimo ou NULL se a árvore estiver vazia
*/
void *CFS_min(TRBTree *tree);

/*
Função: 
    RBT_is_empty
Descrição: 
    Verifica se a árvore rubro-negra está vazia
Entrada: 
    TRBTree* tree: ponteiro para a árvore
Saída: 
    int: 1 se a árvore estiver vazia, 0 caso contrário
*/
int RBT_is_empty(TRBTree *tree);

/*
Função: 
    RBT_size
Descrição: 
    Retorna o número de elementos na árvore rubro-negra
Entrada: 
    TRBTree* tree: ponteiro para a árvore
Saída: 
    int: número de elementos na árvore
*/
int RBT_size(TRBTree *tree);