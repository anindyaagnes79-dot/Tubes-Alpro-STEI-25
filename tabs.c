#include "tabs.h"

static int slot_find(TabList *T) {
    for (int i = 0; i < TAB_MAX_AMOUNT; i++) {
        if (!T->nodes[i].used) {
            return i;
        }
    }
    return TAB_NULL;
}

static void list_push(TabList *T, int idx) {
    T->nodes[idx].prev = T->tail;
    T->nodes[idx].next = TAB_NULL;

    if (T->tail != TAB_NULL) {
        T->nodes[T->tail].next = idx;
    } else {
        T->head = idx;
    }

    T->tail = idx;
    T->size++;
}

static void list_unlink(TabList *T, int idx) {
    int p = T->nodes[idx].prev;
    int n = T->nodes[idx].next;

    if (p != TAB_NULL) {
        T->nodes[p].next = n;
    } else {
        T->head = n;
    }

    if (n != TAB_NULL) {
        T->nodes[n].prev = p;
    } else {
        T->tail = p;
    }

    T->size--;
}


void f06_init(TabList *T) {
    T->head = TAB_NULL;
    T->tail = TAB_NULL;
    T->current = TAB_NULL;
    T->size = 0;
    T->counter = 0;

    for (int i = 0; i < TAB_MAX_AMOUNT; i++) {
        T->nodes[i].used = 0;
    }

    f06_newtab(T);
}

int f06_newtab(TabList *T) {
    if (T->size >= TAB_MAX_AMOUNT) {
        printf("ERROR: Jumlah tab tidak bisa melebihi batas maksimum!\n\n");
        return 0;
    }

    int idx = slot_find(T);
    if (idx == TAB_NULL) return 0;

    T->counter++;
    sprintf(T->nodes[idx].name, "TAB%d", T->counter);
    T->nodes[idx].used = 1;

    list_push(T, idx);

    T->current = idx;

    printf("Tab baru (%s) berhasil dibuat!\n\n", T->nodes[idx].name);
    return 1;
}

int f06_closetab(TabList *T) {
    if (T->size <= 1) {
        printf("ERROR: Tidak bisa menutup tab, tab minimal berjumlah 1!\n\n");
        return 0;
    }

    int closing = T->current;
    int next_current = T->nodes[closing].next;

    if (next_current == TAB_NULL) {
        next_current = T->nodes[closing].prev;
    }

    printf("%s berhasil ditutup.\n\n", T->nodes[closing].name);

    list_unlink(T, closing);
    T->nodes[closing].used = 0;         
    T->nodes[closing].name[0] = '\0';

    T->current = next_current;
    return 1;
}

void f06_checktab(const TabList *T) {
    printf("List of tab(s):\n");
    int idx = T->head;
    int pos = 1;
    while (idx != TAB_NULL) {
        printf("  [%d] %s\n", pos++, T->nodes[idx].name);
        idx = T->nodes[idx].next;
    }
    printf("Current tab: %s\n\n", T->nodes[T->current].name);
}

int f06_nexttab(TabList *T, int step) {
    if (step <= 0) {
        printf("ERROR: Langkah harus positif!\n\n");
        return 0;
    }

    int target = T->current;
    for (int i = 0; i < step; i++) {
        if (T->nodes[target].next == TAB_NULL) {
            printf("ERROR: Langkah melebihi batas tab di sebelah kanan!\n\n");
            return 0;
        }
        target = T->nodes[target].next;
    }

    T->current = target;
    printf("Tab saat ini berhasil diganti ke %s.\n\n", T->nodes[T->current].name);
    return 1;
}

int f06_prevtab(TabList *T, int step) {
    if (step <= 0) {
        printf("ERROR: Langkah harus positif!\n\n");
        return 0;
    }

    int target = T->current;
    for (int i = 0; i < step; i++) {
        if (T->nodes[target].prev == TAB_NULL) {
            printf("ERROR: Langkah melebihi batas tab di sebelah kiri!\n\n");
            return 0;
        }
        target = T->nodes[target].prev;
    }

    T->current = target;
    printf("Tab saat ini berhasil diganti ke %s.\n\n", T->nodes[T->current].name);
    return 1;
}
