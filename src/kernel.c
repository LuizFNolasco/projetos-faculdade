/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#include "kernel.h"
#include "fifo.h"
#include "rbt.h"

struct kernel
{
    Heap *new_process;
    int n_processes;
    void *ready_process;
    double timer;
    Scheduler *scheduler;
    TCB **finished;
    int n_finished;
    int total_threads;
    int n_cpus;
    CPU **cpus;
    bool *cpus_blocked; // Array to track if CPUs are blocked
    int next_cpu_index; // Index of the next CPU to be used
    bool finish_simulation;

    pthread_mutex_t timer_mutex, cpu_available_mutex, set_finish, new_process_mutex, ready_mutex, finish_mutex;
    pthread_cond_t process_available; 
    long timer_ms;
};

Kernel *create_kernel(int n_processes, int n_cpus) { 
    Kernel *kernel = malloc(sizeof(Kernel));
    if (!kernel) return NULL;

    kernel->new_process = heap_create(n_processes, is_higher_priority_process);
    if (!kernel->new_process) {
        free(kernel);
        return NULL;
    }
    kernel->n_processes = n_processes;
    kernel->finished = NULL;

    // Criar cada CPU individualmente e checar falha
    kernel->cpus = malloc(n_cpus * sizeof(CPU *));
    if (!kernel->cpus) {
        heap_destroy(kernel->new_process);
        free(kernel);
        return NULL;
    }
    for (int i = 0; i < n_cpus; i++) {
        kernel->cpus[i] = create_cpu(NULL);
        if (!kernel->cpus[i]) {
            // Destruir CPUs já criadas
            for (int j = 0; j < i; j++) cpu_destroy(kernel->cpus[j]);
            free(kernel->cpus);
            heap_destroy(kernel->new_process);
            free(kernel);
            return NULL;
        }
    }

    kernel->cpus_blocked = calloc(n_cpus, sizeof(bool));
    if (!kernel->cpus_blocked) {
        for (int i = 0; i < n_cpus; i++) cpu_destroy(kernel->cpus[i]);
        free(kernel->cpus);
        heap_destroy(kernel->new_process);
        free(kernel);
        return NULL;
    }

    kernel->n_cpus = n_cpus;
    kernel->finish_simulation = false;
    kernel->next_cpu_index = 0;
    kernel->timer_ms = 0;
    kernel->n_finished = 0;

    // Inicialização de mutexes e condição
    pthread_mutex_init(&kernel->timer_mutex, NULL);
    pthread_mutex_init(&kernel->cpu_available_mutex, NULL);
    pthread_mutex_init(&kernel->set_finish, NULL);
    pthread_mutex_init(&kernel->new_process_mutex, NULL);
    pthread_mutex_init(&kernel->ready_mutex, NULL);
    pthread_mutex_init(&kernel->finish_mutex, NULL);
    pthread_cond_init(&kernel->process_available, NULL);

    return kernel;
}

void kernel_set_total_threads(Kernel *kernel, int total_threads) {
    if (!kernel) {
        return; // Handle invalid input
    }
    kernel->total_threads = total_threads;
}

void kernel_set_type_scheduler(Kernel *kernel, TypeScheduler type, int quantum) {
    if (!kernel) {
        return; // Handle invalid input
    }
    kernel->scheduler = create_scheduler(type, quantum);
    pthread_mutex_lock(&kernel->ready_mutex);
    kernel->ready_process = scheduler_create_ready_set(kernel->scheduler, kernel->n_processes);
    if(type == CFS)
        kernel->finished = calloc(kernel->total_threads, sizeof(TCB *));
    pthread_mutex_unlock(&kernel->ready_mutex);
}

Heap *kernel_get_new_process(Kernel *kernel) {
    if (!kernel) {
        return NULL; // Handle invalid input
    }
    return kernel->new_process;
}

