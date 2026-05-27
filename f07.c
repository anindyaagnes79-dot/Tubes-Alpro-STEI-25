#include "f07.h"
#include <stdio.h>
#include <string.h>

void CreateEmptyStack(StackHistory *S) {
    S->top = -1;
}

bool IsEmptyStack(StackHistory S) {
    return S.top == -1;
}

bool IsFullStack(StackHistory S) {
    return S.top == MAX_HISTORY - 1;
}

void Push(StackHistory *S, const char *url) {
    if (!IsFullStack(*S)) {
        S->top++;
        strncpy(S->data[S->top], url, MAX_URL_LEN - 1);
        S->data[S->top][MAX_URL_LEN - 1] = '\0';
    } else {
		int i;
        for (i = 0; i < MAX_HISTORY - 1; i++) {
            strcpy(S->data[i], S->data[i + 1]);
        }
        strncpy(S->data[S->top], url, MAX_URL_LEN - 1);
        S->data[S->top][MAX_URL_LEN - 1] = '\0';
    }
}

void Pop(StackHistory *S, char *out_url) {
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

    Push(&h->backStack, url);
}


int f07_back(HistoryTab *h) {
    if (h->backStack.top <= 0) {
        printf("Mundur gagal. Riwayat kosong atau sudah berada pada awal riwayat.\n\n");
        return 0;
    }

    char current_url[MAX_URL_LEN];

    Pop(&h->backStack, current_url);
    Push(&h->forwardStack, current_url);

    printf("Mundur ke halaman: %s\n\n", h->backStack.data[h->backStack.top]);
    return 1;
}

int f07_forward(HistoryTab *h) {
    if (IsEmptyStack(h->forwardStack)) {
        printf("Maju gagal. Riwayat masa depan kosong.\n\n");
        return 0;
    }

    char next_url[MAX_URL_LEN];

    Pop(&h->forwardStack, next_url);
    Push(&h->backStack, next_url);

    printf("Maju ke halaman: %s\n\n", h->backStack.data[h->backStack.top]);
    return 1;
}
