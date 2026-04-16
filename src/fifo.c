/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#include "fifo.h"

typedef struct FIFONode {
    void* data;
    struct FIFONode* next;
} FIFONode;

struct fifo {
    pthread_mutex_t mutex;
    FIFONode* head;
    FIFONode* tail;
    int size;
};

FIFOQueue* fifo_create() {
    FIFOQueue* queue = malloc(sizeof(FIFOQueue));
    if (!queue) return NULL;
    
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    return queue;
}

void fifo_destroy(FIFOQueue* queue) {
    if (!queue) return;
    
    while (!fifo_is_empty(queue)) {
        fifo_pop(queue);
    }
    pthread_mutex_destroy(&queue->mutex);
    free(queue);
}

void fifo_push(FIFOQueue* queue, void* data) {
    if (!queue || !data) return;

    FIFONode* newNode = malloc(sizeof(FIFONode));
    if (!newNode) return;
    newNode->data = data;
    newNode->next = NULL;

    pthread_mutex_lock(&queue->mutex);
    if (!queue->head){
        queue->head = newNode;
        queue->tail = newNode;
    } else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
    queue->size++;
    pthread_mutex_unlock(&queue->mutex);
}

void* fifo_pop(FIFOQueue* queue) {
    if (!queue || fifo_is_empty(queue)) return NULL;

    pthread_mutex_lock(&queue->mutex);
    FIFONode* temp = queue->head;
    if (!temp) return NULL;
    void* data = temp->data;
    
    queue->head = queue->head->next;
    if (!queue->head) {
        queue->tail = NULL;
        queue->size = 0;
    } else {
        queue->size--;
    }

    free(temp);
    pthread_mutex_unlock(&queue->mutex);
    return data;
}

void* fifo_peek(FIFOQueue* queue) {
    if (!queue || fifo_is_empty(queue)) return NULL;

    pthread_mutex_lock(&queue->mutex);
    void* data = queue->head->data;
    pthread_mutex_unlock(&queue->mutex);
    return data;
}

int fifo_is_empty(FIFOQueue* queue) {
    if (!queue) return 1;

    pthread_mutex_lock(&queue->mutex);
    int empty = (queue->size == 0);
    pthread_mutex_unlock(&queue->mutex);
    return empty;
}

int fifo_size(FIFOQueue* queue) {
    if (!queue) return 0;
    int sz = 0;
    pthread_mutex_lock(&queue->mutex);
    sz = queue->size;
    pthread_mutex_unlock(&queue->mutex);
    return sz;
}