/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#include "scheduler.h"
#include "io.h"
#include "tcb.h"
#include "fifo.h"
#include "heap.h"
#include "bcp.h"
#include "rbt.h"

struct scheduler
{
    TypeScheduler type;
    int quantum;
};

static int compare_cfs_tcb(const void *a, const void *b) {
    if (a == NULL || b == NULL) return -1;
    Process *pa = bcp_get_process(tcb_get_bcp((TCB *)a));
    Process *pb = bcp_get_process(tcb_get_bcp((TCB *)b));

    long va = bcp_get_vruntime(tcb_get_bcp((TCB *)a));
    long vb = bcp_get_vruntime(tcb_get_bcp((TCB *)b));

    if (va < vb) return -1;
    if (va > vb) return 1;

    int start_diff = process_get_start_time(pa) - process_get_start_time(pb);
    if (start_diff < 0) return -1;
    if (start_diff > 0) return 1;

    int pid_diff = process_get_PID(pa) - process_get_PID(pb);
    if (pid_diff < 0) return -1;
    if (pid_diff > 0) return 1;

    // último desempate: endereço do TCB (garante ordem total e nunca retorna 0)
    return (a < b ? -1 : 1);
}

int is_higher_priority_tcb(void *a, void *b) {
    if (a == NULL || b == NULL) return 0; // Evita nulls
    
    BCP *bcp_a = tcb_get_bcp((TCB *)a);
    BCP *bcp_b = tcb_get_bcp((TCB *)b);
    Process *pa = bcp_get_process(bcp_a);
    Process *pb = bcp_get_process(bcp_b);

    int pa_prio = process_get_priority(pa);
    int pb_prio = process_get_priority(pb);

    // 1️⃣ Menor número de prioridade vem antes
    if (pa_prio != pb_prio) {
        return pa_prio - pb_prio;  
    }

    // 2️⃣ Se prioridade igual → quem começou antes (menor start_time)
    double time_diff = process_get_start_time(pa) - process_get_start_time(pb);
    if (time_diff != 0) {
        return (time_diff < 0) ? -1 : 1;
    }

    // 3️⃣ Se ainda empatar → menor ID vem antes
    return tcb_get_ID((TCB *)a) - tcb_get_ID((TCB *)b);
}

TCB *cfs_scheduler(TRBTree *tree) {
    if (!tree) return NULL;
    TCB *next = CFS_min(tree);
    return next;
}


TCB *round_robin_scheduler(FIFOQueue *threads_list, int last_PID) {
    if (!threads_list || fifo_is_empty(threads_list)) return NULL;

    TCB *next_thread = fifo_pop(threads_list);
    int sz = fifo_size(threads_list), count = 0;
    if (last_PID != -1) {
        while (next_thread && process_get_PID(bcp_get_process(tcb_get_bcp(next_thread))) == last_PID) {
            fifo_push(threads_list, next_thread); // Recoloca no final
            next_thread = fifo_pop(threads_list);
            count++;
            if (count >= sz) break; // Evita loop infinito
        } 
    }
    return next_thread;
}

TCB *priority_scheduler(Heap *thread_heap) {
    if (!thread_heap || heap_is_empty(thread_heap))
        return NULL; // Handle invalid input
    return heap_extract_min(thread_heap); // No current process, return the highest priority process
}

TCB *first_come_first_served_scheduler(FIFOQueue *fifo_queue){
    if (!fifo_queue || fifo_is_empty(fifo_queue)) return NULL;
    return fifo_pop(fifo_queue);  // pega a próxima da fila
}

Scheduler *create_scheduler(TypeScheduler type, int quantum){
    Scheduler *scheduler = (Scheduler *)malloc(sizeof(Scheduler));
    if (!scheduler) {
        return NULL; // Handle memory allocation failure
    }
    
    scheduler->type = type;
    scheduler->quantum = quantum;
    return scheduler;
}

TypeScheduler scheduler_get_type(Scheduler *scheduler) {
    if (!scheduler) {
        return INVALID;
    }
    return scheduler->type;
}

int scheduler_get_quantum(Scheduler *scheduler, TCB *thread) {
    if (!scheduler) {
        return -1; // Handle invalid input
    }
    if(scheduler->type == FIRST_COME_FIRST_SERVED) return tcb_get_total_time(thread);
    return scheduler->quantum;
}

void scheduler_destroy_ready_struct(Scheduler *scheduler, void *data_structure) {
    if (!scheduler || !data_structure) return;

    switch (scheduler->type) {
        case FIRST_COME_FIRST_SERVED:{
            FIFOQueue *fifo = (FIFOQueue *)data_structure;
            while (!fifo_is_empty(fifo)) {
                TCB *tcb = fifo_pop(fifo);
                if (!tcb) continue;
                BCP *bcp = tcb_get_bcp(tcb);
                if (bcp) process_destroy(bcp_get_process(bcp));
                if (bcp) bcp_destroy(bcp);
                tcb_destroy(tcb);
            }
            fifo_destroy(fifo);
            break;
        }
        case ROUND_ROBIN: {
            FIFOQueue *fifo = (FIFOQueue *)data_structure;
            while (!fifo_is_empty(fifo)) {
                TCB *tcb = fifo_pop(fifo);
                if (!tcb) continue;
                BCP *bcp = tcb_get_bcp(tcb);
                if (bcp) process_destroy(bcp_get_process(bcp));
                if (bcp) bcp_destroy(bcp);
                tcb_destroy(tcb);
            }
            fifo_destroy(fifo);
            break;
        }
        case PRIORITY: {
            Heap *heap = (Heap *)data_structure;
            while (!heap_is_empty(heap)) {
                TCB *tcb = heap_extract_min(heap);
                if (!tcb) continue;
                BCP *bcp = tcb_get_bcp(tcb);
                if (bcp) process_destroy(bcp_get_process(bcp));
                if (bcp) bcp_destroy(bcp);
                tcb_destroy(tcb);
            }
            heap_destroy(heap);
            break;
        }
        case CFS: {
            TRBTree *tree = (TRBTree *)data_structure;
            RBT_destroy(tree);
            break;
        }
        default:
            break;
    }
}

