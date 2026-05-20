#include "adj_matrix.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void adj_init(AdjMatrix *A, int max_pages) {
    A->max = max_pages;
    A->mat = (int **)malloc(max_pages * sizeof(int *));
    if (!A->mat) { fprintf(stderr, "Error: malloc AdjMatrix rows\n"); exit(1); }
    int i;
	for (i = 0; i < max_pages; i++) {
        A->mat[i] = (int *)calloc(max_pages, sizeof(int));
        if (!A->mat[i]) { fprintf(stderr, "Error: calloc AdjMatrix col %d\n", i); exit(1); }
    }
}

void adj_free(AdjMatrix *A) {
    int i;
	for (i = 0; i < A->max; i++) free(A->mat[i]);
    free(A->mat);
    A->mat = NULL;
}

void adj_clear(AdjMatrix *A) {
    int i;
	for (i = 0; i < A->max; i++)
        memset(A->mat[i], 0, A->max * sizeof(int));
}

void adj_add_edge(AdjMatrix *A, int src_idx, int dst_idx) {
    if (src_idx < 0 || src_idx >= A->max) return;
    if (dst_idx < 0 || dst_idx >= A->max) return;
    A->mat[src_idx][dst_idx] = 1;
}

void adj_remove_node(AdjMatrix *A, int page_idx, int total_pages) {
    if (page_idx < 0 || page_idx >= A->max) return;
    int j;
	for (j = 0; j < total_pages; j++) {
        A->mat[page_idx][j] = 0;
        A->mat[j][page_idx] = 0;
    }
}

// Bangun ulang matriks dari data WebPageSet dan LinkedPageSet. Indeks matriks = posisi dalam WebPageSet (bukan ID halaman). //
void adj_build(AdjMatrix *A, const WebPageSet *pages, const LinkedPageSet *linked) {
    adj_clear(A);
    int k;
	for (k = 0; k < linked->size; k++) {
        int src_id = linked->data[k].id_sumber;
        int dst_id = linked->data[k].id_tujuan;

        // Cari indeks posisi dalam array WebPageSet //
        int src_idx = -1, dst_idx = -1;
        int i;
		for (i = 0; i < pages->size; i++) {
            if (pages->data[i].id == src_id) src_idx = i;
            if (pages->data[i].id == dst_id) dst_idx = i;
        }
        if (src_idx != -1 && dst_idx != -1)
            adj_add_edge(A, src_idx, dst_idx);
    }
}

// Dapatkan daftar indeks tujuan dari src_idx. Return: jumlah neighbor yang ditemukan.//
int adj_get_neighbors(const AdjMatrix *A, int src_idx,
                      int *out_dst, int out_max) {
    if (src_idx < 0 || src_idx >= A->max) return 0;
    int count = 0;
    int j;
    for (j = 0; j < A->max && count < out_max; j++) {
        if (A->mat[src_idx][j]) out_dst[count++] = j;
    }
    return count;
}
