#ifndef WEB_GRAPH_H
#define WEB_GRAPH_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WEB_PAGES 100
#define IDX_UNDEF -1

#include "f00.h"
#include "f03.h"
#include "f04.h"

typedef struct {
    int id;
    int id_sumber;
    int id_tujuan;
} LinkedHalaman;

typedef struct {
    int matrix[MAX_WEB_PAGES][MAX_WEB_PAGES];
    int ukuran;
} WebGraph;
void init_graph();

int dapatkan_index_matriks(ListHalaman_Web *l, int id_halaman);

void add_link_to_graph(ListHalaman_Web *l, LinkedHalaman link);
void open_page(ListHalaman_Web *l, char* url);
           
void command_openlinked(ListHalaman_Web *l, int x);

#endif
