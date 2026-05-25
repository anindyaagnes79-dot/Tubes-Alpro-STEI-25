#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX_capacity 100

typedef struct {
    int id;
    char* web_url;
    char* content;
} Halaman_Web;

typedef struct {
    Halaman_Web HW[MAX_capacity];
    int nEff;
} ListHalaman_Web;

void BuatHalaman_Web(Halaman_Web *h, int id, char* web_url, char* content);
void BuatListHalaman_Web(ListHalaman_Web *l);
bool isEmpty(ListHalaman_Web l);
bool isFull(ListHalaman_Web l);
bool isIndexEff(ListHalaman_Web l, int i);
int IndexTerbesar(ListHalaman_Web l);
void TambahHalamanWeb(ListHalaman_Web *l, Halaman_Web h);
unsigned int AcakLCG()
void Discover(ListHalaman_Web *l)