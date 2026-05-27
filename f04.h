#ifndef CACHE_H
#define CACHE_H
#include "f03.h"
#include <string.h>
#define CACHE_MAX_AMOUNT 10
#define CACHE_EMPTY -1

typedef struct {
    char url[MAX_URL_LEN];
    WebPage page;
    int prev;
    int next;
    int used;

} Cache;

typedef struct {
    Cache nodes[CACHE_MAX_AMOUNT];
    int head;
    int tail;
    int size;
    int capacity;

} LRUCache;

static int node_find_free(LRUCache *C) {
	int i;
    for (i=0; i<C->capacity; i++) {
        if (!C->nodes[i].used) return i;
    }
    return CACHE_EMPTY;
}

static int node_find(LRUCache *C, const char *url) {
	int i;
    for (i=0; i<C->capacity; i++) {
        if (C->nodes[i].used && strcmp(C->nodes[i].url, url) == 0) {
            return i;
        }
    }
    return CACHE_EMPTY;
}

static void node_detach(LRUCache *C, int i) {
    int p = C->nodes[i].prev;
    int n = C->nodes[i].next;

    if (p != CACHE_EMPTY) {
        C->nodes[p].next = n;
    } else {
        C->head = n;
    }

    if (n != CACHE_EMPTY) {
        C->nodes[n].prev = p;
    } else {
        C->tail = p;
    }

    C->nodes[i].prev = C->nodes[i].next = CACHE_EMPTY;
}

static void node_push_front(LRUCache *C, int i) {
    C->nodes[i].next = C->head;
    C->nodes[i].prev = CACHE_EMPTY;
    if (C->head != CACHE_EMPTY) {
        C->nodes[C->head].prev = i;
    }
    C->head = i;
    if (C->tail == CACHE_EMPTY) {
        C->tail = i;
    }
}

static void cache_init (LRUCache *C, int capacity) {
    C->capacity = (capacity>0 && capacity<=CACHE_MAX_AMOUNT) ? capacity : CACHE_MAX_AMOUNT;
    C->head = C->tail = CACHE_EMPTY;
    C->size = 0;
    int i;
    for (i=0; i<C->capacity; i++) {
        C->nodes[i].used = 0;
        C->nodes[i].prev = C->nodes[i].next = CACHE_EMPTY;
    }
};

static WebPage *cache_get (LRUCache *C, const char *url) {
    int i = node_find(C, url);
    if (i == CACHE_EMPTY) return NULL;
 
    node_detach(C, i);
    node_push_front(C, i);
    return &C->nodes[i].page;
};

static void cache_put (LRUCache *C, const WebPage *page) {
    int i = node_find(C, page->web_url);
    if (i != CACHE_EMPTY) {
        C->nodes[i].page = *page;
        node_detach(C, i);
        node_push_front(C, i);
        return;
    }
 
    /* Evict LRU (tail) jika penuh */
    if (C->size >= C->capacity) {
        int lru = C->tail;
        node_detach(C, lru);
        C->nodes[lru].used = 0;
        C->size--;
    }
 
    int slot = node_find_free(C);
    strncpy(C->nodes[slot].url, page->web_url, MAX_URL_LEN - 1);
    C->nodes[slot].url[MAX_URL_LEN - 1] = '\0';
    C->nodes[slot].page = *page;
    C->nodes[slot].used = 1;
    node_push_front(C, slot);
    C->size++;
};

static int cache_remove(LRUCache *C, const char *url) {
    int i = node_find(C, url);
    if (i == CACHE_EMPTY) return 0;
    node_detach(C,i);
    C->nodes[i].used = 0;
    C->size--;
    return 1;
};

static int cache_size (const LRUCache *C) {
    return C->size;
}

#endif