void kernel_print_new_processes(Kernel *kernel) {
    if (!kernel || !kernel->new_process) {
        return; // Handle invalid input
    }
    printf("\nNew processes:\n");
    void *process;
    while ((process = heap_extract_min(kernel->new_process)) != NULL) {
        Process *process_p = (Process *)process;
        process_print(process_p);
        process_destroy(process_p); // Free the process after printing
    }
}

Scheduler *kernel_get_scheduler(Kernel *kernel) {
    if (!kernel) {
        return NULL; // Handle invalid input
    }
    return kernel->scheduler;
}

static void* time_counter(void *arg) {
    Kernel *kernel = (Kernel *)arg;
    if (!kernel) {
        return NULL; // Handle invalid input
    }

    struct timespec inicio, agora;

    clock_gettime(CLOCK_MONOTONIC_RAW, &inicio);
    bool stop = false;

    while (!stop) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &agora);
        long ms = (agora.tv_sec - inicio.tv_sec) * 1000 +
                  (agora.tv_nsec - inicio.tv_nsec) / 1000000;

        // Atualiza variável compartilhada com mutex
        pthread_mutex_lock(&kernel->timer_mutex);
        kernel->timer_ms = ms;
        pthread_mutex_unlock(&kernel->timer_mutex);

        pthread_mutex_lock(&kernel->set_finish);
        stop = kernel->finish_simulation;
        pthread_mutex_unlock(&kernel->set_finish);

        usleep(10000); // atualiza a cada 10 ms
    }
    return NULL;
}

void kernel_add_process_to_ready(Kernel *kernel, Process *process) {
    if (!kernel || !process) {
        return; // Handle invalid input
    }
    
    pthread_mutex_lock(&kernel->ready_mutex);
    if (kernel->ready_process) {
        BCP *bcp = bcp_create(process);
        scheduler_add_to_ready(kernel->scheduler, kernel->ready_process, bcp);
    }
    pthread_mutex_unlock(&kernel->ready_mutex);
    pthread_cond_signal(&kernel->process_available);
}

static void* loader_new_process(void *arg) {
    Kernel *kernel = (Kernel *)arg;
    if (!kernel) return NULL;

    while (1) {
        pthread_mutex_lock(&kernel->set_finish);
        bool finish = kernel->finish_simulation;
        pthread_mutex_unlock(&kernel->set_finish);
        if (finish) break;

        pthread_mutex_lock(&kernel->new_process_mutex);
        pthread_mutex_lock(&kernel->timer_mutex);

        if (!heap_is_empty(kernel->new_process)) {
            Process *process = heap_peek(kernel->new_process);
            if (kernel->timer_ms >= process_get_start_time(process)) {
                process = heap_extract_min(kernel->new_process);
                pthread_mutex_unlock(&kernel->timer_mutex);
                pthread_mutex_unlock(&kernel->new_process_mutex);

                kernel_add_process_to_ready(kernel, process);
                usleep(10000);
                continue;
            }
        }

        pthread_mutex_unlock(&kernel->timer_mutex);
        pthread_mutex_unlock(&kernel->new_process_mutex);
        usleep(10000);
    }
    return NULL;
}

