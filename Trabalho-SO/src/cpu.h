/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#pragma once

#include "process.h"
#include "tcb.h"
#include "bcp.h"
#include "kernel.h"

typedef struct cpu CPU;

/*
Função: 
    create_cpu
Descrição: 
    Cria e inicializa uma nova estrutura CPU
Entrada: 
    TCB* tcb: ponteiro para o TCB (Thread Control Block) inicial
Saída: 
    CPU*: ponteiro para a estrutura CPU criada ou NULL em caso de falha
*/
CPU *create_cpu(TCB *tcb);

/*
Função: 
    cpu_destroy
Descrição: 
    Destroi a estrutura CPU e libera todos os recursos associados
Entrada: 
    CPU* cpu: ponteiro para a estrutura CPU a ser destruída
Saída: 
    void
*/
void cpu_destroy(CPU *cpu);

/*
Função: 
    cpu_set_running_process
Descrição: 
    Define o processo que a CPU está executando
Entrada: 
    CPU* cpu: ponteiro para a estrutura CPU
    TCB* tcb: ponteiro para o TCB do processo a ser executado
Saída: 
    void
*/
void cpu_set_running_process(CPU *cpu, TCB *tcb);

/*
Função: 
    cpu_run
Descrição: 
    Executa o processo da CPU por um quantum de tempo especificado
Entrada: 
    CPU* cpu: ponteiro para a estrutura CPU
    int quantum: quantidade de tempo a ser executada
Saída: 
    void
*/
void cpu_run(CPU *cpu, int quantum);

/*
Função: 
    cpu_get_running_process
Descrição: 
    Retorna o processo que a CPU está executando atualmente
Entrada: 
    CPU* cpu: ponteiro para a estrutura CPU
Saída: 
    TCB*: ponteiro para o TCB do processo em execução ou NULL se não houver processo
*/
TCB *cpu_get_running_process(CPU *cpu);

/*
Função: 
    cpu_clear_running_process
Descrição: 
    Limpa o processo da CPU (quando o processo termina sua execução)
Entrada: 
    CPU* cpu: ponteiro para a estrutura CPU
Saída: 
    void
*/
void cpu_clear_running_process(CPU *cpu);