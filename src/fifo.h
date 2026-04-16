/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#pragma once

#include <stdlib.h>
#include <pthread.h>
#include "bcp.h"

typedef struct fifo FIFOQueue;

/*
Função: 
    fifo_create
Descrição: 
    Cria e inicializa uma nova FIFO
Entrada: 
    void
Saída: 
    FIFOQueue*: ponteiro para a fila criada ou NULL em caso de falha
*/
FIFOQueue* fifo_create();

/*
Função: 
    fifo_destroy
Descrição: 
    Destroi a fila FIFO e libera todos os recursos associados
Entrada: 
    FIFOQueue* queue: ponteiro para a fila a ser destruída
Saída: 
    void
*/
void fifo_destroy(FIFOQueue* queue);

/*
Função: 
    fifo_push
Descrição: 
    Insere um elemento no final da fila FIFO
Entrada: 
    FIFOQueue* queue: ponteiro para a fila
    void* data: ponteiro para os dados a serem inseridos
Saída: 
    void
*/
void fifo_push(FIFOQueue* queue, void* data);

/*
Função: 
    fifo_pop
Descrição: 
    Remove e retorna o elemento do início da fila FIFO
Entrada: 
    FIFOQueue* queue: ponteiro para a fila
Saída: 
    void*: ponteiro para os dados removidos ou NULL se a fila estiver vazia
*/
void* fifo_pop(FIFOQueue* queue);

/*
Função: 
    fifo_peek
Descrição: 
    Retorna o elemento do início da fila FIFO sem removê-lo
Entrada: 
    FIFOQueue* queue: ponteiro para a fila
Saída: 
    void*: ponteiro para os dados do início ou NULL se a fila estiver vazia
*/
void* fifo_peek(FIFOQueue* queue);

/*
Função: 
    fifo_is_empty
Descrição: 
    Verifica se a fila FIFO está vazia
Entrada: 
    FIFOQueue* queue: ponteiro para a fila
Saída: 
    int: 1 se a fila estiver vazia, 0 caso contrário
*/
int fifo_is_empty(FIFOQueue* queue);

/*
Função: 
    fifo_size
Descrição: 
    Retorna o número de elementos na fila FIFO
Entrada: 
    FIFOQueue* queue: ponteiro para a fila
Saída: 
    int: número de elementos na fila
*/
int fifo_size(FIFOQueue* queue);