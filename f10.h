#ifndef F10_H
#define F10_H

#include "f00.h"
#include "f07.h"

#define DOWNLOAD_MAX_AMOUNT 5
#define MAX_URL_LEN 100

typedef struct {
    char url[MAX_URL_LEN];
    int ticks_sisa;
} DownloadItem;

typedef struct {
    DownloadItem buffer[DOWNLOAD_MAX_AMOUNT];
    int head; 
    int tail; 
    int size; 
} DownloadQueue;

/* Fungsi Utama Download Manager */
void InitDownloadManager(DownloadQueue *q);
void TambahDownload(DownloadQueue *q, char *urlTarget);
void JalankanTick(DownloadQueue *q, ListHalaman_Web *database);

/**
 * EvaluasiPerintahDownload
 * Jembatan modular pencegat terminal yang disatukan di dalam modul F10.
 * Mengembalikan 1 jika perintah dikenali, atau 0 jika bukan urusan F10.
 */
int EvaluasiPerintahDownload(char *perintahUtama, char *argumenTambahan, ListHalaman_Web *database);

#endif
