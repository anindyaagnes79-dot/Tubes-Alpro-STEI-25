#include "halaman_web.h"

int main() {
    ListWeb lw;
    Web w;
    char kata[50];

    initList(&lw);

    buatWeb(&w, 1, "google.com", "Search Engine");
    tambahWeb(&lw, w);
    
    buatWeb(&w, 2, "itb.ac.id", "Web Utama");
    tambahWeb(&lw, w);
    
    buatWeb(&w, 3, "six.itb.ac.id", "Sistem Akademik");
    tambahWeb(&lw, w);
    
    buatWeb(&w, 4, "edunex.itb.ac.id", "Kuliah Online");
    tambahWeb(&lw, w);
    
    buatWeb(&w, 5, "github.com", "Coding");
    tambahWeb(&lw, w);

    printf("Program Cari Web Siap.\n");
    printf("Ketik apa saja untuk cari, atau 'EXIT' untuk keluar.\n");

    while (true) {
        printf("\nCari: ");
        if (scanf("%s", kata) == EOF) break;

        if (strcmp(kata, "EXIT") == 0) {
            printf("Selesai.\n");
            break;
        }
        cari(lw, kata);
    }

    return 0;
}