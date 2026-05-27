#ifndef F05
#define F05

#include "f09.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define IDX_UNDEF -1

#ifdef _WIN32
double getline(char **lineptr, size_t *n, FILE *stream) {
    if (lineptr == NULL || n == NULL || stream == NULL) {
        return -1;
    }
    if (*lineptr == NULL || *n == 0) {
        *n = 128;
        *lineptr = (char *)malloc(*n * sizeof(char));
        if (*lineptr == NULL) return -1;
    }

    int c;
    size_t posisi = 0;
    while ((c = fgetc(stream)) != EOF) {
        if (posisi >= *n - 1) {
            size_t ukuran_baru = *n * 2;
            char *temp = (char *)realloc(*lineptr, ukuran_baru * sizeof(char));
            if (temp == NULL) return -1;
            *lineptr = temp;
            *n = ukuran_baru;
        }
        (*lineptr)[posisi++] = (char)c;
        if (c == '\n') break;
    }
    if (posisi == 0) return -1;
    (*lineptr)[posisi] = '\0';
    return (double)posisi;
}
#endif

char* InputKonten() {
    int kapasitas_konten = 1;
    char *konten_dinamis = (char *)malloc(kapasitas_konten * sizeof(char));
    if (konten_dinamis == NULL) {
        printf("Gagal alokasi memori awal untuk konten!\n");
        return NULL;
    }
    konten_dinamis[0] = '\0';

    char *baris_dinamis = NULL;
    size_t kapasitas_baris = 0;

    printf("Masukkan konten :\n");

    while (true) {
        printf(">>> ");
        if (getline(&baris_dinamis, &kapasitas_baris, stdin) == -1) {
            break;
        }

        baris_dinamis[strcspn(baris_dinamis, "\n")] = 0;

        if (strcmp(baris_dinamis, ".") == 0) {
            break;
        }

        kapasitas_konten += strlen(baris_dinamis) + 2;

        char *temp = (char *)realloc(konten_dinamis, kapasitas_konten * sizeof(char));
        if (temp == NULL) {
            printf("Memori penuh, gagal memperbesar penampung konten!\n");
            free(konten_dinamis);
            free(baris_dinamis);
            return NULL;
        }
        konten_dinamis = temp;

        if (strlen(konten_dinamis) > 0) {
            strcat(konten_dinamis, "\n");
        }
        strcat(konten_dinamis, baris_dinamis);
    }

    free(baris_dinamis);
    return konten_dinamis;
}

void InputLinkedPages(ListHalaman_Web *l, int id_sumber) {
    char url_target[256];

    while (true) {
        printf(">>> ");
        if (scanf("%255s", url_target) == 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            if (strcmp(url_target, "DONE") == 0) {
                break;
            }

            int idx_tujuan = idx_url_database(l, url_target);
            if (idx_tujuan == IDX_UNDEF) {
                printf("URL tidak ditemukan!\n");
            } else {
                LinkedHalaman link;
                link.id_sumber = id_sumber;
                link.id_tujuan = l->HW[idx_tujuan].id;

                add_link_to_graph(l, link);
            }
        }
    }
}

void add_page(ListHalaman_Web *l, char* url) {
    if (url_ada_database(l, url)) {
        printf("Sudah terdapat halaman dengan url %s. Gunakan url lain!\n", url);
        return;
    }
    printf ("(Masukkan '.' untuk menyelesaikakan input)\n");
    char *konten_hasil_input = InputKonten();
    if (konten_hasil_input != NULL) {
        int id_baru = IDTerbesar(*l) + 1;
        Halaman_Web w;

        w.id = id_baru;
        w.web_url = (char *)malloc((strlen(url) + 1) * sizeof(char));
        if (w.web_url != NULL) {
            strcpy(w.web_url, url);
        }
        w.content = konten_hasil_input;

        TambahHalamanWeb(l, w);

        printf("Masukkan linked pages (Ketik 'DONE' jika sudah selesai)\n");
        InputLinkedPages(l, id_baru);

        printf("Halaman %s berhasil ditambahkan!\n", url);
    }
}

