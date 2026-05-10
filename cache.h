#ifndef CACHE_H
#define CACHE_H

#include "web_pages.h"

typedef struct {
    char key[MAX_URL_LEN];
    char value[MAX_CONTENT_LEN];
    int  in_use;
} CacheEntry;

typedef struct {
    CacheEntry *entries;
    int         capacity;  // CACHE_MAX_AMOUNT //
    int         size;      // entri aktif saat ini //
    int         head;      // indeks entri paling lama (FIFO) //
} CacheMap;

void cache_init(CacheMap *C, int capacity);
void cache_free(CacheMap *C);

// Return 1 = Cache-Hit (isi out_content); 0 = Cache-Miss //
int  cache_get(CacheMap *C, const char *url, char *out_content);
void cache_put(CacheMap *C, const char *url, const char *content);
void cache_invalidate(CacheMap *C, const char *url);

#endif 