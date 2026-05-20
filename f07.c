#include "f07.h"
#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* f07_init - kosongkan history                                        */
/* ------------------------------------------------------------------ */
void f07_init(HistoryTab *h) {
    h->total            = 0;
    h->posisi_sekarang  = -1; /* -1 artinya belum ada halaman sama sekali */
}

/* ------------------------------------------------------------------ */
/* f07_tambah_url - simpan URL baru ke history                        */
/* ------------------------------------------------------------------ */
void f07_tambah_url(HistoryTab *h, const char *url) {

    /* Potong forward history dulu kalau posisi bukan di ujung */
    /* Contoh: history = [a, b, c, d], posisi = 1 (b)         */
    /* Buka e → history jadi [a, b, e], posisi = 2            */
    if (h->posisi_sekarang < h->total - 1) {
        h->total = h->posisi_sekarang + 1;
    }

    /* Jangan simpan kalau history sudah penuh */
    if (h->total >= MAX_HISTORY) {
        printf("Peringatan: History sudah penuh, URL lama dihapus.\n");
        /* Geser semua ke kiri (hapus yang paling lama) */
        int i;
        for (i = 0; i < MAX_HISTORY - 1; i++) {
            strncpy(h->urls[i], h->urls[i + 1], MAX_URL_LEN - 1);
            h->urls[i][MAX_URL_LEN - 1] = '\0';
        }
        h->total--;
        h->posisi_sekarang--;
    }

    /* Simpan URL baru */
    strncpy(h->urls[h->total], url, MAX_URL_LEN - 1);
    h->urls[h->total][MAX_URL_LEN - 1] = '\0';
    h->total++;
    h->posisi_sekarang = h->total - 1;
}

/* ------------------------------------------------------------------ */
/* buka_halaman_di_posisi - helper: buka halaman berdasarkan posisi   */
/* ------------------------------------------------------------------ */
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
        /* Halaman sudah dihapus dari database */
        printf("Halaman '%s' tidak ditemukan di database (mungkin sudah dihapus).\n\n", url);
        return;
    }

    f03_display_page(&ctx, cache_hit);
}

/* ------------------------------------------------------------------ */
/* f07_back - kembali ke halaman sebelumnya                           */
/* ------------------------------------------------------------------ */
int f07_back(HistoryTab *h,
             WebPageSet *pages,
             CacheMap   *cache,
             AdjMatrix  *adj)
{
    /* Kasus 1: belum pernah buka halaman apapun */
    if (h->posisi_sekarang < 0) {
        printf("ERROR: BACK TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SEBELUMNYA!\n\n");
        return 0;
    }

    /* Kasus 2: sudah di halaman paling awal */
    if (h->posisi_sekarang == 0) {
        printf("ERROR: BACK TIDAK BISA DIGUNAKAN LAGI KARENA TIDAK ADA HALAMAN SEBELUMNYA!\n\n");
        /* Tampilkan ulang halaman paling awal */
        buka_halaman_di_posisi(h, pages, cache, adj);
        return 0;
    }

    /* Kasus 3: bisa back normal */
    h->posisi_sekarang--;
    printf("BACK: KEMBALI KE HALAMAN %s\n\n", h->urls[h->posisi_sekarang]);
    buka_halaman_di_posisi(h, pages, cache, adj);
    return 1;
}

/* ------------------------------------------------------------------ */
/* f07_forward - maju ke halaman berikutnya                           */
/* ------------------------------------------------------------------ */
int f07_forward(HistoryTab *h,
                WebPageSet *pages,
                CacheMap   *cache,
                AdjMatrix  *adj)
{
    /* Kasus 1: belum pernah buka halaman apapun */
    if (h->posisi_sekarang < 0) {
        printf("ERROR: FORWARD TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SELANJUTNYA!\n\n");
        return 0;
    }

    /* Kasus 2: sudah di halaman paling akhir (atau forward history habis) */
    if (h->posisi_sekarang >= h->total - 1) {
        printf("ERROR: FORWARD TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SELANJUTNYA!\n\n");
        /* Tampilkan ulang halaman terakhir */
        buka_halaman_di_posisi(h, pages, cache, adj);
        return 0;
    }

    /* Kasus 3: bisa forward normal */
    h->posisi_sekarang++;
    printf("FORWARD: KEMBALI KE HALAMAN %s\n\n", h->urls[h->posisi_sekarang]);
    buka_halaman_di_posisi(h, pages, cache, adj);
    return 1;
}

/* ------------------------------------------------------------------ */
/* f07_url_sekarang - ambil URL di posisi sekarang                    */
/* ------------------------------------------------------------------ */
const char *f07_url_sekarang(const HistoryTab *h) {
    if (h->posisi_sekarang < 0 || h->total == 0) {
        return NULL;
    }
    return h->urls[h->posisi_sekarang];
}
