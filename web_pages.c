#include "web_pages.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implementasi ADT Set WebPage + LinkedPage + CSV I/O //

// Konversi newline //

// Ganti \n literal (dua karakter '\','n') → newline asli '\n' //
void unescape_newlines(char *s) {
    char tmp[MAX_CONTENT_LEN];
    int i = 0, j = 0;
    while (s[i] != '\0' && j < MAX_CONTENT_LEN - 1) {
        if (s[i] == '\\' && s[i + 1] == 'n') {
            tmp[j++] = '\n';
            i += 2;
        } else {
            tmp[j++] = s[i++];
        }
    }
    tmp[j] = '\0';
    strcpy(s, tmp);
}

// Ganti newline asli '\n' → \n literal (dua karakter) untuk CSV //
void escape_newlines(char *dst, const char *src, int dst_size) {
    int i = 0, j = 0;
    while (src[i] != '\0' && j < dst_size - 3) {
        if (src[i] == '\n') {
            dst[j++] = '\\';
            dst[j++] = 'n';
        } else if (src[i] == '"') {
            // Escape tanda kutip ganda dengan doubled quote //
            dst[j++] = '"';
            dst[j++] = '"';
        } else {
            dst[j++] = src[i];
        }
        i++;
    }
    dst[j] = '\0';
}

// ADT Set WebPage //

void wpset_init(WebPageSet *S, int max_pages) {
    S->max_pages = max_pages;
    S->size      = 0;
    S->capacity  = (max_pages < 16) ? max_pages : 16;
    S->data      = (WebPage *)malloc(S->capacity * sizeof(WebPage));
    if (!S->data) { fprintf(stderr, "Error: malloc WebPageSet\n"); exit(1); }
}

void wpset_free(WebPageSet *S) {
    free(S->data);
    S->data = NULL;
    S->size = S->capacity = 0;
}

// Tambah page. Return 1 sukses, 0 jika URL sudah ada, -1 jika melebihi MAX_WEB_PAGES //
int wpset_add(WebPageSet *S, const WebPage *page) {
    if (wpset_contains_url(S, page->web_url)) return 0;
    if (S->size >= S->max_pages) return -1;  // D01: tolak jika melebihi batas //

    if (S->size == S->capacity) {
        int new_cap = S->capacity * 2;
        if (new_cap > S->max_pages) new_cap = S->max_pages;
        S->data = (WebPage *)realloc(S->data, new_cap * sizeof(WebPage));
        if (!S->data) { fprintf(stderr, "Error: realloc WebPageSet\n"); exit(1); }
        S->capacity = new_cap;
    }
    S->data[S->size++] = *page;
    return 1;
}

// Hapus page berdasarkan URL. Return 1 sukses, 0 tidak ditemukan //
int wpset_remove_by_url(WebPageSet *S, const char *url) {
    for (int i = 0; i < S->size; i++) {
        if (strcmp(S->data[i].web_url, url) == 0) {
            // Geser elemen ke kiri //
            for (int j = i; j < S->size - 1; j++)
                S->data[j] = S->data[j + 1];
            S->size--;
            return 1;
        }
    }
    return 0;
}

// Linear search berdasarkan URL; return pointer atau NULL //
WebPage *wpset_find_by_url(WebPageSet *S, const char *url) {
    for (int i = 0; i < S->size; i++)
        if (strcmp(S->data[i].web_url, url) == 0) return &S->data[i];
    return NULL;
}

// Linear search berdasarkan ID; return pointer atau NULL //
WebPage *wpset_find_by_id(WebPageSet *S, int id) {
    for (int i = 0; i < S->size; i++)
        if (S->data[i].id == id) return &S->data[i];
    return NULL;
}

int wpset_contains_url(const WebPageSet *S, const char *url) {
    for (int i = 0; i < S->size; i++)
        if (strcmp(S->data[i].web_url, url) == 0) return 1;
    return 0;
}

int wpset_max_id(const WebPageSet *S) {
    int max = 0;
    for (int i = 0; i < S->size; i++)
        if (S->data[i].id > max) max = S->data[i].id;
    return max;
}

// ADT Set LinkedPage //

void lpset_init(LinkedPageSet *S) {
    S->capacity = 16;
    S->size     = 0;
    S->data     = (LinkedPage *)malloc(S->capacity * sizeof(LinkedPage));
    if (!S->data) { fprintf(stderr, "Error: malloc LinkedPageSet\n"); exit(1); }
}

void lpset_free(LinkedPageSet *S) {
    free(S->data);
    S->data = NULL;
    S->size = S->capacity = 0;
}

void lpset_add(LinkedPageSet *S, const LinkedPage *lp) {
    if (S->size == S->capacity) {
        S->capacity *= 2;
        S->data = (LinkedPage *)realloc(S->data, S->capacity * sizeof(LinkedPage));
        if (!S->data) { fprintf(stderr, "Error: realloc LinkedPageSet\n"); exit(1); }
    }
    S->data[S->size++] = *lp;
}

// Hapus semua relasi yang sumbernya adalah id_sumber (untuk delete_page) //
void lpset_remove_by_src(LinkedPageSet *S, int id_sumber) {
    int j = 0;
    for (int i = 0; i < S->size; i++)
        if (S->data[i].id_sumber != id_sumber)
            S->data[j++] = S->data[i];
    S->size = j;
}

