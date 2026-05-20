#ifndef ADJ_MATRIX_H
#define ADJ_MATRIX_H

#include "web_pages.h"

typedef struct {
    int **mat;        // matriks 2D dinamis //
    int   max;        // MAX_WEB_PAGES      //
} AdjMatrix;

void adj_init(AdjMatrix *A, int max_pages);
void adj_free(AdjMatrix *A);
void adj_clear(AdjMatrix *A);

// Tambah edge dari indeks src ke dst //
void adj_add_edge(AdjMatrix *A, int src_idx, int dst_idx);
// Hapus semua edge yang melibatkan indeks page_idx //
void adj_remove_node(AdjMatrix *A, int page_idx, int total_pages);

// Bangun ulang matriks dari WebPageSet + LinkedPageSet //
void adj_build(AdjMatrix *A, const WebPageSet *pages, const LinkedPageSet *linked);

// Ambil daftar tujuan (dst indices) dari indeks src; return jumlah //
int adj_get_neighbors(const AdjMatrix *A, int src_idx,
                      int *out_dst, int out_max);

#endif