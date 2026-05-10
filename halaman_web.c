#include "halaman_web.h"

void BuatList(ListHalaman_Web *l) {
    l->nEff = 0;
}

void BuatHalaman(Halaman_Web *h, int id, char* url, char* konten) {
    h->id = id;
    h->web_url = url;
    h->content = konten;
}

void TambahHalaman(ListHalaman_Web *l, Halaman_Web h) {
    if (l->nEff < MAX_CAPACITY) {
        l->HW[l->nEff] = h;
        l->nEff++;
    }
}

void Search(ListHalaman_Web l, char* query) {
    int panjang_query = strlen(query);
    bool found = false;

    printf("Search result(s) for \"%s\":\n", query);
    for (int i = 0; i < l.nEff; i++) {
        // Prefix matching menggunakan strncmp
        if (strncmp(l.HW[i].web_url, query, panjang_query) == 0) {
            printf("- %s\n", l.HW[i].web_url);
            found = true;
        }
    }
    if (!found) {
        printf("Tidak Ditemukan\n");
    }
}