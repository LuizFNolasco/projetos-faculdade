/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct heap {
    void **array;
    int (*priority_fn)(void *a, void *b);
    int capacity;
    int size;
} Heap;

static void swap(void **a, void **b) {
    void *temp = *a;
    *a = *b;
    *b = temp;
}

Heap *heap_create(int capacity, int (*priority_fn)(void *a, void *b)) {
    Heap *heap = malloc(sizeof(Heap));
    if (!heap) return NULL;

    heap->array = malloc(capacity * sizeof(void *));
    if (!heap->array) {
        free(heap);
        return NULL;
    }

    heap->capacity = capacity;
    heap->size = 0;
    heap->priority_fn = priority_fn;
    return heap;
}

void heap_destroy(Heap *heap) {
    if (!heap) return;
    free(heap->array);
    free(heap);
}

void heap_insert(Heap *heap, void *value) {
    if (!heap || !value) return;

    // Expande se necessário
    if (heap->size >= heap->capacity) {
        int new_capacity = heap->capacity * 2;
        void **new_array = realloc(heap->array, new_capacity * sizeof(void *));
        if (!new_array) {
            return;
        }
        heap->array = new_array;
        heap->capacity = new_capacity;
    }

    int index = heap->size;
    heap->array[index] = value;
    heap->size++;

    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->priority_fn(heap->array[index], heap->array[parent]) < 0) {
            swap(&heap->array[index], &heap->array[parent]);
            index = parent;
        } else break;
    }
}

void *heap_extract_min(Heap *heap) {
    if (!heap) return NULL;

    if (heap->size == 0) {
        return NULL;
    }

    void *min = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;

    int index = 0;
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < heap->size && 
            heap->priority_fn(heap->array[left], heap->array[smallest]) < 0) {
            smallest = left;
        }
        if (right < heap->size &&
            heap->priority_fn(heap->array[right], heap->array[smallest]) < 0) {
            smallest = right;
        }

        if (smallest == index) break;

        swap(&heap->array[index], &heap->array[smallest]);
        index = smallest;
    }
    return min;
}

void *heap_peek(Heap *heap) {
    if (!heap) return NULL;
    void *min = (heap->size > 0) ? heap->array[0] : NULL;
    return min;
}

bool heap_is_empty(Heap *heap) {
    if (!heap) return 1;
    int result = (heap->size == 0);
    return result;
}
