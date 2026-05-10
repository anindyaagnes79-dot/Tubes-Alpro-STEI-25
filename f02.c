#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_capacity 100

// --- DEFINISI ADT ---

typedef struct {
    int id;
    char* web_url;
    char* content;
} Halaman_Web;

typedef struct {
    Halaman_Web HW[MAX_capacity];
    int nEff;
} ListHalaman_Web;

// --- FUNGSI PEMBANTU (KONSTRUKTOR) ---

void BuatListHalaman_Web(ListHalaman_Web *l) {
    l->nEff = 0;
}

void BuatHalaman_Web(Halaman_Web *h, int id, char* web_url, char* content) {
    h->id = id;
    h->web_url = web_url;
    h->content = content;
}

void TambahHalamanWeb(ListHalaman_Web *l, Halaman_Web h) {
    if (l->nEff < MAX_capacity) {
        l->HW[l->nEff] = h;
        l->nEff++;
    } else {
        printf("GAGAL MENAMBAHKAN: List Penuh\n");
    }
}

// --- F02 - SEARCH ---

void Search(ListHalaman_Web l, char* query) {
    bool found = false;
    int query_len = strlen(query);

    printf("Search result(s) for \"%s\":\n", query);

    for (int i = 0; i < l.nEff; i++) {

        if (strncmp(l.HW[i].web_url, query, query_len) == 0) {
            printf("- %s\n", l.HW[i].web_url);
            found = true;
        }
    }

    if (!found) {
        printf("Tidak Ditemukan\n");
    }
}

// --- MAIN FUNCTION (UNTUK TESTING) ---

int main() {
    ListHalaman_Web list;
    Halaman_Web h1, h2, h3, h4;

    // 1. Inisialisasi List
    BuatListHalaman_Web(&list);

    // 2. Membuat Data Contoh (Dummy Data)
    BuatHalaman_Web(&h1, 1, "google.com", "Mesin pencari");
    BuatHalaman_Web(&h2, 2, "github.com", "Tempat simpan kode");
    BuatHalaman_Web(&h3, 3, "gmail.com", "Layanan email");
    BuatHalaman_Web(&h4, 4, "itb.ac.id", "Situs kampus");

    // 3. Memasukkan ke List
    TambahHalamanWeb(&list, h1);
    TambahHalamanWeb(&list, h2);
    TambahHalamanWeb(&list, h3);
    TambahHalamanWeb(&list, h4);

    // 4. Simulasi Input User untuk Search
    char keyword[50];
    printf("Masukkan kata kunci pencarian: ");
    scanf("%s", keyword);

    // 5. Menjalankan Fungsi F02
    Search(list, keyword);

    return 0;
}