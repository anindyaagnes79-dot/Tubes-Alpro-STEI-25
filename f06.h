#ifndef TABS_H
#define TABS_H

#include <stdio.h>
#include <string.h>

#define TAB_MAX_AMOUNT 10
#define TAB_NAME_LEN 100
#define TAB_NULL -1

typedef struct
{
    char name[TAB_NAME_LEN];
    int prev;
    int next;
    int used;
} TabNode;

typedef struct
{
    TabNode nodes[TAB_MAX_AMOUNT];
    int head;
    int tail;
    int current;
    int size;
    int counter;
} TabList;

static int slot_find(TabList *T) {
	int i;
    for (i=0; i<TAB_MAX_AMOUNT; i++) {
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

    T->nodes[idx].prev = T->nodes[idx].next = TAB_NULL;
    T->size--;
}

void f06_init(TabList *T) {
	int i;
    for (i=0; i<TAB_MAX_AMOUNT; i++) {
        T->nodes[i].used = 0;
        T->nodes[i].prev = T->nodes[i].next = TAB_NULL;
        T->nodes[i].name[0] = '\0';
    }

    T->head = T->tail = T->current = TAB_NULL;
    T->size = 0;
    T->counter = 0;

    T->counter++;
    int slot = slot_find(T);
    snprintf(T->nodes[slot].name, TAB_NAME_LEN, "TAB%d", T->counter);
    T->nodes[slot].used = 1;
    list_push(T, slot);
    T->current = slot;
}

int f06_newtab(TabList *T) {
    if (T->size >= TAB_MAX_AMOUNT) {
        printf("ERROR!");
        return 0;
    }

    int slot = slot_find(T);
    T->counter++;
    snprintf(T->nodes[slot].name, TAB_NAME_LEN, "TAB%d", T->counter);
    T->nodes[slot].used = 1;
    list_push(T, slot);

    printf("Tab baru (%s) berhasil dibuat!\n\n", T->nodes[slot].name);
    return 1;
}

int f06_closetab(TabList *T)
{
    if (T->size <= 1) {
        printf("ERROR!");
        return 0;
    }
 
    int closing = T->current;
    int next_current = (T->nodes[closing].next != TAB_NULL)
        ? T->nodes[closing].next : T->nodes[closing].prev;
 
    printf("%s berhasil ditutup.\n\n", T->nodes[closing].name);
 
    list_unlink(T, closing);
    T->nodes[closing].used = 0;         
    T->nodes[closing].name[0] = '\0';
 
    T->current = next_current;
    return 1;
}

void f06_checktab(const TabList *T)
{
    printf("List of tab(s):\n");
    int idx = T->head;
    int pos = 1;
    while (idx != TAB_NULL) {
        printf("  [%d] %s\n", pos++, T->nodes[idx].name);
        idx = T->nodes[idx].next;
    }
    printf("Current tab: %s\n\n", T->nodes[T->current].name);
}

int f06_nexttab(TabList *T, int step)
{
    if (step <= 0) {
        printf("ERROR!");
        return 0;
    }
 
    int target = T->current;
    int i;
    for (i = 0; i < step; i++) {
        if (T->nodes[target].next == TAB_NULL) {
            printf("ERROR!");
            return 0;
        }
        target = T->nodes[target].next;
    }
 
    T->current = target;
    printf("Tab saat ini berhasil diganti ke %s.\n\n", T->nodes[T->current].name);
    return 1;
}

int f06_prevtab(TabList *T, int step)
{
    if (step <= 0) {
        printf("ERROR!");
        return 0;
    }
 
    int target = T->current;
    int i;
    for (i = 0; i < step; i++) {
        if (T->nodes[target].prev == TAB_NULL) {
            printf("ERROR!");
            return 0;
        }
        target = T->nodes[target].prev;
    }
 
    T->current = target;
    printf("Tab saat ini berhasil diganti ke %s.\n\n", T->nodes[T->current].name);
    return 1;
}

#endif 
