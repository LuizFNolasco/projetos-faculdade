/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#pragma once

#define _GNU_SOURCE
#include "process.h"
#include "tcb.h"
#include "bcp.h"
#include "scheduler.h"
#include "heap.h"
#include "cpu.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

typedef struct kernel Kernel;
typedef struct scheduler Scheduler;

/*
Função: 
    create_kernel
Descrição: 
    Cria e inicializa uma estrutura Kernel para gerenciamento do sistema
Entrada: 
    int n_processes: número máximo de processos
    int n_cpus: número de CPUs disponíveis
Saída: 
    Kernel*: ponteiro para a estrutura Kernel criada ou NULL em caso de falha
*/
Kernel *create_kernel(int n_processes, int n_cpus);

/*
Função: 
    kernel_set_type_scheduler
Descrição: 
    Configura o tipo de escalonador a ser utilizado pelo kernel
Entrada: 
    Kernel* kernel: ponteiro para a estrutura Kernel
    TypeScheduler type: tipo do escalonador (FCFS, RR, PRIORITY, CFS)
    int quantum: quantum de tempo para escalonadores que utilizam quantum
Saída: 
    void
*/
void kernel_set_type_scheduler(Kernel *kernel, TypeScheduler type, int quantum);

/*
Função: 
    kernel_print_new_processes
Descrição: 
    Imprime todos os processos na fila de novos processos do kernel
Entrada: 
    Kernel* kernel: ponteiro para a estrutura Kernel
Saída: 
    void
*/
void kernel_print_new_processes(Kernel *kernel);

/*
Função: 
    kernel_get_new_process
Descrição: 
    Obtém a fila de novos processos do kernel
Entrada: 
    Kernel* kernel: ponteiro para a estrutura Kernel
Saída: 
    Heap*: ponteiro para a fila de novos processos
*/
Heap *kernel_get_new_process(Kernel *kernel);

/*
Função: 
    kernel_get_scheduler
Descrição: 
    Obtém o escalonador configurado no kernel
Entrada: 
    Kernel* kernel: ponteiro para a estrutura Kernel
Saída: 
    Scheduler*: ponteiro para o escalonador
*/
Scheduler *kernel_get_scheduler(Kernel *kernel);

/*
Função: 
    destroy_kernel
Descrição: 
    Destroi a estrutura Kernel e libera todos os recursos associados
Entrada: 
    Kernel* kernel: ponteiro para a estrutura Kernel a ser destruída
Saída: 
    void
*/
void destroy_kernel(Kernel *kernel);

/*
Função: 
    kernel_set_total_threads
Descrição: 
    Define o número total de threads no sistema
Entrada: 
    Kernel* kernel: ponteiro para a estrutura Kernel
    int total_threads: número total de threads
Saída: 
    void
*/
void kernel_set_total_threads(Kernel *kernel, int total_threads);

/*
Função: 
    kernel_add_new_process
Descrição: 
    Adiciona um novo processo à fila de novos processos do kernel
Entrada: 
    Kernel* kernel: ponteiro para a estrutura Kernel
    Process* process: ponteiro para o processo a ser adicionado
Saída: 
    void
*/
void kernel_add_new_process(Kernel *kernel, Process *process);

/*
Função: 
    kernel_scheduler_get_type
Descrição: 
    Obtém o tipo de escalonador configurado no kernel
Entrada: 
    Kernel* kernel: ponteiro para a estrutura Kernel
Saída: 
    TypeScheduler: tipo do escalonador
*/
TypeScheduler kernel_scheduler_get_type(Kernel *kernel);

/*
Função: 
    kernel_simulate_cpus
Descrição: 
    Inicia a simulação de execução das CPUs do kernel
Entrada: 
    Kernel* kernel: ponteiro para a estrutura Kernel
Saída: 
    void
*/
void kernel_simulate_cpus(Kernel *kernel);


