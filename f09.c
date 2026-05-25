#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WEB_PAGES 100
#define IDX_UNDEF -1

#include "f00.h"
#include "f03.h"
#include "f04.h"
#include "f09.h"

static WebGraph graph;
static int current_active_page_id = -1;
static int current_linked_ids[MAX_WEB_PAGES];
static int current_linked_count = 0;

void init_graph() {
    graph.ukuran = 0;
    int i;
    int j;
    for (i = 0; i < MAX_WEB_PAGES; i++) {
        for (j = 0; j < MAX_WEB_PAGES; j++) {
            graph.matrix[i][j] = 0;
        }
    }
}

int dapatkan_index_matriks(ListHalaman_Web *l, int id_halaman) {
	int i;
    for (i = 0; i < l->nEff; i++) {
        if (l->HW[i].id == id_halaman) {
            return i;
        }
    }
    return IDX_UNDEF;
}

void add_link_to_graph(ListHalaman_Web *l, LinkedHalaman link) {
    int idx_sumber = dapatkan_index_matriks(l, link.id_sumber);
    int idx_tujuan = dapatkan_index_matriks(l, link.id_tujuan);

    if (idx_sumber != IDX_UNDEF && idx_tujuan != IDX_UNDEF) {
        if (idx_sumber >= MAX_WEB_PAGES || idx_tujuan >= MAX_WEB_PAGES) {
            printf("PERINGATAN: Indeks halaman web di luar kapasitas matriks graf (%d)!\n", MAX_WEB_PAGES);
            printf("Sistem menolak penambahan relasi graf baru demi menjaga kestabilan memori.\n");
            return;
        }

        if (graph.matrix[idx_sumber][idx_tujuan] == 0) {
            graph.matrix[idx_sumber][idx_tujuan] = 1;
            graph.ukuran++;
        }
    }
}

void open_page(ListHalaman_Web *l, char* url) {
	CekDanPerbaruiCache(l, url);
	
    int idx_sumber = BukaHalamanDasar(l, url);

    if (idx_sumber == IDX_UNDEF) {
        printf("ERROR: HALAMAN DENGAN URL %s TIDAK DITEMUKAN!\n", url);
        return;
    }

    current_active_page_id = l->HW[idx_sumber].id;
    current_linked_count = 0;

    printf("\n%s\n", l->HW[idx_sumber].content);
    printf("\nLinked pages:\n");
    int nomor_urut = 1;
    int i;

    for (i = 0; i < l->nEff; i++) {
        if (graph.matrix[idx_sumber][i] == 1) {
            if (current_linked_count >= MAX_WEB_PAGES) {
                printf("[PERINGATAN: Tautan sisa disembunyikan karena memori penuh]\n");
                break;
            }

            printf("[%d] %s\n", nomor_urut, l->HW[i].web_url);

            current_linked_ids[nomor_urut - 1] = l->HW[i].id;
            current_linked_count++;
            nomor_urut++;
        }
    }

    if (current_linked_count == 0) {
        printf("(Halaman ini tidak memiliki tautan keluar)\n");
    }
}

void command_openlinked(ListHalaman_Web *l, int x) {
    if (current_active_page_id == -1) {
        printf("\nERROR: COMMAND HANYA DAPAT DIGUNAKAN SAAT HALAMAN WEB TERBUKA!\n");
        return;
    }

    if (current_linked_count == 0 || x < 1 || x > current_linked_count) {
        printf("\nERROR: HALAMAN TIDAK MEMILIKI TAUTAN YANG BISA DIBUKA!\n");
        
		int i;
        for (i = 0; i < l->nEff; i++) {
            if (l->HW[i].id == current_active_page_id) {
                printf("\nMenampilkan kembali halaman aktif: %s\n", l->HW[i].web_url);
                break;
            }
        }
        return;
    }

    int target_id = current_linked_ids[x - 1];
    char *target_url = NULL;
    int i;
    for (i = 0; i < l->nEff; i++) {
        if (l->HW[i].id == target_id) {
            target_url = l->HW[i].web_url;
            break;
        }
    }

    if (target_url != NULL) {
        open_page(l, target_url);
    }
}

