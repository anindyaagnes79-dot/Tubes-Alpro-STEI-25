#include "f08.h"
#include <stdio.h>
#include <string.h>

void f08_history(HistoryTab *h, int X) {
    if (IsEmptyStack(h->backStack)) {
        printf("Riwayat kosong untuk tab ini.\n\n");
        return;
    }

    if (X <= 0) {
        printf("ERROR: Jumlah riwayat yang ingin dilihat harus lebih dari 0!\n\n");
        return;
    }

    StackHistory tempStack;
    CreateEmptyStack(&tempStack);
    char temp_url[MAX_URL_LEN];
    int count = 0;

    while (!IsEmptyStack(h->backStack) && count < X) {
        Pop(&h->backStack, temp_url);
        Push(&tempStack, temp_url);
        count++;
    }

    printf("Riwayat URL Tab saat ini:\n");
    int print_idx = 1;

    while (!IsEmptyStack(tempStack)) {
        Pop(&tempStack, temp_url);
  
        if (IsEmptyStack(tempStack)) {
            printf("  [%d] %s <- YOU ARE HERE\n", print_idx, temp_url);
        } else {
            printf("  [%d] %s\n", print_idx, temp_url);
        }

        Push(&h->backStack, temp_url);
        print_idx++;
    }
    printf("\n");
}

int f08_back_x(HistoryTab *h, int x) {
    if (x <= 0) {
        printf("ERROR: Langkah mundur (X) harus bilangan bulat positif!\n\n");
        return 0;
    }

    if (h->backStack.top < x) {
        printf("ERROR: Langkah mundur terlalu jauh! (Maksimal %d langkah)\n\n", h->backStack.top);
        return 0;
    }

    char temp_url[MAX_URL_LEN];
	
	int i;
    for (i = 0; i < x; i++) {
        Pop(&h->backStack, temp_url);
        Push(&h->forwardStack, temp_url);
    }


    printf("BACK %d: Mundur ke halaman %s\n\n", x, h->backStack.data[h->backStack.top]);
    return 1;
}

int f08_forward_x(HistoryTab *h, int x) {
    if (x <= 0) {
        printf("ERROR: Langkah maju (X) harus bilangan bulat positif!\n\n");
        return 0;
    }
    if (h->forwardStack.top + 1 < x) {
        printf("ERROR: Langkah maju terlalu jauh! (Maksimal %d langkah)\n\n", h->forwardStack.top + 1);
        return 0;
    }

    char temp_url[MAX_URL_LEN];
	
	int i;
    for (i = 0; i < x; i++) {
        Pop(&h->forwardStack, temp_url);
        Push(&h->backStack, temp_url);
    }

    printf("FORWARD %d: Maju ke halaman %s\n\n", x, h->backStack.data[h->backStack.top]);
    return 1;
}
