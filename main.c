#include "halaman_web.h"

int main() {
    ListHalaman_Web list;
    Halaman_Web h;
    char input[50];

    // 1. Inisialisasi List di RAM
    BuatList(&list);

    // 2. Memasukkan 5 Data Halaman Web sesuai permintaan
    BuatHalaman(&h, 1, "google.com", "Search Engine");
    TambahHalaman(&list, h);
    
    BuatHalaman(&h, 2, "itb.ac.id", "Official Website ITB");
    TambahHalaman(&list, h);
    
    BuatHalaman(&h, 3, "six.itb.ac.id", "Sistem Informasi Akademik");
    TambahHalaman(&list, h);
    
    BuatHalaman(&h, 4, "edunex.itb.ac.id", "Learning Management System ITB");
    TambahHalaman(&list, h);
    
    BuatHalaman(&h, 5, "github.com", "Platform Kolaborasi Kode");
    TambahHalaman(&list, h);

    printf("--- ITB Browser Simulator: Ready ---\n");
    printf("Ketik kata kunci untuk mencari atau 'EXIT' untuk keluar.\n");

    // 3. Loop Pencarian Langsung
    while (true) {
        printf("\nCari URL: ");
        
        // Membaca input kata kunci
        if (scanf("%49s", input) == EOF) break;

        // Cek apakah user ingin keluar
        if (strcmp(input, "EXIT") == 0) {
            printf("Program ditutup.\n");
            break;
        }

        // Jalankan fitur F02 - Search
        Search(list, input);
    }

    return 0;
}