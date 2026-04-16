/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#include "io.h"
#include "scheduler.h"
#include "process.h"
#include "tcb.h"
#include "kernel.h"

Kernel *read_processes(FILE *file, int quantum, int n_cpus) {
    int n_processes;
    TypeScheduler type;
    if (fscanf(file, "%d%*c", &n_processes) != 1) {
        return NULL; // Error reading number of processes
    }
    // Read the scheduling type da ultima linha do arquivo
    int type_int, total_threads = 0;
    Kernel *kernel = NULL;
    kernel = create_kernel(n_processes, n_cpus);
    if (!kernel) {
        return NULL; // Memory allocation error
    }

    int duration, priority, n_threads, arrival_time;
    for (int i = 0; i < n_processes; i++)
    {
        if (fscanf(file, "%d%*c", &duration) != 1) {
            return NULL; // Error reading duration
        }
        if (fscanf(file, "%d%*c", &priority) != 1) {
            return NULL; // Error reading priority
        }
        if (fscanf(file, "%d%*c", &n_threads) != 1) {
            return NULL; // Error reading number of threads
        }
        if (fscanf(file, "%d%*c", &arrival_time) != 1) {
            return NULL; // Error reading arrival time
        }

        Process *process = create_process(i+1, n_threads, priority, arrival_time, duration);
        if (!process) {
            return NULL; // Memory allocation error
        }
        kernel_add_new_process(kernel, process);
        total_threads += n_threads;
    }
    
    if (fscanf(file, "%d%*c", &type_int) != 1) {
        return NULL; // Error reading scheduling type
    }
    
    switch (type_int) {
        case 1:
            type = FIRST_COME_FIRST_SERVED;
            break;
        case 2:
            type = ROUND_ROBIN;
            break;
        case 3:
            type = PRIORITY;
            break;
        case 4:
            type = CFS;
            break;
        default:
            return NULL;
    }
    kernel_set_total_threads(kernel, total_threads);
    kernel_set_type_scheduler(kernel, type, quantum);
    return kernel;
}

void write_process(FILE *file, Process *process,TypeScheduler type_scheduler, int quantum, int id_cpu){
    if(process_get_process_state(process) == PROCESS_RUNNING){
        switch (type_scheduler){
            case FIRST_COME_FIRST_SERVED:
                fprintf(file, "[FCFS] Executando processo PID %d ", process_get_PID(process));
                break;
            case ROUND_ROBIN:
                fprintf(file, "[RR] Executando processo PID %d com quantum %dms ", process_get_PID(process), quantum);
                break;
            case PRIORITY:
                fprintf(file, "[PRIORITY] Executando processo PID %d com prioridade %d ", process_get_PID(process), process_get_priority(process));
                break;
            case CFS:
                fprintf(file, "[CFS] Executando processo PID %d ", process_get_PID(process));
                break;
            default:
                break;
        }
        if(id_cpu != -1)
            fprintf(file, "// processador %d\n", id_cpu);
        else
            fprintf(file, "\n");
    }else if(process_get_process_state(process) == PROCESS_FINISHED){
        switch(type_scheduler){
            case FIRST_COME_FIRST_SERVED:
                fprintf(file, "[FCFS] Processo PID %d finalizado\n", process_get_PID(process));
                break;
            case ROUND_ROBIN:
                fprintf(file, "[RR] Processo PID %d finalizado\n", process_get_PID(process));
                break;
            case PRIORITY:
                fprintf(file, "[PRIORITY] Processo PID %d finalizado\n", process_get_PID(process));
                break;
            case CFS:
                fprintf(file, "[CFS] Processo PID %d finalizado\n", process_get_PID(process));
                break;
            default:
                break;
        }
    }
    
}