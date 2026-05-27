#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "f00.h" 
#include "f01.h" 
#include "f02.h" 
#include "f03.h" 
#include "f04.h"
#include "f05.h" 
#include "f06.h" 
#include "f07.h" 
#include "f08.h" 
#include "f09.h"
#include "f10.h" 
#include "f11.h"

HistoryTab riwayatBrowser; 
LRUCache sistemCache;

void ekstrak_argumen(char *input, char *command, char *argumen) {
    int hasil = sscanf(input, "%s %s", command, argumen);
    if (hasil < 2) {
        argumen[0] = '\0';
    }
}

bool url_valid_e01(char *url) {
    if (url == NULL || strlen(url) == 0) return false;
    int len = strlen(url);
    if (url[0] == '-') return false;

    int titik_pertama = -1;
    int titik_terakhir = -1;
    int i;
    for (i = 0; i < len; i++) {
        if (url[i] == '.') {
            if (titik_pertama == -1) titik_pertama = i;
            titik_terakhir = i;
        }
    }
    if (titik_pertama <= 0 || titik_terakhir == len - 1) return false;
    if (url[titik_pertama - 1] == '-') return false;
    if (titik_pertama < 1 || titik_pertama > 63) return false;

    for (i = 0; i < titik_pertama; i++) {
        char c = url[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-')) {
            return false;
        }
    }
    return true;
}

bool numerik_valid_e02(char *str_angka) {
    if (str_angka == NULL || strlen(str_angka) == 0) return false;
    int len = strlen(str_angka);
    if (len > 6) return false;

    int i;
    for (i = 0; i < len; i++) {
        if (str_angka[i] < '0' || str_angka[i] > '9') return false;
    }
    long nilai = atol(str_angka);
    if (nilai <= 0 || nilai > 999999) return false;
    
    return true;
}

