/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
typedef struct kernel Kernel;
typedef enum type_scheduler TypeScheduler;
typedef struct process Process;

/*
Função: 
    read_processes
Descrição: 
    Lê os dados dos processos de um arquivo e cria uma estrutura Kernel com as informações
Entrada: 
    FILE* file: ponteiro para o arquivo de entrada
    int quantum: quantum de tempo para escalonamento
    int n_cpus: número de CPUs disponíveis
Saída: 
    Kernel*: ponteiro para a estrutura Kernel criada ou NULL em caso de erro
*/
Kernel *read_processes(FILE *file, int quantum, int n_cpus);

/*
Função: 
    read_processes
Descrição: 
    Lê os dados dos processos de um arquivo e cria uma estrutura Kernel com as informações
Entrada: 
    FILE* file: ponteiro para o arquivo de entrada
    int quantum: quantum de tempo para escalonamento
    int n_cpus: número de CPUs disponíveis
Saída: 
    Kernel*: ponteiro para a estrutura Kernel criada ou NULL em caso de erro
*/
void write_process(FILE *file, Process *process, TypeScheduler type_scheduler, int quantum, int id_cpu);
