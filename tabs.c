#ifndef TABS_H
#define TABS_H

#include <stdio.h>
#include <string.h>

#define MAX_TAB 999999   /* Kapasitas maksimum list */
#define IDX_MIN 0        /* Index tak terdefinisi   */
#define IDX_UNDEF -1     /* Index tak terdefinisi   */
#define ELEM_UNDEF -999  /* Elemen tak terdefinisi  */

typedef int IdxType;
typedef int ElType;

typedef struct {

	/* memori tempat penyimpan elemen (buffer) */
	ElType buffer [MAX_TAB];
    /* id current tab*/
    int currentIdx;
    /* counter TAB */
    int tabCounter;
	/* banyaknya elemen efektif */
	int nEff;

} Tabs;

void CreateTabs(Tabs *T) {
    T->nEff = 1;
    T->currentIdx = 0;
    T->tabCounter = 1;
    T->buffer[0] = 1;
};

void NewTab(Tabs *T) {
    if (T->nEff >= MAX_TAB) {
        printf("ERROR: Jumlah tab tidak bisa melebihi batas maksimum!\n");
        return;
    }

    T->tabCounter++;

    T->buffer[T->nEff] = T->tabCounter;

    printf("Tab baru (TAB%d) berhasil dibuat!\n", T->buffer[T->nEff]);

    T->nEff++;
};

void CloseTab(Tabs *T) {
    if (T->nEff == 1) {
        printf("ERROR: Tidak bisa menutup tab, tab minimal berjumlah 1!\n");
        return;
    }

    printf("TAB%d berhasil ditutup.\n",
           T->buffer[T->currentIdx]);

    /* shift elemennya ya*/
    for (int i = T->currentIdx; i < T->nEff - 1; i++) {
        T->buffer[i] = T->buffer[i + 1];
    }

    T->nEff--;

    if (T->currentIdx >= T->nEff) {
        T->currentIdx = T->nEff - 1;
    }
};

void CheckTab(Tabs T) {
    if (T.nEff == 1) {
        printf("List of tab:\n");
    } else {
        printf("List of tabs:\n");
    }
    for (int i = 0; i < T.nEff; i++) {
        printf("[%d] TAB%d\n", i + 1, T.buffer[i]);
    }
    printf("Current tab: TAB%d\n", T.buffer[T.currentIdx]);
}

void NextTab(Tabs *T, int step) {
    int newPos = T->currentIdx + step;

    if (newPos >= T->nEff) {
        printf("ERROR: Posisi tab tidak valid!\n");
        return;
    }

    T->currentIdx = newPos;

    printf("Tab saat ini berhasil diganti ke TAB%d.\n", T->buffer[T->currentIdx]);
};

void PrevTab(Tabs *T, int step) {
    int newPos = T->currentIdx - step;

    if (newPos < 0) {
        printf("ERROR: Posisi tab tidak valid!\n");
        return;
    }

    T->currentIdx = newPos;

    printf("Tab saat ini berhasil diganti ke TAB%d.\n", T->buffer[T->currentIdx]);
};


int main() {

    Tabs T;

    CreateTabs(&T);

    CheckTab(T);

    NewTab(&T);
    NewTab(&T);

    CheckTab(T);

    NextTab(&T, 2);

    CheckTab(T);

    CloseTab(&T);

    CheckTab(T);

    return 0;
}

#endif 