int main() {
    ListHalaman_Web database;
    BuatListHalaman_Web(&database); 
    init_graph(); 
    
    TabList tSistem;
    f06_init(&tSistem); 
    
    f07_init(&riwayatBrowser); 
	
    DownloadQueue qDownload;
    InitDownloadManager(&qDownload); 

    cache_init(&sistemCache, CACHE_MAX_AMOUNT); 
    
	MuatDataDariCSV("config/web_pages.csv", &database); 

    MuatGrafDariCSV("config/linked_pages.csv", &database); 
    
    printf("\n====================================================================\n");
    printf(" Berhasil memuat data. Statistik Database Saat Ini (nEff): %d\n", database.nEff);
    printf("====================================================================\n");

    printf("====================================================================\n");
    printf("              SELAMAT DATANG DI SIMULASI WEB BROWSER             \n");
    printf("====================================================================\n");
    printf("| DAFTAR PERINTAH UTAMA BROWSER:                                   |\n");
    printf("+---------------------------------+--------------------------------+\n");
    printf("|  1. discover                    | 11. back                       |\n");
    printf("|  2. cari <query>                | 12. forward                    |\n");
    printf("|  3. add_page <url>              | 13. history <jumlah_tampil>    |\n");
    printf("|  4. edit_page <url>             | 14. back_x <jumlah_langkah>    |\n");
    printf("|  5. delete_page <url>           | 15. forward_x <jumlah_langkah> |\n");
    printf("|  6. open <url>                  | 16. download <url>             |\n");
    printf("|  7. openlinked <nomor>          | 17. tick                       |\n");
    printf("|  8. newtab                      | 18. save <nama_folder>         |\n");
    printf("|  9. closetab                    | 19. exit                       |\n");
    printf("| 10. checktab                    |                                |\n");
    printf("====================================================================\n");

    char input_user[512];
    char command[50];
    char argumen[256];

    while (true) {
        printf("\n>>> ");
        if (fgets(input_user, sizeof(input_user), stdin) == NULL) {
            break;
        }

        input_user[strcspn(input_user, "\n")] = 0;
        if (strlen(input_user) == 0) continue;

        ekstrak_argumen(input_user, command, argumen);

        if (strcmp(command, "open") == 0 || strcmp(command, "add_page") == 0 || 
            strcmp(command, "edit_page") == 0 || strcmp(command, "delete_page") == 0 || 
            strcmp(command, "download") == 0) {
            if (strlen(argumen) > 0 && !url_valid_e01(argumen)) {
                printf("ERROR E01: Format URL '%s' tidak valid menurut aturan sistem!\n", argumen);
                continue; 
            }
        }

        if (strcmp(command, "switchtab") == 0 || strcmp(command, "history") == 0 || 
            strcmp(command, "back_x") == 0 || strcmp(command, "forward_x") == 0) {
            if (strlen(argumen) > 0 && !numerik_valid_e02(argumen)) {
                printf("ERROR E02: Input '%s' harus bilangan bulat positif (Max 6 digit)!\n", argumen);
                continue; 
            }
        }

        if (strcmp(command, "discover") == 0) {
            Discover(&database); 
        }
        else if (strcmp(command, "cari") == 0) {
            if (strlen(argumen) == 0) {
                printf("Gagal: Format harus 'cari <query>'\n");
            } else {
                cari(database, argumen); 
            }
        }
        else if (strcmp(command, "add_page") == 0) {
            if (strlen(argumen) == 0) {
                printf("Gagal: Format harus 'add_page <url>'\n");
            } else {
                add_page(&database, argumen); 
            }
        }
        else if (strcmp(command, "edit_page") == 0) {
            if (strlen(argumen) == 0) {
                printf("Gagal: Format harus 'edit_page <url>'\n");
            } else {
                edit_page(&database, argumen); 
            }
        }
        else if (strcmp(command, "delete_page") == 0) {
            if (strlen(argumen) == 0) {
                printf("Gagal: Format harus 'delete_page <url>'\n");
            } else {
                delete_page(&database, argumen); 
            }
        }
        else if (strcmp(command, "open") == 0) {
            if (strlen(argumen) == 0) {
                printf("Gagal: Format harus 'open <url>'\n");
            } else {
                WebPage *cekCache = cache_get(&sistemCache, argumen);
                
                if (cekCache != NULL) {
                    printf("[Status: Cache-Hit] Sukses menarik data halaman dari memori cache cepat.\n");
                } else {
                    printf("[Status: Cache-Miss] Data tidak ada di cache. Membaca database CSV dan memperbarui memori cache...\n");
                    
                    int idx_db = idx_url_database(&database, argumen);
                    if (idx_db != IDX_UNDEF) {
                        WebPage dataBaru;
                        dataBaru.id = database.HW[idx_db].id;
                        strcpy(dataBaru.web_url, database.HW[idx_db].web_url);
                        strcpy(dataBaru.content, database.HW[idx_db].content);
                        
                        cache_put(&sistemCache, &dataBaru);
                    }
                }
                open_page(&database, argumen); 
                f07_tambah_url(&riwayatBrowser, argumen); 
            }
        }
        else if (strcmp(command, "openlinked") == 0) {
            if (strlen(argumen) == 0) {
                printf("Gagal: Format harus 'openlinked <nomor>'\n");
            } else {
                int nomor = atoi(argumen);
                command_openlinked(&database, nomor); 
            }
        }
        else if (strcmp(command, "newtab") == 0) {
            f06_newtab(&tSistem); 
        }
        else if (strcmp(command, "closetab") == 0) {
            f06_closetab(&tSistem); 
        }
        else if (strcmp(command, "checktab") == 0) {
            f06_checktab(&tSistem); 
        }
        else if (strcmp(command, "switchtab") == 0) {
            int nomorTarget = atoi(argumen);
            int indeksKetemu = -1;
            int i;
            for (i = 0; i < tSistem.size; i++) {
                if (tSistem.nodes[i].used && (i + 1 == nomorTarget)) {
                    indeksKetemu = i;
                    break;
                }
            }
            if (indeksKetemu != -1) {
                int selisih = indeksKetemu - tSistem.current;
                if (selisih > 0) f06_nexttab(&tSistem, selisih); 
                else if (selisih < 0) f06_prevtab(&tSistem, -selisih); 
            } else {
                printf("ERROR: Tab dengan nomor %d tidak ditemukan!\n", nomorTarget);
            }
        }
        else if (strcmp(command, "back") == 0) {
            f07_back(&riwayatBrowser); 
        }
        else if (strcmp(command, "forward") == 0) {
            f07_forward(&riwayatBrowser); 
        }
        else if (strcmp(command, "history") == 0) {
            int jumlahTampil = (strlen(argumen) == 0) ? 10 : atoi(argumen);
            f08_history(&riwayatBrowser, jumlahTampil); 
        }
        else if (strcmp(command, "back_x") == 0) {
            int langkahMundur = atoi(argumen);
            f08_back_x(&riwayatBrowser, langkahMundur); 
        }
        else if (strcmp(command, "forward_x") == 0) {
            int langkahMaju = atoi(argumen);
            f08_forward_x(&riwayatBrowser, langkahMaju); 
        }
        else if (strcmp(command, "download") == 0) {
            TambahDownload(&qDownload, argumen); 
        }
        else if (strcmp(command, "tick") == 0) {
            JalankanTick(&qDownload, &database); 
        }
        else if (strcmp(command, "save") == 0) {
            if (strlen(argumen) == 0) {
                printf("Gagal: Format harus 'save <folder>'\n\n");
            } else {
                EksekusiSaveData(argumen, &database); 
            }
        }
        else if (strcmp(command, "exit") == 0) {
            if (JalankanExitManager(&database) == 0) { 
                break; 
            }	
        }
        else {
            printf("Perintah tidak dikenal! Silakan coba lagi.\n");
        }
    }

    DealokasiList(&database); 
    return 0; 
}
