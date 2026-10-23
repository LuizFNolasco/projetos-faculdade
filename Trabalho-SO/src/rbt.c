/* Grupo 12
    Luiz Felipe Pereira Nolasco - 2025102338
    Gessica de Souza Silva - 2022102174
*/

#include "rbt.h"
#include <stdlib.h>
#include <assert.h>
#include "tcb.h"

typedef struct TNoRB {
    void *chave;
    int cor;
    struct TNoRB *esq, *dir;
} TNoRB;

struct TRBTree {
    TNoRB *raiz;
    int sz;
    int (*cmp)(const void *, const void *);
};

#define RED 1
#define BLACK 0

// --- Rotations e helpers ---
static int isRed(TNoRB *x) { return x && x->cor == RED; }

static TNoRB *rotLeft(TNoRB *h) {
    TNoRB *x = h->dir;
    h->dir = x->esq;
    x->esq = h;
    x->cor = h->cor;
    h->cor = RED;
    return x;
}

static TNoRB *rotRight(TNoRB *h) {
    TNoRB *x = h->esq;
    h->esq = x->dir;
    x->dir = h;
    x->cor = h->cor;
    h->cor = RED;
    return x;
}

static void flipColors(TNoRB *h) {
    h->cor = RED;
    if(h->esq) h->esq->cor = BLACK;
    if(h->dir) h->dir->cor = BLACK;
}

// --- Inserção LLRBT ---
static TNoRB *insert_node(TNoRB *h, void *key, int (*cmp)(const void *, const void *)) {
    if (!h) {
        TNoRB *n = malloc(sizeof(TNoRB));
        n->chave = key;
        n->esq = n->dir = NULL;
        n->cor = RED;
        return n;
    }

    int c = cmp(key, h->chave);
    if (c < 0) h->esq = insert_node(h->esq, key, cmp);
    else if (c > 0) h->dir = insert_node(h->dir, key, cmp);
    else h->chave = key; // Atualiza chave se igual (não deve ocorrer em CFS)

    if (isRed(h->dir) && !isRed(h->esq)) h = rotLeft(h);
    if (isRed(h->esq) && isRed(h->esq->esq)) h = rotRight(h);
    if (isRed(h->esq) && isRed(h->dir)) flipColors(h);

    return h;
}

// --- Busca ---
static void *search_node(TNoRB *h, void *key, int (*cmp)(const void *, const void *)) {
    if (!h) return NULL;
    int c = cmp(key, h->chave);
    if (c == 0) return h->chave;
    if (c < 0) return search_node(h->esq, key, cmp);
    else return search_node(h->dir, key, cmp);
}

// --- Min ---
// Retorna o TCB "mínimo" da árvore que ainda não terminou
static TCB *cfs_min_node(TRBTree *tree, TNoRB *h) {
    if (!h) return NULL;

    TCB *left_min = cfs_min_node(tree, h->esq);
    TCB *right_min = cfs_min_node(tree, h->dir);
    TCB *cur = (TCB *)h->chave;

    // Ignora threads terminadas
    if (tcb_is_terminated(cur)) cur = NULL;

    // Começa comparando left e cur
    TCB *min = left_min;
    if (cur) {
        if (!min || tree->cmp(cur, min) < 0)
            min = cur;
    }

    // Compara com right_min
    if (right_min) {
        if (!min || tree->cmp(right_min, min) < 0)
            min = right_min;
    }

    return min;
}

// --- API pública ---
TRBTree *RBT_create(int (*cmp)(const void *, const void *)) {
    TRBTree *t = malloc(sizeof(TRBTree));
    t->raiz = NULL;
    t->cmp = cmp;
    t->sz = 0;
    return t;
}

void destroy_rec(TNoRB *node) {
    if (!node) return;
    destroy_rec(node->esq);
    destroy_rec(node->dir);
    free(node);
}

void RBT_destroy(TRBTree *tree) {
    if (!tree) return;
    destroy_rec(tree->raiz);
    free(tree);
}

void RBT_insert(TRBTree *tree, void *key) {
    tree->raiz = insert_node(tree->raiz, key, tree->cmp);
    if (!tcb_is_terminated((TCB *)key)) tree->sz++;
    else tree->sz--;
    if(tree->raiz) tree->raiz->cor = BLACK;
}

int RBT_is_empty(TRBTree *tree) {
    return tree->sz == 0;
}

void *RBT_search(TRBTree *tree, void *key) {
    return search_node(tree->raiz, key, tree->cmp);
}

int RBT_size(TRBTree *tree) {
    if (!tree) return 0;
    return tree->sz;
}

void *CFS_min(TRBTree *tree) {
    if (!tree || !tree->raiz) return NULL;
    return cfs_min_node(tree, tree->raiz);
}

void RBT_delete(TRBTree *tree, void *key) {
    tree = tree;
    key = key;
}