// Hapus semua relasi yang tujuannya adalah id_tujuan (dead links) //
void lpset_remove_by_dst(LinkedPageSet *S, int id_tujuan) {
    int j = 0;
    for (int i = 0; i < S->size; i++)
        if (S->data[i].id_tujuan != id_tujuan)
            S->data[j++] = S->data[i];
    S->size = j;
}

int lpset_max_id(const LinkedPageSet *S) {
    int max = 0;
    for (int i = 0; i < S->size; i++)
        if (S->data[i].id > max) max = S->data[i].id;
    return max;
}

// CSV Parse * Format web_pages.csv: id,web_url,"content dengan \\n literal" Format linked_pages.csv: id,id_sumber,id_tujuan //

static int parse_web_page_line(const char *line, WebPage *out) {
    const char *p = line;
    char buf[16];
    int  k;

    // id //
    k = 0;
    while (*p && *p != ',' && k < 15) buf[k++] = *p++;
    buf[k] = '\0';
    if (*p != ',') return 0;
    p++;
    out->id = atoi(buf);

    // web_url – boleh ada atau tidak ada quote //
    k = 0;
    if (*p == '"') {
        p++;
        while (*p && *p != '"' && k < MAX_URL_LEN - 1) out->web_url[k++] = *p++;
        if (*p == '"') p++;
    } else {
        while (*p && *p != ',' && k < MAX_URL_LEN - 1) out->web_url[k++] = *p++;
    }
    out->web_url[k] = '\0';
    if (*p != ',') return 0;
    p++;

    // content – wajib diapit " //
    if (*p != '"') return 0;
    p++;
    k = 0;
    while (*p && k < MAX_CONTENT_LEN - 1) {
        if (*p == '"' && *(p + 1) == '"') { // doubled quote //
            out->content[k++] = '"';
            p += 2;
        } else if (*p == '"') {
            p++;
            break;
        } else {
            out->content[k++] = *p++;
        }
    }
    out->content[k] = '\0';

    unescape_newlines(out->content);
    return 1;
}

static int parse_linked_page_line(const char *line, LinkedPage *out) {
    const char *p = line;
    char buf[16];
    int  k;

    k = 0;
    while (*p && *p != ',' && k < 15) buf[k++] = *p++;
    buf[k] = '\0';
    if (*p != ',') return 0;
    p++;
    out->id = atoi(buf);

    k = 0;
    while (*p && *p != ',' && k < 15) buf[k++] = *p++;
    buf[k] = '\0';
    if (*p != ',') return 0;
    p++;
    out->id_sumber = atoi(buf);

    k = 0;
    while (*p && *p != '\n' && *p != '\r' && *p != '\0' && k < 15) buf[k++] = *p++;
    buf[k] = '\0';
    out->id_tujuan = atoi(buf);
    return 1;
}

// Load //

void load_web_pages(WebPageSet *S, const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) return;

    // Buffer besar karena content bisa panjang //
    char line[MAX_CONTENT_LEN + 512];

    // Lewati header //
    if (!fgets(line, sizeof(line), fp)) { fclose(fp); return; }

    while (fgets(line, sizeof(line), fp)) {
        int len = (int)strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) line[--len] = '\0';
        if (len == 0) continue;

        WebPage pg;
        if (parse_web_page_line(line, &pg)) {
            int r = wpset_add(S, &pg);
            if (r == -1)
                fprintf(stderr, "Peringatan: Halaman '%s' diabaikan, melebihi MAX_WEB_PAGES=%d\n",
                        pg.web_url, S->max_pages);
        }
    }
    fclose(fp);
}

void load_linked_pages(LinkedPageSet *S, const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) return;

    char line[256];
    if (!fgets(line, sizeof(line), fp)) { fclose(fp); return; }

    while (fgets(line, sizeof(line), fp)) {
        int len = (int)strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) line[--len] = '\0';
        if (len == 0) continue;

        LinkedPage lp;
        if (parse_linked_page_line(line, &lp)) lpset_add(S, &lp);
    }
    fclose(fp);
}

// Save //

void save_web_pages(const WebPageSet *S, const char *filepath) {
    FILE *fp = fopen(filepath, "w");
    if (!fp) { fprintf(stderr, "Error: tidak bisa membuka %s untuk ditulis\n", filepath); return; }

    fprintf(fp, "id,web_url,content\n");
    char esc[MAX_CONTENT_LEN * 2];
    for (int i = 0; i < S->size; i++) {
        escape_newlines(esc, S->data[i].content, sizeof(esc));
        fprintf(fp, "%d,\"%s\",\"%s\"\n", S->data[i].id, S->data[i].web_url, esc);
    }
    fclose(fp);
}

void save_linked_pages(const LinkedPageSet *S, const char *filepath) {
    FILE *fp = fopen(filepath, "w");
    if (!fp) { fprintf(stderr, "Error: tidak bisa membuka %s untuk ditulis\n", filepath); return; }

    fprintf(fp, "id,id_sumber,id_tujuan\n");
    for (int i = 0; i < S->size; i++)
        fprintf(fp, "%d,%d,%d\n", S->data[i].id, S->data[i].id_sumber, S->data[i].id_tujuan);
    fclose(fp);
}