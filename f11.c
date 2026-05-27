#include "f11.h" 
#include "f09.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void TulisKontenCSV(FILE *file, const char *konten) {
    int i = 0;
    fprintf(file, "\""); 
    while (konten[i] != '\0') {
        if (konten[i] == '\n') {
            fprintf(file, "\\n"); 
        } else {
            fputc(konten[i], file);
        }
        i++;
    }
    fprintf(file, "\""); 
}

void MuatDataDariCSV(const char *pathFile, ListHalaman_Web *database) {
    FILE *file = fopen(pathFile, "r");
    if (file == NULL) {
        printf("Peringatan: Berkas database '%s' tidak ditemukan. Memulai dengan database kosong.\n", pathFile);
        return;
    }

    char baris[1024];
    if (fgets(baris, sizeof(baris), file) == NULL) {
        fclose(file);
        return;
    }

    while (fgets(baris, sizeof(baris), file) != NULL) {
        baris[strcspn(baris, "\r\n")] = 0; 

        char *token_id = strtok(baris, ",");
        if (token_id == NULL) continue;
        int id_web = atoi(token_id);

        char *token_url = strtok(NULL, ",");
        if (token_url == NULL) continue;

        char *token_content = strtok(NULL, "\"");
        if (token_content == NULL) continue;

        int len_content = strlen(token_content);
        char *konten_bersih = (char *)malloc((len_content + 1) * sizeof(char));
        int i = 0, j = 0;

        while (i < len_content) {
            if (token_content[i] == '\\' && token_content[i + 1] == 'n') {
                konten_bersih[j] = '\n'; 
                i += 2;
            } else {
                konten_bersih[j] = token_content[i];
                i++;
            }
            j++;
        }
        konten_bersih[j] = '\0';

        Halaman_Web web_baru;
        web_baru.id = id_web;
        web_baru.web_url = (char *)malloc((strlen(token_url) + 1) * sizeof(char));
        strcpy(web_baru.web_url, token_url);
        web_baru.content = konten_bersih;

        TambahHalamanWeb(database, web_baru);
    }

    fclose(file);
}

void MuatGrafDariCSV(const char *pathFile, ListHalaman_Web *database) {
    FILE *file = fopen(pathFile, "r");
    if (file == NULL) {
        return;
    }

    char baris[256];
    if (fgets(baris, sizeof(baris), file) == NULL) {
        fclose(file);
        return;
    }

    while (fgets(baris, sizeof(baris), file) != NULL) {
        baris[strcspn(baris, "\r\n")] = 0;

        char *token_id = strtok(baris, ",");
        if (token_id == NULL) continue;

        char *token_sumber = strtok(NULL, ",");
        if (token_sumber == NULL) continue;
        int id_src = atoi(token_sumber); 

        char *token_tujuan = strtok(NULL, ",");
        if (token_tujuan == NULL) continue;
        int id_dst = atoi(token_tujuan); 

        LinkedHalaman relasi_link;
        relasi_link.id_sumber = id_src;
        relasi_link.id_tujuan = id_dst;

        add_link_to_graph(database, relasi_link);
    }

    fclose(file);
}

void EksekusiSaveData(char *folderTarget, ListHalaman_Web *database) {
    char pathWeb[256];
    char pathLink[256];
    
    sprintf(pathWeb, "%s/web_pages.csv", folderTarget);
    sprintf(pathLink, "%s/linked_pages.csv", folderTarget);

    FILE *fWeb = fopen(pathWeb, "w");
    if (fWeb != NULL) {
        fprintf(fWeb, "id,web_url,content\n");
        int i;
        for (i = 0; i < database->nEff; i++) {
            fprintf(fWeb, "%d,%s,", database->HW[i].id, database->HW[i].web_url);
            TulisKontenCSV(fWeb, database->HW[i].content);
            fprintf(fWeb, "\n");
        }
        fclose(fWeb);
    }

    FILE *fLink = fopen(pathLink, "w");
    if (fLink != NULL) {
        fprintf(fLink, "id,id_sumber,id_tujuan\n");
        int id_counter = 1;
        int i, j;
        for (i = 0; i < database->nEff; i++) {
            for (j = 0; j < database->nEff; j++) {
                if (graph.matrix[i][j] == 1) { 
                    fprintf(fLink, "%d,%d,%d\n", id_counter++, database->HW[i].id, database->HW[j].id);
                }
            }
        }
        fclose(fLink);
    }
}

int JalankanExitManager(ListHalaman_Web *database) {
    char pilihan;
    char folderSave[100];
    int status_loop = 1;
    int nilai_return = 1; 

    while (status_loop) {
        printf("Save data before exiting? (y/n)\n");
        printf(">>> ");
        scanf(" %c", &pilihan);

        if (pilihan == 'y' || pilihan == 'Y') {
            printf("\nPlease input the save folder\n");
            printf(">>> ");
            scanf("%s", folderSave);

            printf("Saving data into %s folder...\n", folderSave);
            EksekusiSaveData(folderSave, database);
            printf("Data saved!\n");
            printf("Goodbye~\n");
            
            nilai_return = 0; 
            status_loop = 0;  
        } 
        else if (pilihan == 'n' || pilihan == 'N') {
            printf("Goodbye~\n");
            nilai_return = 0; 
            status_loop = 0;  
        } 
        else {
            printf("Pilihan tidak valid. Silakan ketik 'y' atau 'n'.\n\n");
            status_loop = 0;  
        }
    }
    
    return nilai_return;
}
