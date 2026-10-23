/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#pragma once
#include <pthread.h>

#include "bcp.h"

typedef struct tcb TCB;

/*
Função: 
    tcb_create
Descrição: 
    Cria um TCB (Thread Control Block) associado ao BCP do processo
Entrada: 
    BCP* bcp: ponteiro para o BCP (Bloco de Controle do Processo)
    double total_time: tempo total de CPU que a thread precisa executar (em ms)
    int id: identificador da thread
Saída: 
    TCB*: ponteiro para o TCB criado ou NULL em caso de falha
*/
TCB *tcb_create(BCP *bcp, double total_time, int id);

/* Destroi o TCB: faz pthread_join na thread interna e libera recursos. */
/*
Função: 
    tcb_destroy
Descrição: 
    Destroi o TCB e libera todos os recursos associados
Entrada: 
    TCB* tcb: ponteiro para o TCB a ser destruído
Saída: 
    void
*/
void tcb_destroy(TCB *tcb);

/* Getters / setters thread-safe (uso geral) */
/*
Função: 
    tcb_get_bcp
Descrição: 
    Obtém o BCP associado ao TCB
Entrada: 
    TCB* tcb: ponteiro para o TCB
Saída: 
    BCP*: ponteiro para o BCP associado ou NULL se não existir
*/
BCP *tcb_get_bcp(TCB *tcb);

/*
Função: 
    tcb_get_total_time
Descrição: 
    Obtém o tempo total restante que a thread precisa executar
Entrada: 
    TCB* tcb: ponteiro para o TCB
Saída: 
    double: tempo total restante em milissegundos ou -1 em caso de erro
*/
double tcb_get_total_time(TCB *tcb);

/*
Função: 
    tcb_get_executed_time
Descrição: 
    Obtém o tempo já executado pela thread
Entrada: 
    TCB* tcb: ponteiro para o TCB
Saída: 
    double: tempo executado em milissegundos ou -1 em caso de erro
*/
double tcb_get_executed_time(TCB *tcb);

/*
Função: 
    tcb_add_executed_time
Descrição: 
    Incrementa o tempo executado e decrementa o tempo total restante da thread
Entrada: 
    TCB* tcb: ponteiro para o TCB
    double quantum: quantidade de tempo a ser adicionada/removida
Saída: 
    void
*/
void   tcb_add_executed_time(TCB *tcb, double quantum); /* incrementa remaining e decrementa total */

/*
Função: 
    tcb_is_terminated
Descrição: 
    Verifica se a thread terminou sua execução
Entrada: 
    TCB* tcb: ponteiro para o TCB
Saída: 
    int: 1 se a thread terminou, 0 caso contrário
*/
int    tcb_is_terminated(TCB *tcb);

/*
Função: 
    tcb_get_ID
Descrição: 
    Obtém o identificador da thread
Entrada: 
    TCB* tcb: ponteiro para o TCB
Saída: 
    int: identificador da thread ou -1 em caso de erro
*/
int tcb_get_ID(TCB *tcb);

/*
Função: 
    tcb_set_terminated
Descrição: 
    Marca a thread como terminada
Entrada: 
    TCB* tcb: ponteiro para o TCB
Saída: 
    void
*/
void   tcb_set_terminated(TCB *tcb);

/* Função chamada pela CPU para executar um quantum nesta TCB.*/
/*
Função: 
    tcb_working_thread
Descrição: 
    Função chamada pela CPU para executar um quantum de tempo na thread
Entrada: 
    TCB* tcb: ponteiro para o TCB
    double quantum: quantum de tempo a ser executado (em ms)
Saída: 
    void
*/
void tcb_working_thread(TCB *tcb, double quantum);
