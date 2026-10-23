/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#include "bcp.h"
struct bcp {
    Process *process;
    pthread_mutex_t lock; // Mutex for thread safety
    double cpu_time;     // acumulado total
    double vruntime;     // NOVO: tempo virtual usado pelo CFS
};



BCP *bcp_create(Process *process) {
    BCP *bcp = malloc(sizeof(BCP));
    if (bcp == NULL) {
        return NULL; // Handle memory allocation failure
    }
    bcp->process = process;
    bcp->cpu_time = 0;
    bcp->vruntime = 0;
    pthread_mutex_init(&bcp->lock, NULL);
    return bcp;
}

void bcp_destroy(BCP *bcp) {
    if (bcp != NULL) {
        pthread_mutex_destroy(&bcp->lock);
        free(bcp);
    }
}

void bcp_lock(BCP *bcp) {
    pthread_mutex_lock(&bcp->lock);
}

void bcp_unlock(BCP *bcp) {
    pthread_mutex_unlock(&bcp->lock);
}

void bcp_print(BCP *bcp){
    process_print(bcp->process);
    printf("CPU_TIME: %.2f\n\n", bcp->cpu_time);
}

Process *bcp_get_process(BCP *bcp) {
    return bcp->process;
}

ProcessState bcp_get_process_state(BCP* bcp){
    return process_get_process_state(bcp->process);
}

double bcp_get_vruntime(BCP *bcp){
    return bcp? bcp->vruntime : -1;
}

double bcp_get_cpu_time(BCP *bcp){
    return bcp? bcp->cpu_time : -1;
}


void bcp_increment_vruntime(BCP *bcp, double time){
    if (bcp)
        bcp->vruntime += time;
}

void bcp_increment_cpu_time(BCP *bcp, double time){
    if (bcp)
        bcp->cpu_time += time; 
}
