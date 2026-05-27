#include "f03.h"
#include "f04.h"
#include "f07.h"
#include "f09.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

extern HistoryTab riwayatBrowser; 
extern LRUCache sistemCache; 

static void bangun_adt_webpage_set(ListHalaman_Web *l, WebPageSet *WSet) {
    WSet->size = l->nEff;
    WSet->capacity = MAX_WEB_PAGES;
    WSet->pages = (WebPage *)malloc(WSet->size * sizeof(WebPage));
    
    int i;
    for (i = 0; i < l->nEff; i++) {
        WSet->pages[i].id = l->HW[i].id;
        
        char temp_url[MAX_URL_LEN];
        strncpy(temp_url, l->HW[i].web_url, MAX_URL_LEN - 1);
        temp_url[MAX_URL_LEN - 1] = '\0';
        temp_url[strcspn(temp_url, "\r\n")] = 0;
        
        int src_idx = 0;
        int dst_idx = 0;
        while (temp_url[src_idx] != '\0') {
            if (temp_url[src_idx] != '"') {
                WSet->pages[i].web_url[dst_idx] = temp_url[src_idx];
                dst_idx++;
            }
            src_idx++;
        }
        WSet->pages[i].web_url[dst_idx] = '\0';
        
        strncpy(WSet->pages[i].content, l->HW[i].content, MAX_CONTENT_LEN - 1);
        WSet->pages[i].content[MAX_CONTENT_LEN - 1] = '\0';
    }
}

static void bangun_adt_linked_page_set(ListHalaman_Web *l, int idx_sumber, LinkedPageSet *LSet) {
    LSet->size = 0;
    LSet->capacity = MAX_WEB_PAGES;
    LSet->links = (LinkedPage *)malloc(LSet->capacity * sizeof(LinkedPage));
    
    if (idx_sumber < 0 || idx_sumber >= l->nEff) {
        return;
    }

    int id_sumber_aktif = l->HW[idx_sumber].id;

    FILE *file = fopen("linked_pages.csv", "r");
    if (file == NULL) {
        file = fopen("config/linked_pages.csv", "r");
    }
    if (file == NULL) {
        file = fopen("data/linked_pages.csv", "r");
    }

    if (file != NULL) {
        char baris[256];
        while (fgets(baris, sizeof(baris), file) != NULL) {
            int csv_id = -1;
            int csv_id_sumber = -1;
            int csv_id_tujuan = -1;
            
            if (sscanf(baris, "%d,%d,%d", &csv_id, &csv_id_sumber, &csv_id_tujuan) == 3) {
                if (csv_id_sumber == id_sumber_aktif) {
                    if (LSet->size < LSet->capacity) {
                        LSet->links[LSet->size].id_sumber = csv_id_sumber;
                        LSet->links[LSet->size].id_tujuan = csv_id_tujuan;
                        LSet->size++;
                    }
                }
            }
        }
        fclose(file);
    }
}

