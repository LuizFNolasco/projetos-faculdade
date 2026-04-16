/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#include "cpu.h"
#include <stdlib.h>
#include <pthread.h>

struct cpu {
    TCB *tcb;
    pthread_mutex_t tcb_mutex;  // Protege o acesso ao tcb
};

// Cria uma CPU
CPU *create_cpu(TCB *tcb) {
    CPU *cpu = malloc(sizeof(CPU));
    if (!cpu) return NULL;

    cpu->tcb = tcb;
    pthread_mutex_init(&cpu->tcb_mutex, NULL);
    return cpu;
}

// Destroi uma CPU
void cpu_destroy(CPU *cpu) {
    if (cpu) {
        pthread_mutex_destroy(&cpu->tcb_mutex);
        if (cpu->tcb)
        {
            if(tcb_get_bcp(cpu->tcb)) process_destroy(bcp_get_process(tcb_get_bcp(cpu->tcb)));
            if(tcb_get_bcp(cpu->tcb)) bcp_destroy(tcb_get_bcp(cpu->tcb));
            tcb_destroy(cpu->tcb);
        }
        
        free(cpu);
    }
}

// Executa o processo da CPU por um quantum
void cpu_run(CPU *cpu, int quantum) {
    if (cpu) {
        pthread_mutex_lock(&cpu->tcb_mutex);
        TCB *tcb_local = cpu->tcb;
        pthread_mutex_unlock(&cpu->tcb_mutex);

        if (tcb_local) {
            tcb_working_thread(tcb_local, quantum);
        }
    }
}

// Define o processo que a CPU está executando
void cpu_set_running_process(CPU *cpu, TCB *tcb){
    if (cpu) {
        pthread_mutex_lock(&cpu->tcb_mutex);
        cpu->tcb = tcb;
        pthread_mutex_unlock(&cpu->tcb_mutex);
    }
}

// Retorna o processo que a CPU está executando
TCB *cpu_get_running_process(CPU *cpu) {
    if (cpu) {
        pthread_mutex_lock(&cpu->tcb_mutex);
        TCB *tcb_local = cpu->tcb;
        pthread_mutex_unlock(&cpu->tcb_mutex);
        return tcb_local;
    }
    return NULL;
}

// Limpa o processo da CPU (quando termina)
void cpu_clear_running_process(CPU *cpu) {
    if (cpu) {
        pthread_mutex_lock(&cpu->tcb_mutex);
        cpu->tcb = NULL;
        pthread_mutex_unlock(&cpu->tcb_mutex);
    }
}
