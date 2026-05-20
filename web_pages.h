#ifndef WEB_PAGES_H
#define WEB_PAGES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ADT Set – menyimpan daftar WebPage dan LinkedPage //

#define MAX_URL_LEN     256
#define MAX_CONTENT_LEN 8192

// Tipe data satu halaman web //
typedef struct {
    int  id;
    char web_url[MAX_URL_LEN];
    char content[MAX_CONTENT_LEN];
} WebPage;

// Tipe data satu relasi linked page //
typedef struct {
    int id;
    int id_sumber;
    int id_tujuan;
} LinkedPage;

// ADT SET – WebPage (dynamic array) //
typedef struct {
    WebPage *data;
    int      size;       
    int      capacity;   
    int      max_pages;  
} WebPageSet;

void     wpset_init(WebPageSet *S, int max_pages);
void     wpset_free(WebPageSet *S);
int      wpset_add(WebPageSet *S, const WebPage *page);
int      wpset_remove_by_url(WebPageSet *S, const char *url);
WebPage *wpset_find_by_url(WebPageSet *S, const char *url);
WebPage *wpset_find_by_id(WebPageSet *S, int id);
int      wpset_contains_url(const WebPageSet *S, const char *url);
int      wpset_max_id(const WebPageSet *S);

// ADT SET – LinkedPage (dynamic array) //
typedef struct {
    LinkedPage *data;
    int         size;
    int         capacity;
} LinkedPageSet;

void lpset_init(LinkedPageSet *S);
void lpset_free(LinkedPageSet *S);
void lpset_add(LinkedPageSet *S, const LinkedPage *lp);
void lpset_remove_by_src(LinkedPageSet *S, int id_sumber);
void lpset_remove_by_dst(LinkedPageSet *S, int id_tujuan);
int  lpset_max_id(const LinkedPageSet *S);

// Utilitas konversi newline //
void unescape_newlines(char *s);
void escape_newlines(char *dst, const char *src, int dst_size);

// Loader & Saver CSV //
void load_web_pages(WebPageSet *S, const char *filepath);
void load_linked_pages(LinkedPageSet *S, const char *filepath);
void save_web_pages(const WebPageSet *S, const char *filepath);
void save_linked_pages(const LinkedPageSet *S, const char *filepath);

#endif