void open_page(ListHalaman_Web *l, char* url) {
    if (url != NULL) {
        url[strcspn(url, "\r\n ")] = 0;
    }

    WebPageSet databaseSet;
    bangun_adt_webpage_set(l, &databaseSet);
    
    int idx_sumber = -1;
    int k;
    for (k = 0; k < databaseSet.size; k++) {
        if (strcmp(databaseSet.pages[k].web_url, url) == 0) {
            idx_sumber = k;
            break;
        }
    }

    if (idx_sumber == -1) {
        printf("ERROR: HALAMAN DENGAN URL %s TIDAK DITEMUKAN!\n", url);
        free(databaseSet.pages);
        return;
    }

    LinkedPageSet relasiKeluarSet;
    bangun_adt_linked_page_set(l, idx_sumber, &relasiKeluarSet);

    current_active_page_id = l->HW[idx_sumber].id;
    current_linked_count = 0;

    f07_tambah_url(&riwayatBrowser, databaseSet.pages[idx_sumber].web_url);
    cache_put(&sistemCache, &databaseSet.pages[idx_sumber]);

    printf("\n--------------------------------------------------\n");
    printf("URL: %s\n", databaseSet.pages[idx_sumber].web_url);
    printf("Content:\n%s\n", databaseSet.pages[idx_sumber].content);
    printf("--------------------------------------------------\n");
    
    printf("\nLinked pages (Daftar Elemen ADT LinkedPageSet):\n");
    int nomor_urut = 1;
    int i, j;

    for (i = 0; i < relasiKeluarSet.size; i++) {
        int id_target = relasiKeluarSet.links[i].id_tujuan;
        int idx_ketemu = -1;
        for (k = 0; k < databaseSet.size; k++) {
            if (databaseSet.pages[k].id == id_target) {
                idx_ketemu = k;
                break;
            }
        }

        if (idx_ketemu != -1) {
            if (current_linked_count >= MAX_WEB_PAGES) {
                break;
            }
            printf("[%d] %s\n", nomor_urut, databaseSet.pages[idx_ketemu].web_url);
            current_linked_ids[nomor_urut - 1] = id_target;
            current_linked_count++;
            nomor_urut++;
        }
    }

    if (current_linked_count == 0) {
        printf("(Halaman ini tidak memiliki tautan keluar)\n");
    }

    char subInput[512]; char subCommand[50]; char subArgumen[256];

    while (true) {
        printf("\n[Page-Mode] >>> ");
        if (fgets(subInput, sizeof(subInput), stdin) == NULL) {
            break; 
        }

        subInput[strcspn(subInput, "\n")] = 0;
        if (strlen(subInput) == 0) continue;

        int scanResult = sscanf(subInput, "%s %s", subCommand, subArgumen);
        if (scanResult < 2) subArgumen[0] = '\0';

        if (strcmp(subCommand, "home") == 0) {
            printf("Keluar dari halaman web. Kembali ke menu utama simulator.\n");
            current_active_page_id = -1;
            current_linked_count = 0;
            break; 
        }
        else if (strcmp(subCommand, "openlinked") == 0) {
            int nomorTautan = atoi(subArgumen);
            
            if (nomorTautan >= 1 && nomorTautan <= current_linked_count) {
                int id_tujuan_target = current_linked_ids[nomorTautan - 1];
                int idx_target_baru = -1;
                
                for (k = 0; k < databaseSet.size; k++) {
                    if (databaseSet.pages[k].id == id_tujuan_target) {
                        idx_target_baru = k;
                        break;
                    }
                }
                
                if (idx_target_baru != -1) {
                    idx_sumber = idx_target_baru;
                    current_active_page_id = databaseSet.pages[idx_sumber].id;
                    current_linked_count = 0;
                    
                    f07_tambah_url(&riwayatBrowser, databaseSet.pages[idx_sumber].web_url);
                    cache_put(&sistemCache, &databaseSet.pages[idx_sumber]);
                    
                    free(relasiKeluarSet.links);
                    bangun_adt_linked_page_set(l, idx_sumber, &relasiKeluarSet);
                    
                    printf("\n--------------------------------------------------\n");
                    printf("URL: %s\n", databaseSet.pages[idx_sumber].web_url);
                    printf("Content:\n%s\n", databaseSet.pages[idx_sumber].content);
                    printf("--------------------------------------------------\n");
                    
                    printf("\nLinked pages (Daftar Elemen ADT LinkedPageSet):\n");
                    int nUrut = 1;
                    for (i = 0; i < relasiKeluarSet.size; i++) {
                        int id_target = relasiKeluarSet.links[i].id_tujuan;
                        int idx_ketemu = -1;
                        for (k = 0; k < databaseSet.size; k++) {
                            if (databaseSet.pages[k].id == id_target) {
                                idx_ketemu = k;
                                break;
                            }
                        }
                        if (idx_ketemu != -1) {
                            if (current_linked_count >= MAX_WEB_PAGES) break;
                            printf("[%d] %s\n", nUrut, databaseSet.pages[idx_ketemu].web_url);
                            current_linked_ids[nUrut - 1] = id_target;
                            current_linked_count++;
                            nUrut++;
                        }
                    }
                    if (current_linked_count == 0) {
                        printf("(Halaman ini tidak memiliki tautan keluar)\n");
                    }
                }
            } else {
                if (current_linked_count == 0) {
                    printf("ERROR: Halaman ini tidak memiliki tautan keluar yang bisa dibuka!\n");
                } else {
                    printf("ERROR: Nomor tautan tidak valid! Silakan masukkan angka 1 sampai %d.\n", current_linked_count);
                }
            }
        }
        else if (strcmp(subCommand, "back") == 0) {
            if (f07_back(&riwayatBrowser)) {
                int topIdx = riwayatBrowser.backStack.top;
                if (topIdx >= 0) {
                    int idxTarget = idx_url_database(l, riwayatBrowser.backStack.data[topIdx]);
                    if (idxTarget != IDX_UNDEF) {
                        idx_sumber = idxTarget;
                        current_active_page_id = l->HW[idx_sumber].id;
                        current_linked_count = 0;
                        
                        int set_idx = idx_sumber;
                        for (k = 0; k < databaseSet.size; k++) {
                            if (databaseSet.pages[k].id == l->HW[idx_sumber].id) {
                                set_idx = k;
                                break;
                            }
                        }
                        
                        cache_put(&sistemCache, &databaseSet.pages[set_idx]);
                        
                        printf("\n--------------------------------------------------\n");
                        printf("URL: %s\n", databaseSet.pages[set_idx].web_url);
                        printf("Content:\n%s\n", databaseSet.pages[set_idx].content);
                        printf("--------------------------------------------------\n");
                        
                        free(relasiKeluarSet.links);
                        bangun_adt_linked_page_set(l, idx_sumber, &relasiKeluarSet);
                        
                        printf("\nLinked pages:\n");
                        int nUrut = 1;
                        for (j = 0; j < relasiKeluarSet.size; j++) {
                            int t_id = relasiKeluarSet.links[j].id_tujuan;
                            int k_idx = -1;
                            for (k = 0; k < databaseSet.size; k++) {
                                if (databaseSet.pages[k].id == t_id) {
                                    k_idx = k;
                                    break;
                                }
                            }
                            if (k_idx != -1) {
                                if (current_linked_count >= MAX_WEB_PAGES) break;
                                printf("[%d] %s\n", nUrut, databaseSet.pages[k_idx].web_url);
                                current_linked_ids[nUrut - 1] = t_id;
                                current_linked_count++;
                                nUrut++;
                            }
                        }
                        if (current_linked_count == 0) printf("(Halaman ini tidak memiliki tautan keluar)\n");
                    }
                }
            }
        }
        else if (strcmp(subCommand, "forward") == 0) {
            if (f07_forward(&riwayatBrowser)) {
                int topIdx = riwayatBrowser.backStack.top;
                if (topIdx >= 0) {
                    int idxTarget = idx_url_database(l, riwayatBrowser.backStack.data[topIdx]);
                    if (idxTarget != IDX_UNDEF) {
                        idx_sumber = idxTarget;
                        current_active_page_id = l->HW[idx_sumber].id;
                        current_linked_count = 0;
                        
                        int set_idx = idx_sumber;
                        for (k = 0; k < databaseSet.size; k++) {
                            if (databaseSet.pages[k].id == l->HW[idx_sumber].id) {
                                set_idx = k;
                                break;
                            }
                        }
                        
                        cache_put(&sistemCache, &databaseSet.pages[set_idx]);
                        
                        printf("\n--------------------------------------------------\n");
                        printf("URL: %s\n", databaseSet.pages[set_idx].web_url);
                        printf("Content:\n%s\n", databaseSet.pages[set_idx].content);
                        printf("--------------------------------------------------\n");
                        
                        free(relasiKeluarSet.links);
                        bangun_adt_linked_page_set(l, idx_sumber, &relasiKeluarSet);
                        
                        printf("\nLinked pages:\n");
                        int nUrut = 1;
                        for (j = 0; j < relasiKeluarSet.size; j++) {
                            int t_id = relasiKeluarSet.links[j].id_tujuan;
                            int k_idx = -1;
                            for (k = 0; k < databaseSet.size; k++) {
                                if (databaseSet.pages[k].id == t_id) {
                                    k_idx = k;
                                    break;
                                }
                            }
                            if (k_idx != -1) {
                                if (current_linked_count >= MAX_WEB_PAGES) break;
                                printf("[%d] %s\n", nUrut, databaseSet.pages[k_idx].web_url);
                                current_linked_ids[nUrut - 1] = t_id;
                                current_linked_count++;
                                nUrut++;
                            }
                        }
                        if (current_linked_count == 0) printf("(Halaman ini tidak memiliki tautan keluar)\n");
                    }
                }
            }
        }
        else {
            printf("Akses Ditolak: Selama halaman terbuka, hanya fitur 'home', 'back', 'forward', dan 'openlinked' yang aktif!\n");
        }
    }

    free(databaseSet.pages);
    free(relasiKeluarSet.links);
}
