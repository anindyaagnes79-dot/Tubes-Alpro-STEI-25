#include "halaman_web.h"

void initList(ListWeb *l) {
    l->jumlah = 0;
}

void buatWeb(Web *w, int id, char* url, char* isi) {
    w->id = id;
    w->url = url;
    w->isi = isi;
}

void tambahWeb(ListWeb *l, Web w) {
    if (l->jumlah < MAX) {
        l->daftar[l->jumlah] = w;
        l->jumlah++;
    }
}

void cari(ListWeb l, char* query) {
    int i, j;
    bool ketemu = false;
    int len_q = 0;
    while (query[len_q] != '\0') {
        len_q++;
    }

    printf("Hasil cari '%s':\n", query);

    for (i = 0; i < l.jumlah; i++) {
        bool sama = true;
        for (j = 0; j < len_q; j++) {
            if (l.daftar[i].url[j] != query[j]) {
                sama = false;
                break; 
            }
        }
        if (sama) {
            printf("- %s\n", l.daftar[i].url);
            ketemu = true;
        }
    }
    if (!ketemu) {
        printf("Gak ketemu.\n");
    }
}