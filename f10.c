#include "f10.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void InitDownloadManager(DownloadQueue *q) {
    q->head = 0;
    q->tail = -1;
    q->size = 0;
}

void TambahDownload(DownloadQueue *q, char *urlTarget) {
    if (q->size >= DOWNLOAD_MAX_AMOUNT) {
        printf("Download tidak diterima, antrian sudah penuh.\n\n");
        return;
    }

    q->tail = (q->tail + 1) % DOWNLOAD_MAX_AMOUNT;
    
    strncpy(q->buffer[q->tail].url, urlTarget, MAX_URL_LEN - 1);
    q->buffer[q->tail].url[MAX_URL_LEN - 1] = '\0';

    int panjangUrl = 0;
    while (urlTarget[panjangUrl] != '\0') {
        panjangUrl++;
    }
    int nTicks = (panjangUrl / 5) + 2;
    q->buffer[q->tail].ticks_sisa = nTicks;

    q->size++;

    if (q->size == 1) {
        printf("Download %s (%d ticks)\n\n", urlTarget, nTicks);
    } else {
        printf("Download %s (%d ticks) -> antrian no %d, %d ticks masih tertunda dari antrian sebelumnya\n\n", 
               urlTarget, nTicks, q->size, q->buffer[q->head].ticks_sisa);
    }
}

static void EksekusiSimpanFile(char *urlAsli, ListHalaman_Web *database) {
    char namaFile[MAX_URL_LEN];
    strncpy(namaFile, urlAsli, MAX_URL_LEN - 1);
    namaFile[MAX_URL_LEN - 1] = '\0';

    char *titik = strrchr(namaFile, '.');
    if (titik != NULL) {
        *titik = '\0'; 
    }
    strcat(namaFile, ".txt"); 

    const char *kontenWeb = "Data konten web kosong.";
    int i;
    for (i = 0; i < database->nEff; i++) {
        if (strcmp(database->HW[i].web_url, urlAsli) == 0) {
            kontenWeb = database->HW[i].content;
        }
    }

    FILE *fileLokal = fopen(namaFile, "w");
    if (fileLokal != NULL) {
        fprintf(fileLokal, "%s", kontenWeb);
        fclose(fileLokal);
    }

    printf("%s selesai didownload, ke file %s.\n", urlAsli, namaFile);
}

void JalankanTick(DownloadQueue *q, ListHalaman_Web *database) {
    if (q->size == 0) {
        printf("Antrian download saat ini kosong.\n\n");
        return;
    }

    int idxHead = q->head;
    q->buffer[idxHead].ticks_sisa--;

    if (q->buffer[idxHead].ticks_sisa > 0) {
        printf("Downloading %s... (%d ticks tersisa)\n\n", 
               q->buffer[idxHead].url, q->buffer[idxHead].ticks_sisa);
    } 
    else {
        EksekusiSimpanFile(q->buffer[idxHead].url, database);

        q->head = (q->head + 1) % DOWNLOAD_MAX_AMOUNT;
        q->size--;

        if (q->size > 0) {
            printf("Lanjut downloading %s... (%d ticks tersisa)\n\n", 
                   q->buffer[q->head].url, q->buffer[q->head].ticks_sisa);
        } else {
            printf("\n");
        }
    }
}
