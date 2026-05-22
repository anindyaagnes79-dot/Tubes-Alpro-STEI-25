#include "f07.h"
#include <stdio.h>
#include <string.h>

void f07_init(HistoryTab *h) {
    h->total            = 0;
    h->posisi_sekarang  = -1; 
}

void f07_tambah_url(HistoryTab *h, const char *url) {

    if (h->posisi_sekarang < h->total - 1) {
        h->total = h->posisi_sekarang + 1;
    }

    if (h->total >= MAX_HISTORY) {
        printf("Peringatan: History sudah penuh, URL lama dihapus.\n");
        int i;
        for (i = 0; i < MAX_HISTORY - 1; i++) {
            strncpy(h->urls[i], h->urls[i + 1], MAX_URL_LEN - 1);
            h->urls[i][MAX_URL_LEN - 1] = '\0';
        }
        h->total--;
        h->posisi_sekarang--;
    }

    strncpy(h->urls[h->total], url, MAX_URL_LEN - 1);
    h->urls[h->total][MAX_URL_LEN - 1] = '\0';
    h->total++;
    h->posisi_sekarang = h->total - 1;
}

static void buka_halaman_di_posisi(HistoryTab *h,
                                   WebPageSet *pages,
                                   CacheMap   *cache,
                                   AdjMatrix  *adj)
{
    const char *url = h->urls[h->posisi_sekarang];

    OpenPageContext ctx;
    int cache_hit;

    int ditemukan = f03_open_page(url, pages, cache, adj, &ctx, &cache_hit);

    if (!ditemukan) {
        printf("Halaman '%s' tidak ditemukan di database (mungkin sudah dihapus).\n\n", url);
        return;
    }

    f03_display_page(&ctx, cache_hit);
}

int f07_back(HistoryTab *h,
             WebPageSet *pages,
             CacheMap   *cache,
             AdjMatrix  *adj)
{
    if (h->posisi_sekarang < 0) {
        printf("ERROR: BACK TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SEBELUMNYA!\n\n");
        return 0;
    }

    if (h->posisi_sekarang == 0) {
        printf("ERROR: BACK TIDAK BISA DIGUNAKAN LAGI KARENA TIDAK ADA HALAMAN SEBELUMNYA!\n\n");
        buka_halaman_di_posisi(h, pages, cache, adj);
        return 0;
    }

    h->posisi_sekarang--;
    printf("BACK: KEMBALI KE HALAMAN %s\n\n", h->urls[h->posisi_sekarang]);
    buka_halaman_di_posisi(h, pages, cache, adj);
    return 1;
}

int f07_forward(HistoryTab *h,
                WebPageSet *pages,
                CacheMap   *cache,
                AdjMatrix  *adj)
{
    if (h->posisi_sekarang < 0) {
        printf("ERROR: FORWARD TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SELANJUTNYA!\n\n");
        return 0;
    }

    if (h->posisi_sekarang >= h->total - 1) {
        printf("ERROR: FORWARD TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SELANJUTNYA!\n\n");
        buka_halaman_di_posisi(h, pages, cache, adj);
        return 0;
    }

    h->posisi_sekarang++;
    printf("FORWARD: KEMBALI KE HALAMAN %s\n\n", h->urls[h->posisi_sekarang]);
    buka_halaman_di_posisi(h, pages, cache, adj);
    return 1;
}

const char *f07_url_sekarang(const HistoryTab *h) {
    if (h->posisi_sekarang < 0 || h->total == 0) {
        return NULL;
    }
    return h->urls[h->posisi_sekarang];
}
