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


void BuatHalaman_Web(Halaman_Web *h, int id, char* web_url, char* content){
    h->id=id;
    h->web_url=web_url;
    h->content=content;
}

void BuatListHalaman_Web(ListHalaman_Web *l){
    l->nEff=0;
}

bool isEmpty(ListHalaman_Web l){
    return (l.nEff==0);
}

bool isFull(ListHalaman_Web l){
    return (l.nEff==MAX_capacity);
}

bool isIndexEff(ListHalaman_Web l, int i){
    return (i>=0 && i< l.nEff);
}

int IndexTerbesar(ListHalaman_Web l){
    if (isEmpty(l)){
        return 0;
    }
    int max=l.HW[0].id;
    for (int i=0; i<l.nEff; i++){
        if (l.HW[i].id>max){
            max=l.HW[i].id;
        }
    }
    return max;
}

void TambahHalamanWeb(ListHalaman_Web *l, Halaman_Web h){
    if (isFull(*l)==true){
        printf ("GAGAL MENAMBAHKAN\n");
    }
    else{
        l->HW[l->nEff]=h;
        l->nEff++;
    }
}

unsigned int AcakLCG(){
    static unsigned long long angka;
    static bool inisialisasi=false;
    if (!inisialisasi){
        angka=(unsigned int)time(NULL);
        inisialisasi=true;
    }

    unsigned long long A=67676767;
    unsigned long long C=16525;
    unsigned long long M=4141414141;
    angka=(A*angka+C)%M;
    return (unsigned int)angka;
}

void Discover(ListHalaman_Web *l){
    if ((l->nEff)<5 && isEmpty(*l)==false){
        for (int i=0; i<l->nEff; i++){
            printf ("- %s", l->HW[i].web_url);
        }
    }
    else if(isEmpty(*l)==true){
        printf ("ERROR");
    }
    else{
        int banyak_url_tampil=0;
        bool sudah_tampil[MAX_capacity] = {false};
        while (banyak_url_tampil<5){
            int index=AcakLCG()%l->nEff;
            if (sudah_tampil[index]==false) {
                printf("- %s,", l->HW[index].web_url);
                sudah_tampil[index] = true;
                banyak_url_tampil++;
            }
        }
    }
}

