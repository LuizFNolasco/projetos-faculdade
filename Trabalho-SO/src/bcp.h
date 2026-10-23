
/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#pragma once

#include "process.h"
#include <pthread.h>

// Forward declaration
typedef struct bcp BCP;

// Criação e destruição

/*
Função: 
    bcp_create
Descrição: 
    Cria e inicializa uma nova estrutura BCP (Bloco de Controle do Processo)
Entrada: 
    Process* process: ponteiro para o processo associado
Saída: 
    BCP*: ponteiro para a estrutura BCP criada ou NULL em caso de falha
*/
BCP *bcp_create(Process *process);

/*
Função: 
    bcp_destroy
Descrição: 
    Libera a memória e destrói a estrutura BCP
Entrada: 
    BCP* bcp: ponteiro para a estrutura BCP a ser destruída
Saída: 
    void
*/
void bcp_destroy(BCP *bcp);

/*
Função: 
    bcp_lock
Descrição: 
    Bloqueia o mutex da estrutura BCP para acesso thread-safe
Entrada: 
    BCP* bcp: ponteiro para a estrutura BCP
Saída: 
    void
*/
void bcp_lock(BCP *bcp);

/*
Função: 
    bcp_unlock
Descrição: 
    Desbloqueia o mutex da estrutura BCP
Entrada: 
    BCP* bcp: ponteiro para a estrutura BCP
Saída: 
    void
*/
void bcp_unlock(BCP *bcp);

/*
Função: 
    bcp_print
Descrição: 
    Imprime informações do processo e tempo de CPU da estrutura BCP
Entrada: 
    BCP* bcp: ponteiro para a estrutura BCP
Saída: 
    void
*/
void bcp_print(BCP *bcp);

// Funções de incremento

/*
Função: 
    bcp_increment_vruntime
Descrição: 
    Incrementa o tempo virtual (vruntime) do processo
Entrada: 
    BCP* bcp: ponteiro para a estrutura BCP
    double time: tempo a ser incrementado
Saída: 
    void
*/
void bcp_increment_vruntime(BCP *bcp, double time);

/*
Função: 
    bcp_increment_cpu_time
Descrição: 
    Incrementa o tempo total de CPU utilizado pelo processo
Entrada: 
    BCP* bcp: ponteiro para a estrutura BCP
    double time: tempo a ser incrementado
Saída: 
    void
*/
void bcp_increment_cpu_time(BCP *bcp, double time);

// Funções de setagem

/*
Função: 
    bcp_set_process_state
Descrição: 
    Define o estado do processo associado ao BCP
Entrada: 
    BCP* bcp: ponteiro para a estrutura BCP
    ProcessState state: novo estado do processo
Saída: 
    void
*/
void bcp_set_process_state(BCP* bcp, ProcessState state);

// Funções get

/*
Função: 
    bcp_get_process
Descrição: 
    Obtém o ponteiro para o processo associado ao BCP
Entrada: 
    BCP* bcp: ponteiro para a estrutura BCP
Saída: 
    Process*: ponteiro para o processo associado
*/
Process *bcp_get_process(BCP *bcp);

/*
Função: 
    bcp_get_process_state
Descrição: 
    Obtém o estado atual do processo associado ao BCP
Entrada: 
    BCP* bcp: ponteiro para a estrutura BCP
Saída: 
    ProcessState: estado atual do processo
*/
ProcessState bcp_get_process_state(BCP* bcp);

/*
Função: 
    bcp_get_cpu_time
Descrição: 
    Obtém o tempo total de CPU utilizado pelo processo
Entrada: 
    BCP* bcp: ponteiro para a estrutura BCP
Saída: 
    double: tempo total de CPU ou -1 se bcp for NULL
*/
double bcp_get_cpu_time(BCP *bcp);

/*
Função: 
    bcp_get_vruntime
Descrição: 
    Obtém o tempo virtual (vruntime) do processo
Entrada: 
    BCP* bcp: ponteiro para a estrutura BCP
Saída: 
    double: tempo virtual ou -1 se bcp for NULL
*/
double bcp_get_vruntime(BCP *bcp);