void edit_page(ListHalaman_Web *l, char* url) {
    int idx = idx_url_database(l, url);

    if (idx == IDX_UNDEF) {
        printf("Tidak ada halaman dengan url %s!\n", url);
        return;
    }

    printf("[Status: Cache-Hit] Mengambil data dari cache...\n\n");
    printf("Konten saat ini:\n%s\n\n", l->HW[idx].content);

    printf("Linked pages:\n");
    int nomor_urut = 1;
    int i;
    for (i = 0; i < l->nEff; i++) {
        if (graph.matrix[idx][i] == 1) {
            printf("[%d] %s\n", nomor_urut++, l->HW[i].web_url);
        }
    }
    if (nomor_urut == 1) {
        printf("(Halaman ini tidak memiliki tautan keluar)\n");
    }
    printf("\n");

    printf("Masukkan konten baru (akhiri dengan '.' atau ketik '.' saja jika tidak ingin mengubah konten)\n");
    char *konten_baru = InputKonten();
    if (konten_baru != NULL && strlen(konten_baru) > 0) {
        free(l->HW[idx].content);
        l->HW[idx].content = konten_baru;
    } else if (konten_baru != NULL) {
        free(konten_baru); 
    }

    char opsi_link[256];
    printf("Masukkan linked pages baru (Ketik 'DONE' jika sudah selesai, atau ketik 'SKIP' jika tidak ingin mengubah linked pages)\n");
    printf(">>> ");
    if (scanf("%255s", opsi_link) == 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (strcmp(opsi_link, "SKIP") != 0) {
            int j;
            for (j = 0; j < MAX_WEB_PAGES; j++) {
                if (graph.matrix[idx][j] == 1) {
                    graph.matrix[idx][j] = 0;
                    graph.ukuran--;
                }
            }

            if (strcmp(opsi_link, "DONE") != 0) {
                int idx_tujuan = idx_url_database(l, opsi_link);
                if (idx_tujuan == IDX_UNDEF) {
                    printf("URL tidak ditemukan!\n");
                } else {
                    LinkedHalaman link;
                    link.id_sumber = l->HW[idx].id;
                    link.id_tujuan = l->HW[idx_tujuan].id;
                    add_link_to_graph(l, link);
                }
                InputLinkedPages(l, l->HW[idx].id);
            }
        }
    }

    printf("Halaman %s berhasil diperbarui!\n", url);
}

void delete_page(ListHalaman_Web *l, char* url) {
    int idx = idx_url_database(l, url);

    if (idx == IDX_UNDEF) {
        printf("Tidak ada halaman dengan url %s!\n", url);
        return;
    }

    printf("[Status: Cache-Hit] URL ditemukan di cache dan telah dibersihkan.\n");
    printf("Membersihkan relasi linked pages...\n");

    int i;
    int j;
	for (i = 0; i < l->nEff; i++) {
        if (graph.matrix[idx][i] == 1) graph.ukuran--;
        if (graph.matrix[i][idx] == 1 && i != idx) graph.ukuran--;
    }

    for (i = idx; i < l->nEff - 1; i++) {
        for (j = 0; j < l->nEff; j++) {
            graph.matrix[i][j] = graph.matrix[i + 1][j];
        }
    }

    for (j = idx; j < l->nEff - 1; j++) {
        for (i = 0; i < l->nEff; i++) {
            graph.matrix[i][j] = graph.matrix[i][j + 1];
        }
    }
    
    for (i = 0; i < l->nEff; i++) {
        graph.matrix[l->nEff - 1][i] = 0;
        graph.matrix[i][l->nEff - 1] = 0;
    }

    if (current_active_page_id == l->HW[idx].id) {
        current_active_page_id = -1;
        current_linked_count = 0;
    }

    free(l->HW[idx].web_url);
    free(l->HW[idx].content);
    l->HW[idx].web_url = NULL;
    l->HW[idx].content = NULL;

    for (i = idx; i < l->nEff - 1; i++) {
        l->HW[i] = l->HW[i + 1];
    }
    l->nEff--;

    printf("\nHalaman %s berhasil dihapus!\n", url);
}

#endif 
