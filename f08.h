#ifndef F08_H
#define F08_H

#include "f07.h"
#include "web_pages.h"

/*
 * Menampilkan riwayat tab yang sedang aktif.
 * Sesuai spek, perintah 'history <X>' menampilkan X riwayat terakhir.
 * Jika X melebihi total history, tampilkan semua yang ada.
 */
void f08_history(HistoryTab *h, int X);

/*
 * Jalankan perintah BACK <X>.
 * Mundur sebanyak X langkah.
 * Return 1 jika berhasil, 0 jika gagal.
 */
int f08_back_x(HistoryTab *h, int x);

/*
 * Jalankan perintah FORWARD <X>.
 * Maju sebanyak X langkah.
 * Return 1 jika berhasil, 0 jika gagal.
 */
int f08_forward_x(HistoryTab *h, int x);

#endif
