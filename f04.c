#include "f04.h"
#include <stdio.h>
#include <string.h>

static MemoriCache cacheSistem = {{}, 0};

void CekDanPerbaruiCache(ListHalaman_Web *listData, char *alamatUrl) {
    int i;
    int statusHit = 0;

    for (i = 0; i < cacheSistem.jumlahTerisi; i++) {
        if (strcmp(cacheSistem.daftarSlot[i].web_url, alamatUrl) == 0) {
            printf("[Status: Cache-Hit] Mengambil data dari cache...\n");
            statusHit = 1;
            break;
        }
    }

    if (statusHit == 0) {
        printf("[Status: Cache-Miss] Mengambil data dari database...\n");
        int indeksDatabase = idx_url_database(listData, alamatUrl);

        if (indeksDatabase != IDX_UNDEF) {
            Halaman_Web halamanBaru = listData->HW[indeksDatabase];

            if (cacheSistem.jumlahTerisi < CACHE_MAX_AMOUNT) {
                cacheSistem.daftarSlot[cacheSistem.jumlahTerisi] = halamanBaru;
                cacheSistem.jumlahTerisi++;
            } 
            else {
                for (i = 0; i < CACHE_MAX_AMOUNT - 1; i++) {
                    cacheSistem.daftarSlot[i] = cacheSistem.daftarSlot[i + 1];
                }
                cacheSistem.daftarSlot[CACHE_MAX_AMOUNT - 1] = halamanBaru;
            }
        }
    }
}
