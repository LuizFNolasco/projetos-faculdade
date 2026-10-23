/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#pragma once

#include <stdbool.h>
#include <pthread.h>

typedef struct heap Heap;

/*
Função: 
    heap_create
Descrição: 
    Cria e inicializa uma nova estrutura de heap (fila de prioridade)
Entrada: 
    int capacity: capacidade inicial do heap
    int (*priority_fn)(void *a, void *b): função de comparação de prioridade
Saída: 
    Heap*: ponteiro para o heap criado ou NULL em caso de falha
*/
Heap *heap_create(int capacity, int (*priority_fn)(void *a, void *b));

/*
Função: 
    heap_destroy
Descrição: 
    Destroi o heap e libera todos os recursos associados
Entrada: 
    Heap* heap: ponteiro para o heap a ser destruído
Saída: 
    void
*/
void heap_destroy(Heap *heap);

/*
Função: 
    heap_insert
Descrição: 
    Insere um novo elemento no heap mantendo a propriedade de heap
Entrada: 
    Heap* heap: ponteiro para o heap
    void* value: ponteiro para o valor a ser inserido
Saída: 
    void
*/
void heap_insert(Heap *heap, void *value);

/*
Função: 
    heap_extract_min
Descrição: 
    Remove e retorna o elemento com maior prioridade (menor valor) do heap
Entrada: 
    Heap* heap: ponteiro para o heap
Saída: 
    void*: ponteiro para o elemento extraído ou NULL se o heap estiver vazio
*/
void *heap_extract_min(Heap *heap);

/*
Função: 
    heap_peek
Descrição: 
    Retorna o elemento com maior prioridade sem removê-lo do heap
Entrada: 
    Heap* heap: ponteiro para o heap
Saída: 
    void*: ponteiro para o elemento de maior prioridade ou NULL se o heap estiver vazio
*/
void *heap_peek(Heap *heap);

/*
Função: 
    heap_is_empty
Descrição: 
    Verifica se o heap está vazio
Entrada: 
    Heap* heap: ponteiro para o heap
Saída: 
    bool: true se o heap estiver vazio, false caso contrário
*/
bool heap_is_empty(Heap *heap);