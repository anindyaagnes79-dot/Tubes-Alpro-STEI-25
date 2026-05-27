#ifndef F00
#define F00

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define IDX_UNDEF -1
#define MAX_WEB_PAGES 100

typedef struct {
    int id;
    char *web_url;
    char *content;
} Halaman_Web;

typedef struct {
    Halaman_Web HW[MAX_WEB_PAGES]; 
    int nEff;
    int kapasitas;
} ListHalaman_Web;

static void BuatHalaman_Web(Halaman_Web *h, int id, char* web_url, char* content){
    h->id = id;
    h->web_url = (char*)malloc((strlen(web_url) + 1) * sizeof(char));
    if (h->web_url != NULL) {
        strcpy(h->web_url, web_url);
    }

    h->content = (char*)malloc((strlen(content) + 1) * sizeof(char));
    if (h->content != NULL) {
        strcpy(h->content, content);
    }
}

static void BuatListHalaman_Web(ListHalaman_Web *l){
    l->nEff = 0;
    l->kapasitas = MAX_WEB_PAGES;
}

static bool isEmpty(ListHalaman_Web l){
    return (l.nEff == 0);
}

static bool isFull(ListHalaman_Web l){
    return (l.nEff == l.kapasitas);
}

static void TambahHalamanWeb(ListHalaman_Web *l, Halaman_Web h){
    if (isFull(*l)) {
        printf("GAGAL: Kapasitas list sudah penuh!\n");
        return;
    }
    l->HW[l->nEff] = h;
    l->nEff++;
}

static void DealokasiList(ListHalaman_Web *l) {
	int i;
    for (i = 0; i < l->nEff; i++) {
        if (l->HW[i].web_url != NULL) {
            free(l->HW[i].web_url);
            l->HW[i].web_url = NULL;
        }
        if (l->HW[i].content != NULL) {
            free(l->HW[i].content);
            l->HW[i].content = NULL;
        }
    }

    l->nEff = 0;
    l->kapasitas = 0;
}

static int IDTerbesar(ListHalaman_Web l){
    if (isEmpty(l) == true){
        return 0;
    }
    int max_id = l.HW[0].id;
    int i;
    for (i = 1; i < l.nEff; i++) {
        if (l.HW[i].id > max_id) {
            max_id = l.HW[i].id;
        }
    }
    return max_id;
}

static int idx_url_database(ListHalaman_Web *l, char* url) {
	int i;
    for (i = 0; i < l->nEff; i++) {
        if (strcmp(l->HW[i].web_url, url) == 0) {
            return i;
        }
    }
    return IDX_UNDEF;
}

static bool url_ada_database(ListHalaman_Web *l, char* url) {
    return idx_url_database(l, url) != IDX_UNDEF;
}

#endif