// Função para recolocar thread na fila de prontos ou finalizar se necessário
bool scheduler_back_to_ready(Scheduler *scheduler, void *data_structure, TCB *thread, int id_cpu) {
    if (!scheduler || !data_structure || !thread) return false;

    BCP *bcp = tcb_get_bcp(thread);
    Process *process = bcp_get_process(bcp);
    int thread_id = tcb_get_ID(thread);

    if(scheduler->type == CFS) {
        RBT_insert((TRBTree *)data_structure, thread);
        if (tcb_is_terminated(thread) && tcb_get_ID(thread) == process_get_n_threads(process))
        {
            process_set_process_state(process, PROCESS_FINISHED);
            write_process(stdout, process, scheduler->type, scheduler->quantum, id_cpu);
        }
        return true;
    }

    // Verifica se a thread terminou
    if (tcb_get_total_time(thread) <= 0) {        
        // Verifica se todas as threads do processo terminaram
        if (process_has_finished(process, thread_id)) {
            process_set_process_state(process, PROCESS_FINISHED);
            write_process(stdout, process, scheduler->type, scheduler->quantum, id_cpu);
            
            // Libera recursos
            bcp_destroy(bcp);
            tcb_destroy(thread);
            process_destroy(process);
        } else {
            // Apenas esta thread terminou, mas o processo continua
            tcb_destroy(thread);
        }
        return false;
    } else {
        // Thread não terminou, recoloca na fila de prontos
        switch (scheduler->type) {
            case ROUND_ROBIN:
                fifo_push((FIFOQueue *)data_structure, thread);
                break;
            case PRIORITY:
                heap_insert((Heap *)data_structure, thread);
                break;
            case FIRST_COME_FIRST_SERVED:
                // Não preemptivo, mantém running até terminar
                break;
            default:
                break;
        }
    }
    return true;
}

void destroy_scheduler(Scheduler *scheduler) {
    if (scheduler) {
        free(scheduler);
    }
}

// Função principal de mudança de contexto
TCB *scheduler_change_context(Scheduler *scheduler, void *data_structure, int id_cpu, int PID) {
    if (!scheduler || !data_structure) return NULL;

    // Seleciona a próxima thread baseada no algoritmo de escalonamento
    TCB *next_thread = NULL;
    switch (scheduler->type) {
        case ROUND_ROBIN:
            next_thread = round_robin_scheduler((FIFOQueue *)data_structure, PID);
            break;
        case PRIORITY:
            next_thread = priority_scheduler((Heap *)data_structure);
            break;
        case FIRST_COME_FIRST_SERVED:
            next_thread = first_come_first_served_scheduler((FIFOQueue *)data_structure);
            break;
        case CFS:
            next_thread = cfs_scheduler((TRBTree *)data_structure);
            break;
        default:
            break;
    }
    // Atualiza estado do próximo processo para running
    if (next_thread) {
        Process *next_process = bcp_get_process(tcb_get_bcp(next_thread));
        process_set_process_state(next_process, PROCESS_RUNNING);
        bool write = true;
        if(PID == process_get_PID(bcp_get_process(tcb_get_bcp(next_thread)))) write = false;
        else if (scheduler->type == FIRST_COME_FIRST_SERVED && bcp_get_cpu_time(tcb_get_bcp(next_thread)) > 0) write = false;
        if(write) write_process(stdout, next_process, scheduler->type, scheduler->quantum, id_cpu);
    }
    return next_thread;
}

void scheduler_add_to_ready(Scheduler *scheduler, void *data_structure, BCP *bcp) {
    if (!scheduler || !data_structure) {
        return;
    }
    process_set_process_state(bcp_get_process(bcp), PROCESS_READY);
    int n_threads = process_get_n_threads(bcp_get_process(bcp));
    double time_per_thread = process_get_duration(bcp_get_process(bcp))/n_threads;
    for (int i = 0; i < n_threads; i++)
    {
        TCB *tcp = tcb_create(bcp, time_per_thread, i+1);
        switch (scheduler->type) {
            case ROUND_ROBIN:
                fifo_push((FIFOQueue *)data_structure, tcp);
                break;

            case PRIORITY:
                heap_insert((Heap *)data_structure, tcp);
                break;

            case FIRST_COME_FIRST_SERVED:
                fifo_push((FIFOQueue *)data_structure, tcp);
                break;

            case CFS:
                RBT_insert((TRBTree *)data_structure, tcp); // chave = tcp*, valor irrelevante
                break;

            default:
                break;
        }
    }
}

void *scheduler_create_ready_set(Scheduler *scheduler, int n_processes){
    if (!scheduler) {
        return NULL;
    }

    switch (scheduler->type) {
        case ROUND_ROBIN:
            return fifo_create();
        case PRIORITY:
            return heap_create(n_processes, is_higher_priority_tcb);
        case FIRST_COME_FIRST_SERVED:
            return fifo_create();
        case CFS:
            return RBT_create(compare_cfs_tcb);
        default:
            return NULL;
    }
}