// Thread de trabalho da CPU
static void* cpu_worker(void *arg) {
    struct {
        Kernel *kernel;
        int cpu_idx;
    } *data = arg;
    Kernel *kernel = data->kernel;
    pthread_mutex_lock(&kernel->cpu_available_mutex);
    int cpu_idx = data->cpu_idx;
    CPU *cpu = kernel->cpus[cpu_idx];
    pthread_mutex_unlock(&kernel->cpu_available_mutex);
    int PID = -1;
    free(data);

    while (1) {
        // Verifica se a simulação terminou
        pthread_mutex_lock(&kernel->set_finish);
        bool finish = kernel->finish_simulation;
        pthread_mutex_unlock(&kernel->set_finish);
        if (finish) break;

        // Bloqueia a CPU
        pthread_mutex_lock(&kernel->cpu_available_mutex);
        if (kernel->cpus_blocked[cpu_idx]) {
            pthread_mutex_unlock(&kernel->cpu_available_mutex);
            usleep(10000);
            continue;
        }
        kernel->cpus_blocked[cpu_idx] = true;
        pthread_mutex_unlock(&kernel->cpu_available_mutex);
        // Change context para obter próxima thread
        int id_cpu = kernel->n_cpus > 1 ? cpu_idx : -1;
        pthread_mutex_lock(&kernel->ready_mutex);
        TCB *next_thread = scheduler_change_context(kernel->scheduler, kernel->ready_process, id_cpu, PID);
        pthread_mutex_unlock(&kernel->ready_mutex);
        
        // Executa a thread se existir
        if (next_thread) {
            cpu_set_running_process(cpu, next_thread);
            cpu_run(cpu, scheduler_get_quantum(kernel->scheduler, next_thread));

            if(tcb_is_terminated(next_thread) && scheduler_get_type(kernel->scheduler) == CFS) {
                pthread_mutex_lock(&kernel->finish_mutex);
                kernel->finished[kernel->n_finished++] = next_thread;
                pthread_mutex_unlock(&kernel->finish_mutex);
            }
            
            PID = process_get_PID(bcp_get_process(tcb_get_bcp(next_thread)));
            // Recoloca thread na fila ou finaliza
            pthread_mutex_lock(&kernel->ready_mutex);
            scheduler_back_to_ready(kernel->scheduler, kernel->ready_process, next_thread, id_cpu);
            pthread_mutex_unlock(&kernel->ready_mutex);
        }

        // Limpa a CPU
        cpu_clear_running_process(cpu);

        // Libera a CPU
        pthread_mutex_lock(&kernel->cpu_available_mutex);
        kernel->cpus_blocked[cpu_idx] = false;
        pthread_mutex_unlock(&kernel->cpu_available_mutex);

        usleep(100000);
    }
    return NULL;
}

static void *kernel_dispatch_processes(void *arg) {
    Kernel *kernel = (Kernel *)arg;
    pthread_t th[kernel->n_cpus];

    for (int i = 0; i < kernel->n_cpus; i++) {
        struct { Kernel *kernel; int cpu_idx; } *data = malloc(sizeof(*data));
        data->kernel = kernel;
        data->cpu_idx = i;

        pthread_create(&th[i], NULL, cpu_worker, data);
    }

    for (int i = 0; i < kernel->n_cpus; i++) {
        pthread_join(th[i], NULL);
    }
    return NULL;
}

static void* kernel_monitor(void *arg) {
    Kernel *kernel = (Kernel *)arg;
    
    while (true) {
        pthread_mutex_lock(&kernel->new_process_mutex);
        bool new_empty = heap_is_empty(kernel->new_process);
        pthread_mutex_unlock(&kernel->new_process_mutex);
        
        pthread_mutex_lock(&kernel->ready_mutex);
        TypeScheduler type = scheduler_get_type(kernel->scheduler);
        bool ready_empty = false;
        
        if (type == FIRST_COME_FIRST_SERVED || type == ROUND_ROBIN) {
            ready_empty = fifo_is_empty((FIFOQueue *)kernel->ready_process);
        } else if (type == PRIORITY) {
            ready_empty = heap_is_empty((Heap *)kernel->ready_process);
        } else if (type == CFS){
            ready_empty = (CFS_min((TRBTree *)kernel->ready_process) == NULL);
        }       
        pthread_mutex_unlock(&kernel->ready_mutex);

        bool cpus_busy = false;
        pthread_mutex_lock(&kernel->cpu_available_mutex);
        for (int i = 0; i < kernel->n_cpus; i++)
            if (kernel->cpus_blocked[i]) {
                cpus_busy = true;
                break;
            }
        pthread_mutex_unlock(&kernel->cpu_available_mutex);

        if (new_empty && ready_empty && !cpus_busy) {
            pthread_mutex_lock(&kernel->set_finish);
            kernel->finish_simulation = true;
            pthread_mutex_unlock(&kernel->set_finish);
            break;
        }
        
        usleep(100000); // Verificar a cada 100ms
    }
    printf("Escalonador terminou execução de todos processos\n");
    return NULL;
}

