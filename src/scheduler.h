/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#pragma once
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

// Forward declarations
typedef struct scheduler Scheduler;
typedef struct bcp BCP;
typedef struct tcb TCB;

// Enum definido primeiro
typedef enum type_scheduler {
    FIRST_COME_FIRST_SERVED = 1,
    ROUND_ROBIN = 2,
    PRIORITY = 3,
    CFS = 4,
    INVALID = -1
} TypeScheduler;

// Funções

/*
Função: 
    create_scheduler
Descrição: 
    Cria e inicializa um escalonador com o tipo e quantum especificados
Entrada: 
    TypeScheduler type: tipo do escalonador (FCFS, RR, PRIORITY, CFS)
    int time_slice: quantum de tempo para escalonadores que utilizam time slice
Saída: 
    Scheduler*: ponteiro para o escalonador criado ou NULL em caso de falha
*/
Scheduler *create_scheduler(TypeScheduler type, int time_slice);

/*
Função: 
    scheduler_get_quantum
Descrição: 
    Obtém o quantum de tempo para uma thread específica
Entrada: 
    Scheduler* scheduler: ponteiro para o escalonador
    TCB* thread: ponteiro para a thread
Saída: 
    int: quantum de tempo em milissegundos ou -1 em caso de erro
*/
int scheduler_get_quantum(Scheduler *scheduler, TCB *thread);

/*
Função: 
    scheduler_get_type
Descrição: 
    Obtém o tipo do escalonador
Entrada: 
    Scheduler* scheduler: ponteiro para o escalonador
Saída: 
    TypeScheduler: tipo do escalonador
*/
TypeScheduler scheduler_get_type(Scheduler *scheduler);

/*
Função: 
    destroy_scheduler
Descrição: 
    Destroi o escalonador e libera a memória alocada
Entrada: 
    Scheduler* scheduler: ponteiro para o escalonador a ser destruído
Saída: 
    void
*/
void destroy_scheduler(Scheduler *scheduler);

/*
Função: 
    scheduler_back_to_ready
Descrição: 
    Recoloca uma thread na fila de prontos ou finaliza se necessário
Entrada: 
    Scheduler* scheduler: ponteiro para o escalonador
    void* data_structure: estrutura de dados da fila de prontos
    TCB* thread: ponteiro para a thread
    int id_cpu: identificador da CPU
Saída: 
    bool: true se a thread foi recolocada na fila, false se foi finalizada
*/
bool scheduler_back_to_ready(Scheduler *scheduler, void *data_structure, TCB *thread, int id_cpu);

/*
Função: 
    scheduler_change_context
Descrição: 
    Realiza a mudança de contexto selecionando a próxima thread a ser executada
Entrada: 
    Scheduler* scheduler: ponteiro para o escalonador
    void* data_structure: estrutura de dados da fila de prontos
    int id_cpu: identificador da CPU
    TCB* current_thread: ponteiro para a thread atual em execução
Saída: 
    TCB*: ponteiro para a próxima thread a ser executada ou NULL se não houver
*/
TCB *scheduler_change_context(Scheduler *scheduler, void *data_structure, int id_cpu, int PID);

/*
Função: 
    scheduler_add_to_ready
Descrição: 
    Adiciona um BCP (Bloco de Controle do Processo) à fila de prontos
Entrada: 
    Scheduler* scheduler: ponteiro para o escalonador
    void* data_structure: estrutura de dados da fila de prontos
    BCP* bcp: ponteiro para o BCP a ser adicionado
Saída: 
    void
*/
void scheduler_add_to_ready(Scheduler *scheduler, void *data_structure, BCP *bcp);

/*
Função: 
    scheduler_create_ready_set
Descrição: 
    Cria a estrutura de dados para a fila de prontos baseada no tipo de escalonador
Entrada: 
    Scheduler* scheduler: ponteiro para o escalonador
    int n_processes: número de processos (para dimensionamento inicial)
Saída: 
    void*: ponteiro para a estrutura de dados criada
*/
void *scheduler_create_ready_set(Scheduler *scheduler, int n_processes);

/*
Função: 
    scheduler_destroy_ready_struct
Descrição: 
    Destroi a estrutura de dados da fila de prontos e libera todos os recursos
Entrada: 
    Scheduler* scheduler: ponteiro para o escalonador
    void* data_structure: estrutura de dados da fila de prontos
Saída: 
    void
*/
void scheduler_destroy_ready_struct(Scheduler *scheduler, void *data_structure);
