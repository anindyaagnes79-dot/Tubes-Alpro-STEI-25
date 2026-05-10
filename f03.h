#ifndef F03_H
#define F03_H

#include "web_pages.h"
#include "cache.h"
#include "adj_matrix.h"

#define MAX_LINKED 512

typedef struct {
    char url[MAX_URL_LEN];
    char content[MAX_CONTENT_LEN];
    char linked_urls[MAX_LINKED][MAX_URL_LEN];
    int  linked_count;
} OpenPageContext;

// f03_open_page:Isi ctx dari URL. Gunakan AdjMatrix untuk linked pages (EB/D01).Return: 1 = ditemukan, 0 = 404 out_cache_hit: 1 = Cache-Hit, 0 = Cache-Miss //
int f03_open_page(const char   *url,
                  WebPageSet   *pages,
                  CacheMap     *cache,
                  AdjMatrix    *adj,
                  OpenPageContext *ctx,
                  int          *out_cache_hit);

// Tampilkan konten + linked pages ke terminal //
void f03_display_page(const OpenPageContext *ctx, int cache_hit);

// f03_run_page_session: Loop interaktif dalam halaman. action: 0=home, 1=back, 2=forward, 3=openlinked next_url: diisi jika action==3 //
void f03_run_page_session(const OpenPageContext *ctx,
                          char *next_url,
                          int  *action);

#endif 