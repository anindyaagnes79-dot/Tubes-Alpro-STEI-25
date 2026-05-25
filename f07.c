#include "f07.h"
#include <stdio.h>
#include <string.h>

void CreateEmptyStack(Stack *S) { 
    S->top = -1; 
}
bool IsEmptyStack(Stack S) { 
    return S.top == -1; 
}
bool IsFullStack(Stack S) { 
    return S.top == MAX_HISTORY - 1; 
}
void Push(Stack *S, const char *url) {
    if (!IsFullStack(*S)) {
        S->top++;
        strncpy(S->data[S->top], url, MAX_URL_LEN - 1);
        S->data[S->top][MAX_URL_LEN - 1] = '\0';
    }
}
void Pop(Stack *S, char *out_url) {
    if (!IsEmptyStack(*S)) {
        strcpy(out_url, S->data[S->top]);
        S->top--;
    }
}

void f07_init(HistoryTab *h) {
    CreateEmptyStack(&h->backStack);
    CreateEmptyStack(&h->forwardStack);
}

void f07_tambah_url(HistoryTab *h, const char *url) {
    CreateEmptyStack(&h->forwardStack);

    if (IsFullStack(h->backStack)) {
        printf("Peringatan: History sudah penuh, URL lama dihapus.\n");
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            strcpy(h->backStack.data[i], h->backStack.data[i + 1]);
        }
        h->backStack.top--;
    }
    Push(&h->backStack, url);
}

int f07_back(HistoryTab *h, WebPageSet *pages, CacheMap *cache, AdjMatrix *adj) {
    if (h->backStack.top <= 0) {
        printf("ERROR: BACK TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SEBELUMNYA!\n\n");
        return 0;
    }

    char temp[MAX_URL_LEN];
    Pop(&h->backStack, temp);
    Push(&h->forwardStack, temp); 

    printf("BACK: KEMBALI KE HALAMAN %s\n\n", h->backStack.data[h->backStack.top]);
    return 1;
}

int f07_forward(HistoryTab *h, WebPageSet *pages, CacheMap *cache, AdjMatrix *adj) {
    if (IsEmptyStack(h->forwardStack)) {
        printf("ERROR: FORWARD TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SELANJUTNYA!\n\n");
        return 0;
    }

    char temp[MAX_URL_LEN];
    Pop(&h->forwardStack, temp);
    Push(&h->backStack, temp); 

    printf("FORWARD: MAJU KE HALAMAN %s\n\n", h->backStack.data[h->backStack.top]);
    return 1;
}
