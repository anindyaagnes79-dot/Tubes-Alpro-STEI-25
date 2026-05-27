#ifndef F10_H
#define F10_H

#include "f00.h"

#define DOWNLOAD_MAX_AMOUNT 5
#define MAX_URL_LEN 256

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

void InitDownloadManager(DownloadQueue *q);
void TambahDownload(DownloadQueue *q, char *urlTarget);
void JalankanTick(DownloadQueue *q, ListHalaman_Web *database);

#endif
