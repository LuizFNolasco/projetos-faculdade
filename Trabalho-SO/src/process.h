/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#pragma once
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

enum process_state {
    PROCESS_NEW = 'n',
    PROCESS_READY = 'r',
    PROCESS_RUNNING = 'R',
    PROCESS_FINISHED = 'f',
};

typedef struct process Process;
typedef enum process_state ProcessState;

/*
Função: 
    is_higher_priority_process
Descrição: 
    Função de comparação para determinar se um processo tem prioridade maior que outro
Entrada: 
    void* a: ponteiro para o primeiro processo
    void* b: ponteiro para o segundo processo
Saída: 
    int: 1 se 'a' tem prioridade maior que 'b', 0 caso contrário
*/
int is_higher_priority_process(void *a, void *b);

/*
Função: 
    create_process
Descrição: 
    Cria e inicializa uma nova estrutura Process
Entrada: 
    int process_id: identificador do processo
    int thread_count: número de threads do processo
    int priority: prioridade do processo
    double start_time: tempo de início do processo
    double duration: duração do processo
Saída: 
    Process*: ponteiro para o processo criado ou NULL em caso de falha
*/
Process *create_process(int process_id, int thread_count, int priority, double start_time, double duration);

/*
Função: 
    process_set_process_state
Descrição: 
    Define o estado atual do processo
Entrada: 
    Process* process: ponteiro para o processo
    ProcessState state: novo estado do processo
Saída: 
    void
*/
void process_set_process_state(Process *process, ProcessState state);

/*
Função: 
    process_get_process_state
Descrição: 
    Obtém o estado atual do processo
Entrada: 
    Process* process: ponteiro para o processo
Saída: 
    ProcessState: estado atual do processo
*/
ProcessState process_get_process_state(Process *process);

/*
Função: 
    process_get_priority
Descrição: 
    Obtém a prioridade do processo
Entrada: 
    Process* process: ponteiro para o processo
Saída: 
    int: prioridade do processo ou -1 se processo for NULL
*/
int process_get_priority(Process *process);

/*
Função: 
    process_has_finished
Descrição: 
    Verifica se o processo terminou com base no ID da thread
Entrada: 
    Process* process: ponteiro para o processo
    int thread_id: identificador da thread
Saída: 
    bool: true se o processo terminou, false caso contrário
*/
bool process_has_finished(Process *process, int thread_id);

/*
Função: 
    process_get_start_time
Descrição: 
    Obtém o tempo de início do processo
Entrada: 
    Process* process: ponteiro para o processo
Saída: 
    double: tempo de início do processo ou 0.0 se processo for NULL
*/
double process_get_start_time(Process *process);

/*
Função: 
    process_print
Descrição: 
    Imprime as informações do processo
Entrada: 
    Process* process: ponteiro para o processo
Saída: 
    void
*/
void process_print(Process *process);

/*
Função: 
    process_destroy
Descrição: 
    Destroi a estrutura Process e libera a memória
Entrada: 
    Process* process: ponteiro para o processo a ser destruído
Saída: 
    void
*/
void process_destroy(Process *process);

/*
Função: 
    process_get_PID
Descrição: 
    Obtém o ID do processo
Entrada: 
    Process* process: ponteiro para o processo
Saída: 
    int: ID do processo ou -1 se processo for NULL
*/
int process_get_PID(Process *process);

/*
Função: 
    process_get_n_threads
Descrição: 
    Obtém o número de threads do processo
Entrada: 
    Process* process: ponteiro para o processo
Saída: 
    int: número de threads do processo ou 0 se processo for NULL
*/
int process_get_n_threads(Process *process);

/*
Função: 
    process_get_duration
Descrição: 
    Obtém a duração do processo
Entrada: 
    Process* process: ponteiro para o processo
Saída: 
    double: duração do processo ou -1 se processo for NULL
*/
double process_get_duration(Process *process);