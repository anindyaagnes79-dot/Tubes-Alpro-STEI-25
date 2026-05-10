#include "cache.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void cache_init(CacheMap *C, int capacity) {
    C->capacity = capacity;
    C->size     = 0;
    C->head     = 0;
    C->entries  = (CacheEntry *)calloc(capacity, sizeof(CacheEntry));
    if (!C->entries) { fprintf(stderr, "Error: calloc CacheMap\n"); exit(1); }
    for (int i = 0; i < capacity; i++) C->entries[i].in_use = 0;
}

void cache_free(CacheMap *C) {
    free(C->entries);
    C->entries = NULL;
    C->size = C->capacity = 0;
}

int cache_get(CacheMap *C, const char *url, char *out_content) {
    for (int i = 0; i < C->capacity; i++) {
        if (C->entries[i].in_use && strcmp(C->entries[i].key, url) == 0) {
            strcpy(out_content, C->entries[i].value);
            return 1;
        }
    }
    return 0;
}

void cache_put(CacheMap *C, const char *url, const char *content) {
    // Update //
    for (int i = 0; i < C->capacity; i++) {
        if (C->entries[i].in_use && strcmp(C->entries[i].key, url) == 0) {
            strncpy(C->entries[i].value, content, MAX_CONTENT_LEN - 1);
            C->entries[i].value[MAX_CONTENT_LEN - 1] = '\0';
            return;
        }
    }

    int slot;
    if (C->size < C->capacity) {
        // Cari slot kosong //
        slot = -1;
        for (int i = 0; i < C->capacity; i++)
            if (!C->entries[i].in_use) { slot = i; break; }
        C->size++;
    } else {
        // Evict paling lama (FIFO) //
        slot    = C->head;
        C->head = (C->head + 1) % C->capacity;
    }

    strncpy(C->entries[slot].key,   url,     MAX_URL_LEN - 1);
    strncpy(C->entries[slot].value, content, MAX_CONTENT_LEN - 1);
    C->entries[slot].key[MAX_URL_LEN - 1]     = '\0';
    C->entries[slot].value[MAX_CONTENT_LEN-1] = '\0';
    C->entries[slot].in_use = 1;
}

void cache_invalidate(CacheMap *C, const char *url) {
    for (int i = 0; i < C->capacity; i++) {
        if (C->entries[i].in_use && strcmp(C->entries[i].key, url) == 0) {
            C->entries[i].in_use = 0;
            C->entries[i].key[0] = '\0';
            if (C->size > 0) C->size--;
            return;
        }
    }
}