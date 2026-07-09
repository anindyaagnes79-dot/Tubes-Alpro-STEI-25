#ifndef F07_H
#define F07_H

#include <stdbool.h>

#define MAX_HISTORY 100
#define MAX_URL_LEN 256

typedef struct {
    char data[MAX_HISTORY][MAX_URL_LEN];
    int top; 
} StackHistory;

void CreateEmptyStack(StackHistory *S);
bool IsEmptyStack(StackHistory S);
bool IsFullStack(StackHistory S);
void Push(StackHistory *S, const char *url);
void Pop(StackHistory *S, char *out_url);

typedef struct {
    StackHistory backStack;   
    StackHistory forwardStack; 
} HistoryTab;


void f07_init(HistoryTab *h);
void f07_tambah_url(HistoryTab *h, const char *url);
int f07_back(HistoryTab *h);
int f07_forward(HistoryTab *h);

#endif
