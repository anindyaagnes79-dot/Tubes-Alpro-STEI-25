#ifndef HALAMAN_WEB_H
#define HALAMAN_WEB_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX 100

typedef struct {
    int id;
    char* url;
    char* isi;
} Web;

typedef struct {
    Web daftar[MAX];
    int jumlah;
} ListWeb;

// Daftar fungsi dasar
void initList(ListWeb *l);
void buatWeb(Web *w, int id, char* url, char* isi);
void tambahWeb(ListWeb *l, Web w);
void cari(ListWeb l, char* query);

#endif