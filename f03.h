#ifndef OPEN_PAGE_H
#define OPEN_PAGE_H

#include "f00.h"

#define MAX_URL_LEN 256
#define MAX_CONTENT_LEN 1024

typedef struct {
    int id;
    char web_url[MAX_URL_LEN];
    char content[MAX_CONTENT_LEN];
} WebPage;

typedef struct {
    WebPage *pages;
    int size;
    int capacity;
} WebPageSet;

typedef struct {
    int id_sumber;
    int id_tujuan;
} LinkedPage;

typedef struct {
    LinkedPage *links;
    int size;
    int capacity;
} LinkedPageSet;

void open_page(ListHalaman_Web *l, char* url);

#endif
