#include "f03.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static void trim(char *s) {
    int len = (int)strlen(s);
    while (len > 0 && isspace((unsigned char)s[len-1])) s[--len] = '\0';
    int start = 0;
    while (s[start] && isspace((unsigned char)s[start])) start++;
    if (start > 0) memmove(s, s + start, len - start + 1);
}

// f03_open_page //
int f03_open_page(const char   *url,
                  WebPageSet   *pages,
                  CacheMap     *cache,
                  AdjMatrix    *adj,
                  OpenPageContext *ctx,
                  int          *out_cache_hit)
{
    strncpy(ctx->url, url, MAX_URL_LEN - 1);
    ctx->url[MAX_URL_LEN - 1] = '\0';
    ctx->linked_count = 0;

    // Langkah 1: Cache check //
    char cached[MAX_CONTENT_LEN];
    if (cache_get(cache, url, cached)) {
        *out_cache_hit = 1;
        strncpy(ctx->content, cached, MAX_CONTENT_LEN - 1);
        ctx->content[MAX_CONTENT_LEN - 1] = '\0';
    } else {
        *out_cache_hit = 0;
        // Linear search di WebPageSet (ADT Set – sesuai spesifikasi F03) //
        WebPage *found = wpset_find_by_url(pages, url);
        if (!found) return 0;  // 404 //

        strncpy(ctx->content, found->content, MAX_CONTENT_LEN - 1);
        ctx->content[MAX_CONTENT_LEN - 1] = '\0';
        cache_put(cache, url, found->content);
    }

    // Langkah 2: Ambil linked pages dari AdjMatrix (D01 EB). Cari indeks halaman ini di WebPageSet //
    int src_idx = -1;
    for (int i = 0; i < pages->size; i++) {
        if (strcmp(pages->data[i].web_url, url) == 0) { src_idx = i; break; }
    }

    if (src_idx != -1) {
        int dst_indices[MAX_LINKED];
        int n = adj_get_neighbors(adj, src_idx, dst_indices, MAX_LINKED);
        int count = 0;
        for (int k = 0; k < n && count < MAX_LINKED; k++) {
            int di = dst_indices[k];
            if (di < pages->size) {
                strncpy(ctx->linked_urls[count],
                        pages->data[di].web_url,
                        MAX_URL_LEN - 1);
                ctx->linked_urls[count][MAX_URL_LEN - 1] = '\0';
                count++;
            }
        }
        ctx->linked_count = count;
    }

    return 1;
}

// f03_display_page //
void f03_display_page(const OpenPageContext *ctx, int cache_hit) {
    if (cache_hit) {
        printf("[Status: Cache-Hit]\n");
        printf("Mengambil data dari cache...\n\n");
    } else {
        printf("[Status: Cache-Miss]\n");
        printf("Mengambil data dari database...\n\n");
    }

    printf("%s\n", ctx->content);

    if (ctx->linked_count > 0) {
        printf("\nLinked pages:\n");
        for (int i = 0; i < ctx->linked_count; i++)
            printf("  [%d] %s\n", i + 1, ctx->linked_urls[i]);
    }
    printf("\n");
}

// f03_run_page_session. Loop interaktif dalam halaman yang sedang terbuka. action: 0=home, 1=back, 2=forward, 3=openlinked //
void f03_run_page_session(const OpenPageContext *ctx,
                          char *next_url,
                          int  *action)
{
    char input[512];

    while (1) {
        printf(">>> ");
        if (!fgets(input, sizeof(input), stdin)) { *action = 0; return; }

        int len = (int)strlen(input);
        while (len > 0 && (input[len-1] == '\n' || input[len-1] == '\r'))
            input[--len] = '\0';
        trim(input);
        if (len == 0) continue;

        if (strcmp(input, "home") == 0)    { *action = 0; return; }
        if (strcmp(input, "back") == 0)    { *action = 1; return; }
        if (strcmp(input, "forward") == 0) { *action = 2; return; }

        if (strncmp(input, "openlinked", 10) == 0) {
            char *arg = input + 10;
            while (*arg == ' ') arg++;

            if (*arg == '\0') { printf("Usage: openlinked <nomor>\n"); continue; }

            // Validasi numerik //
            int valid = 1;
            for (int i = 0; arg[i]; i++)
                if (!isdigit((unsigned char)arg[i])) { valid = 0; break; }

            if (!valid) { printf("Input tidak valid! Masukkan angka.\n"); continue; }

            int x = atoi(arg);
            if (ctx->linked_count == 0) {
                printf("ERROR: HALAMAN TIDAK MEMILIKI TAUTAN YANG BISA DIBUKA!\n\n");
                // Tampilkan ulang halaman sesuai contoh spesifikasi //
                printf("%s\n\n", ctx->content);
                continue;
            }
            if (x < 1 || x > ctx->linked_count) {
                printf("Nomor tidak valid! Pilih antara 1 sampai %d.\n",
                       ctx->linked_count);
                continue;
            }

            strncpy(next_url, ctx->linked_urls[x - 1], MAX_URL_LEN - 1);
            next_url[MAX_URL_LEN - 1] = '\0';
            *action = 3;
            return;
        }

        printf("Perintah tidak valid! Saat membuka halaman tersedia: "
               "home, back, forward, openlinked <x>\n");
    }
}