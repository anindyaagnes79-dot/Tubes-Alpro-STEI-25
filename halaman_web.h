#ifndef HALAMAN_WEB_H
#define HALAMAN_WEB_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CAPACITY 100

typedef struct {
    int id;
    char* web_url;
    char* content;
} Halaman_Web;

typedef struct {
    Halaman_Web HW[MAX_CAPACITY];
    int nEff;
} ListHalaman_Web;

void BuatList(ListHalaman_Web *l);
void BuatHalaman(Halaman_Web *h, int id, char* url, char* konten);
void TambahHalaman(ListHalaman_Web *l, Halaman_Web h);
void Search(ListHalaman_Web l, char* query);

#endif