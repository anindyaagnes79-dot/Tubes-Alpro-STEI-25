#ifndef TABS_H
#define TABS_H

#include <stdio.h>
#include <string.h>

#define TAB_MAX_AMOUNT 10
#define TAB_NAME_LEN 100
#define TAB_NULL -1


typedef struct {
    char name[TAB_NAME_LEN];
    int prev;
    int next;
    int used;
} TabNode;


typedef struct {
    TabNode nodes[TAB_MAX_AMOUNT];
    int head;
    int tail;
    int current;
    int size;
    int counter;
} TabList;

void f06_init(TabList *T);
int f06_newtab(TabList *T);
int f06_closetab(TabList *T);
void f06_checktab(const TabList *T);
int f06_nexttab(TabList *T, int step);
int f06_prevtab(TabList *T, int step);

#endif
