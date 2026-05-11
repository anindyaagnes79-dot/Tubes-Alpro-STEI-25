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

    unsigned long long A=18738713713;
    unsigned long long C=16525;
    unsigned long long M=10987654321;
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


void add_page(ListHalaman_Web *l, char* url){
    bool ada=false;
    char konten[1024];
    char baris[1024];
    for (int i=0; i<l->nEff;i++){
        if (strcmp(l->HW[i].web_url,url)==0){
            printf("Sudah terdapat halaman dengan url %s. Gunakan url lain yang belum terdaftar!\n", url);
            ada=true;
            break;
        }
    }
    if (isFull(*l)){
        printf ("Gagal : KAPASITAS PENUH\n");
    }
    else if (ada==false && isFull(*l)==false){
        Halaman_Web w;
        printf("Masukkan isi konten:\n");
        while (true) {
            scanf("%s", baris);
            if (strcmp(baris, ".") == 0) {
                break;
            }
            if (strlen(konten) > 0) {
                strcat(konten, " ");
            }
            strcat(konten, baris);
        }
        int id_baru=IndexTerbesar(*l)+1;
        BuatHalaman_Web(&w,id_baru,strdup(url),strdup(konten));
        TambahHalamanWeb(l,w);
    }

}

int main(){

Halaman_Web h1,h2,h3,h4,h5,h6,h7;
ListHalaman_Web w;
BuatHalaman_Web(&h1,123,"Anjay.com","ahahah");
BuatHalaman_Web(&h2,124,"Kebo.com","aef");
BuatHalaman_Web(&h3,125,"Sama.com","ahaefh");
BuatHalaman_Web(&h4,126,"Kobi.com","aefahah");
BuatHalaman_Web(&h5,127,"Tenang.com","ahahefah");
BuatHalaman_Web(&h6,128,"Gila.com","ahasshah");
BuatHalaman_Web(&h7,129,"Alprog.com","aefkeshah");
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
Discover(&w);
printf("\n");
Discover(&w);
printf("\n");
Discover(&w);
printf("\n");

add_page(&w,"Alprog123.com");
printf("%s,",w.HW[7].content);

return 0;
}


