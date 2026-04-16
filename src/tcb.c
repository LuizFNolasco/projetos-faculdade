/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#include "tcb.h"
#include "bcp.h"
#include "process.h"   // para process_getPriority
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
struct tcb {
    BCP *bcp;
    pthread_t thread;           /* thread "representativa" — criada em tcb_create */
    pthread_mutex_t mutex;      /* protege os campos abaixo */
    int ID;
    double total_time;          /* tempo restante que a thread precisa executar (ms) */
    double executed_time;       /* tempo já executado por esta thread (ms) */
    int terminated;             /* 0 = rodando/pendente, 1 = terminou */
};

/* Exec thread: função simples que existe apenas para que
   pthread_create tenha algo para executar. A execução "real"
   é feita por tcb_working_thread quando CPU chama. */
static void *exec_thread(void *arg) {
    (void)arg;
    /* thread "vazia" — retorno imediato; a existência dela permite
       fazer pthread_join(tcb->thread) sem undefined behaviour.*/
    return NULL;
}

TCB *tcb_create(BCP *bcp, double total_time, int id) {
    if (!bcp) return NULL;
    TCB *tcb = (TCB *)malloc(sizeof(TCB));
    if (!tcb) return NULL;

    tcb->bcp = bcp;
    tcb->total_time = (total_time < 0.0) ? 0.0 : total_time;
    tcb->executed_time = 0.0;
    tcb->terminated = 0;
    tcb->ID = id;
    pthread_mutex_init(&tcb->mutex, NULL);

    if (pthread_create(&tcb->thread, NULL, exec_thread, tcb) != 0) {
        pthread_mutex_destroy(&tcb->mutex);
        free(tcb);
        return NULL;
    }

    return tcb;
}

void tcb_destroy(TCB *tcb) {
    if (!tcb) return;
    pthread_join(tcb->thread, NULL);
    pthread_mutex_destroy(&tcb->mutex);
    free(tcb);
}

BCP *tcb_get_bcp(TCB *tcb) {
    if (!tcb || !tcb->bcp) return NULL;
    return tcb->bcp;
}

int tcb_get_ID(TCB *tcb){
    return tcb? tcb->ID : -1;
}

double tcb_get_total_time(TCB *tcb) {
    if (!tcb) return -1.0;
    pthread_mutex_lock(&tcb->mutex);
    double v = tcb->total_time;
    pthread_mutex_unlock(&tcb->mutex);
    return v;
}

double tcb_get_executed_time(TCB *tcb) {
    if (!tcb) return -1.0;
    pthread_mutex_lock(&tcb->mutex);
    double v = tcb->executed_time;
    pthread_mutex_unlock(&tcb->mutex);
    return v;
}

void tcb_add_executed_time(TCB *tcb, double quantum) {
    if (!tcb || quantum <= 0.0) return;
    pthread_mutex_lock(&tcb->mutex);
    double used = (tcb->total_time >= quantum) ? quantum : tcb->total_time;
    tcb->executed_time += used;
    tcb->total_time -= used;
    pthread_mutex_unlock(&tcb->mutex);
}

int tcb_is_terminated(TCB *tcb) {
    if (!tcb) return true;
    pthread_mutex_lock(&tcb->mutex);
    int v = tcb->terminated;
    pthread_mutex_unlock(&tcb->mutex);
    return v;
}

void tcb_set_terminated(TCB *tcb) {
    if (!tcb) return;
    pthread_mutex_lock(&tcb->mutex);
    tcb->terminated = true;
    pthread_mutex_unlock(&tcb->mutex);
}

/* --- prioridade -> peso --- */
double tcb_priority_to_weight(int priority) {
    if (priority < 0) priority = 0;
    if (priority > 40) priority = 40;
    return 1.0 + (40 - priority) * 0.1;
}

/* --- execução por quantum --- */
/* Esta função é chamada pela CPU para simular a execução da TCB por
   'quantum' milissegundos. Ela:
   - atualiza valores da TCB (executed / total),
   - atualiza bcp->cpu_time e bcp->vruntime (usando lock do BCP),
   - marca thread como terminada se necessário.
*/
void tcb_working_thread(TCB *tcb, double quantum) {
    if (!tcb || quantum <= 0.0) return;
    pthread_mutex_lock(&tcb->mutex);
    if (tcb->total_time <= 0.0) {
        pthread_mutex_unlock(&tcb->mutex);
        return;
    }
    double used = (tcb->total_time >= quantum) ? quantum : tcb->total_time;
    tcb->executed_time += used;
    tcb->total_time -= used;
    int finished_now = (tcb->total_time <= 0.0) ? 1 : 0;
    pthread_mutex_unlock(&tcb->mutex);

    BCP *bcp = tcb->bcp;
    if (bcp) {
        double weight = tcb_priority_to_weight(process_get_priority(bcp_get_process(bcp)));
        bcp_lock(bcp);
        bcp_increment_cpu_time(bcp, used);
        bcp_increment_vruntime(bcp, used / weight);
        bcp_unlock(bcp);
    }
    if (finished_now) {
        tcb_set_terminated(tcb);
    }
    usleep(quantum);
}
