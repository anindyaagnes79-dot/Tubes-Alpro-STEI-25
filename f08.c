#include "f08.h"
#include <stdio.h>
#include <string.h>

void f08_history(HistoryTab *h, int X) {
    if (h->total == 0 || h->posisi_sekarang == -1) {
        printf("Riwayat kosong untuk tab ini.\n\n");
        return;
    }

    // Validasi input numerik E02
    if (X <= 0) {
        printf("ERROR: Jumlah riwayat yang ingin dilihat harus lebih dari 0!\n\n");
        return;
    }

    printf("Riwayat URL Tab saat ini:\n");
    
    // Menentukan batas bawah (agar tidak print indeks minus kalau X kebesaran)
    int batas_bawah = (h->total - X >= 0) ? (h->total - X) : 0;

    // Looping dari batas_bawah sampai indeks terakhir (total - 1)
    for (int i = batas_bawah; i < h->total; i++) {
        printf("[%d] %s", i + 1, h->urls[i]);
        
        // Beri tanda posisi saat ini
        if (i == h->posisi_sekarang) {
            printf(" <- YOU ARE HERE");
        }
        printf("\n");
    }
    printf("\n");
}

int f08_back_x(HistoryTab *h, int x) {
    // Validasi Input E02
    if (x <= 0) {
        printf("ERROR: Langkah mundur (X) harus bilangan bulat positif!\n\n");
        return 0;
    }

    if (h->posisi_sekarang < 0) {
        printf("ERROR: Tab masih kosong, tidak bisa mundur!\n\n");
        return 0;
    }

    // Cek apakah mundur X langkah melebihi batas (indeks < 0)
    if (h->posisi_sekarang - x < 0) {
        printf("ERROR: Langkah mundur terlalu jauh! (Hanya bisa mundur maksimal %d langkah)\n\n", h->posisi_sekarang);
        return 0;
    }

    // Eksekusi mundur
    h->posisi_sekarang -= x;
    printf("BACK %d: Mundur ke halaman %s\n\n", x, h->urls[h->posisi_sekarang]);
    
    return 1;
}

int f08_forward_x(HistoryTab *h, int x) {
    // Validasi Input E02
    if (x <= 0) {
        printf("ERROR: Langkah maju (X) harus bilangan bulat positif!\n\n");
        return 0;
    }

    if (h->posisi_sekarang < 0) {
        printf("ERROR: Tab masih kosong, tidak bisa maju!\n\n");
        return 0;
    }

    // Cek apakah maju X langkah melebihi URL yang tersimpan
    if (h->posisi_sekarang + x >= h->total) {
        int max_maju = h->total - 1 - h->posisi_sekarang;
        printf("ERROR: Langkah maju terlalu jauh! (Hanya bisa maju maksimal %d langkah)\n\n", max_maju);
        return 0;
    }

    // Eksekusi maju
    h->posisi_sekarang += x;
    printf("FORWARD %d: Maju ke halaman %s\n\n", x, h->urls[h->posisi_sekarang]);

    return 1;
}