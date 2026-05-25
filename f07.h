#ifndef F07_H
#define F07_H

#include "f03.h"
#include "f04.h"
#include "web_pages.h"
#include "adj_matrix.h"
#include <stdbool.h>

#define MAX_HISTORY 100
typedef struct {
    char data[MAX_HISTORY][MAX_URL_LEN];
    int top; 
} Stack;

void CreateEmptyStack(Stack *S);
bool IsEmptyStack(Stack S);
bool IsFullStack(Stack S);
void Push(Stack *S, const char *url);
void Pop(Stack *S, char *out_url);

typedef struct {
    Stack backStack;   
    Stack forwardStack; 
} HistoryTab;

void f07_init(HistoryTab *h);
void f07_tambah_url(HistoryTab *h, const char *url);
int f07_back(HistoryTab *h, WebPageSet *pages, CacheMap *cache, AdjMatrix *adj);
int f07_forward(HistoryTab *h, WebPageSet *pages, CacheMap *cache, AdjMatrix *adj);

#endif