void kernel_simulate_cpus(Kernel *kernel) {
    if (!kernel) return;

    pthread_t timer_thread, loader_thread, monitor_thread, dispatcher_thread;

    // Cria thread do timer
    pthread_create(&timer_thread, NULL, time_counter, kernel);

    // Cria thread que carrega novos processos para ready
    pthread_create(&loader_thread, NULL, loader_new_process, kernel);

    // Cria thread que monitora o término da simulação
    pthread_create(&monitor_thread, NULL, kernel_monitor, kernel);

    // Cria threads das CPUs usando kernel_dispatch_processes
    pthread_create(&dispatcher_thread, NULL, kernel_dispatch_processes, kernel);

    // Aguarda threads do timer, loader e monitor
    pthread_join(timer_thread, NULL);
    pthread_join(loader_thread, NULL);
    pthread_join(monitor_thread, NULL);
    pthread_join(dispatcher_thread, NULL);
}
    
void destroy_kernel(Kernel *kernel){
    if (kernel) {
        if (kernel->new_process) {
            void *process;
            while ((process = heap_extract_min(kernel->new_process)) != NULL) {
                Process *process_p = (Process *)process;
                process_destroy(process_p); // Free the process
            }
            heap_destroy(kernel->new_process);
        }
        if (kernel->cpus) {
            for (int i = 0; i < kernel->n_cpus; i++) {
                if (kernel->cpus[i]) {
                    cpu_destroy(kernel->cpus[i]);
                }
            }
            free(kernel->cpus);
            free(kernel->cpus_blocked);
        }
        scheduler_destroy_ready_struct(kernel->scheduler, kernel->ready_process);
        destroy_scheduler(kernel->scheduler);

        for (int i = 0; i < kernel->n_finished; i++)
        {
            if(!kernel->finished[i]) continue;
            int n_threads = process_get_n_threads(bcp_get_process(tcb_get_bcp(kernel->finished[i])));
            for (int j = i+1; j < n_threads; j++)
                if(tcb_get_bcp(kernel->finished[i]) == tcb_get_bcp(kernel->finished[j])){
                    tcb_destroy(kernel->finished[j]);
                    kernel->finished[j] = NULL;
                }
            process_destroy(bcp_get_process(tcb_get_bcp(kernel->finished[i])));
            bcp_destroy(tcb_get_bcp(kernel->finished[i]));
            tcb_destroy(kernel->finished[i]);
            kernel->finished[i] = NULL;
        };
        if(kernel->finished)
            free(kernel->finished);

        pthread_mutex_destroy(&kernel->cpu_available_mutex);
        pthread_mutex_destroy(&kernel->new_process_mutex);
        pthread_mutex_destroy(&kernel->ready_mutex);
        pthread_mutex_destroy(&kernel->set_finish);
        pthread_mutex_destroy(&kernel->timer_mutex);
        pthread_mutex_destroy(&kernel->finish_mutex);
        pthread_cond_destroy(&kernel->process_available);
        free(kernel);
    }
}

void kernel_add_new_process(Kernel *kernel, Process *process){
    if (kernel && process) {
        pthread_mutex_lock(&kernel->new_process_mutex);
        if (kernel->new_process)
            heap_insert(kernel->new_process, process);
        pthread_mutex_unlock(&kernel->new_process_mutex);
    }
}

TypeScheduler kernel_scheduler_get_type(Kernel *kernel){
    if(!kernel){
        return -1;
    }
    return scheduler_get_type(kernel->scheduler);
}