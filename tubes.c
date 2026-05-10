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

/* Konstruktor : create list Halaman Web kosong */
void BuatListHalaman_Web(ListHalaman_Web *l){
    l->nEff=0;
}

/* Mengecek list kosong
 * Mengirimkan true jika list kosong, mengirimkan false jika tidak */
bool isEmpty(ListHalaman_Web l){
    return (l.nEff==0);
}
/* Mengecek list penuh */
/* Mengirimkan true jika list penuh, mengirimkan false jika tidak */
bool isFull(ListHalaman_Web w){
    return (w.nEff==MAX_capacity);
}

bool isIndexEff(ListHalaman_Web w, int i){
    return (i>=0 && i< w.nEff);
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

unsigned int angka;

unsigned int Acak(){
    int A=1103515245;
    int C=12345;
    int M=2147483648;
    angka=(A*angka+C)%M;
    return angka;
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
            int index=Acak()%l->nEff;
            if (sudah_tampil[index]==false) {
                printf("- %s,", l->HW[index].web_url);
                sudah_tampil[index] = true;
                banyak_url_tampil++;
            }
        }
    }
}

int main(){
angka=time(NULL);

Halaman_Web h1,h2,h3,h4,h5,h6,h7;
ListHalaman_Web w;
BuatHalaman_Web(&h1,123,"agdga.com","ahahah");
BuatHalaman_Web(&h2,124,"rjghjrhjg.com","aef");
BuatHalaman_Web(&h3,125,"smdmsmds.com","ahaefh");
BuatHalaman_Web(&h4,126,"asjdjsdj.com","aefahah");
BuatHalaman_Web(&h5,127,"aejufeu.com","ahahefah");
BuatHalaman_Web(&h6,128,"mefmlemfle.com","ahasshah");
BuatHalaman_Web(&h7,129,"mefmlefejhfle.com","aefkeshah");
BuatListHalaman_Web(&w);
TambahHalamanWeb(&w,h1);
TambahHalamanWeb(&w,h2);
TambahHalamanWeb(&w,h3);
TambahHalamanWeb(&w,h4);
TambahHalamanWeb(&w,h5);
TambahHalamanWeb(&w,h6);
TambahHalamanWeb(&w,h7);
//printf("%d,",w.HW[0].id);
//printf("%s,",w.HW[0].web_url);
//printf("%s,",w.HW[0].content);

Discover(&w);
printf("\n");

Discover(&w);
printf("\n");

Discover(&w);

return 0;
}


