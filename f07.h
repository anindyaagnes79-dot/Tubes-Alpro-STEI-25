#ifndef F07_H
#define F07_H

#include "f03.h"
#include "f04.h"
#include "web_pages.h"
#include "adj_matrix.h"

/* Ukuran maksimal history per tab */
#define MAX_HISTORY 100

/*
 * HistoryTab - menyimpan riwayat URL yang dibuka di satu tab
 *
 * Cara kerjanya seperti array biasa dengan dua penunjuk:
 *
 *   urls[0]  urls[1]  urls[2]  urls[3]  urls[4]
 *   -------  -------  -------  -------  -------
 *   a.com    b.com    c.com    (kosong) (kosong)
 *                       ^
 *                    posisi_sekarang = 2
 *                    total = 3
 *
 * - "total" = berapa URL yang tersimpan
 * - "posisi_sekarang" = kamu sedang di URL nomor berapa (indeks)
 *
 * Kalau kamu buka halaman baru saat posisi_sekarang bukan di ujung,
 * semua URL setelah posisi itu dihapus (forward history dipotong).
 */
typedef struct {
    char urls[MAX_HISTORY][MAX_URL_LEN]; /* daftar URL yang pernah dibuka */
    int  total;                          /* jumlah URL tersimpan          */
    int  posisi_sekarang;                /* posisi kamu saat ini          */
} HistoryTab;

/* ----- Fungsi-fungsi F07 ----- */

/* Inisialisasi history (kosongkan semua) */
void f07_init(HistoryTab *h);

/*
 * Tambah URL baru ke history setelah membuka halaman.
 * Jika posisi bukan di ujung, forward history dipotong dulu.
 */
void f07_tambah_url(HistoryTab *h, const char *url);

/*
 * Jalankan perintah BACK.
 * - Jika bisa back: tampilkan halaman sebelumnya, return 1
 * - Jika tidak bisa: tampilkan pesan error, return 0
 */
int f07_back(HistoryTab    *h,
             WebPageSet    *pages,
             CacheMap      *cache,
             AdjMatrix     *adj);

/*
 * Jalankan perintah FORWARD.
 * - Jika bisa forward: tampilkan halaman berikutnya, return 1
 * - Jika tidak bisa: tampilkan pesan error, return 0
 */
int f07_forward(HistoryTab    *h,
                WebPageSet    *pages,
                CacheMap      *cache,
                AdjMatrix     *adj);

/*
 * Ambil URL di posisi sekarang (untuk ditampilkan ulang setelah error).
 * Return NULL jika history kosong.
 */
const char *f07_url_sekarang(const HistoryTab *h);

#endif
