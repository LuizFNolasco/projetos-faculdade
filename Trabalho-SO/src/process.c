/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#include "process.h"

struct process {
    int process_id;
    ProcessState state;
    int priority;
    double start_time;
    double duration;
    int n_threads;
};

int is_higher_priority_process(void *a, void *b) {
    if (!a || !b) return 0;
    Process *pa = a, *pb = b;
    if (pa->start_time != pb->start_time)
        return pa->start_time < pb->start_time;
    return pa->process_id < pb->process_id;
}

Process *create_process(int process_id, int thread_count, int priority, double start_time, double duration) {
    Process *process = (Process *)malloc(sizeof(Process));
    if (!process) {
        return NULL; // Handle memory allocation failure
    }
    
    process->process_id = process_id;
    process->n_threads = thread_count;
    process->priority = priority;
    process->state = PROCESS_NEW;
    process->duration = duration;
    process->start_time = start_time;

    return process;
}

ProcessState process_get_process_state(Process *process) {
    if (process) {
        return process->state;
    }
    return PROCESS_NEW; // Default state
}

bool process_has_finished(Process *process, int thread_id) {
    if (!process) return false;
    return process->n_threads == thread_id;
}

void process_set_process_state(Process *process, ProcessState state) {
    if (process) {
        process->state = state;
    }
}

int process_get_priority(Process *process) {
    if (process) {
        return process->priority;
    }
    return -1; // Default priority
}

double process_get_start_time(Process *process) {
    if (process) {
        return process->start_time;
    }
    return 0.0; // Default start time
}

void process_print(Process *process) {
    if (!process) {
        return; // Handle invalid input
    }
    printf("Process ID: %d\n", process->process_id);
    printf("Thread Count: %d\n", process->n_threads);
    printf("Priority: %d\n", process->priority);
    printf("State: %d\n", process->state);
    printf("Start Time: %.2f\n", process->start_time);
    printf("Duração: %.2f\n", process->duration);
}

void process_destroy(Process *process) {
    if (process) {
        free(process);
    }
    return;
}

int process_get_PID(Process *process){
    if(!process){
        return -1;
    }
    return process->process_id;
}

double process_get_duration(Process *process){
    return process ? process->duration : -1;
}

int process_get_n_threads(Process *process){
    if(!process){
        return 0;
    }
    return process->n_threads;
}