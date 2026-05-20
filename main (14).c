#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "web_pages.h"
#include "f04.h"
#include "adj_matrix.h"
#include "config.h"
#include "f03.h"
#include "f07.h" // Modul Navigasi History Back/Forward

// State aplikasi global (dilewatkan antar fungsi via pointer)
typedef struct {
    AppConfig     cfg;
    WebPageSet    pages;
    LinkedPageSet linked;
    CacheMap      cache;
    AdjMatrix     adj;
    char          current_folder[512];
    HistoryTab    history; // Menyimpan tracker riwayat navigasi tab saat ini
} AppState;

// Fungsi pengganti isspace() dari ctype.h agar steril sesuai batasan spek
static int ur_isspace(unsigned char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static void trim_str(char *s) {
    int len = (int)strlen(s);
    while (len > 0 && ur_isspace((unsigned char)s[len-1])) s[--len] = '\0';
    int st = 0;
    while (s[st] && ur_isspace((unsigned char)s[st])) st++;
    if (st > 0) memmove(s, s + st, len - st + 1);
}

// D03 – Muat semua file dari folder
static void do_load(AppState *app, const char *folder) {
    char path[1024];

    // Menggunakan fopen untuk memeriksa keberadaan folder/konfigurasi secara portabel standar C
    build_path(path, sizeof(path), folder, "config.txt");
    FILE *f_check = fopen(path, "r");
    if (f_check == NULL) {
        printf("Error: %s folder or config.txt not found!\n\n", folder);
        return;
    }
    fclose(f_check);

    printf("Loading new data from %s folder...\n", folder);

    // Simpan nama folder aktif
    strncpy(app->current_folder, folder, 511);
    app->current_folder[511] = '\0';

    // Bersihkan data lama di RAM
    wpset_free(&app->pages);
    lpset_free(&app->linked);
    cache_free(&app->cache);
    adj_free(&app->adj);

    // Baca config.txt (baris 1)
    config_load(&app->cfg, path);

    // Re-init dengan konfigurasi baru
    wpset_init(&app->pages,  app->cfg.max_web_pages);
    lpset_init(&app->linked);
    cache_init(&app->cache,  app->cfg.cache_max);
    adj_init(&app->adj,      app->cfg.max_web_pages);

    // Baca web_pages.csv
    build_path(path, sizeof(path), folder, "web_pages.csv");
    load_web_pages(&app->pages, path);

    // Baca linked_pages.csv
    build_path(path, sizeof(path), folder, "linked_pages.csv");
    load_linked_pages(&app->linked, path);

    // Bangun AdjMatrix (D01)
    adj_build(&app->adj, &app->pages, &app->linked);

    // Reset history tracker tab saat memuat folder konfigurasi baru
    f07_init(&app->history);

    printf("New data loaded\n");
    printf("  Halaman web  : %d (maks %d)\n", app->pages.size, app->cfg.max_web_pages);
    printf("  Linked pages : %d\n", app->linked.size);
    printf("  Cache maks   : %d\n\n", app->cfg.cache_max);
}

// D04 – Simpan semua file ke folder
static void do_save(AppState *app, const char *folder) {
    char path[1024];

    printf("Saving data into %s folder...\n", folder);

    // Deteksi folder duplikat secara aman dengan fopen via library standar C
    build_path(path, sizeof(path), folder, "config.txt");
    FILE *f_check = fopen(path, "r");
    if (f_check != NULL) {
        fclose(f_check);
        printf("WARNING: %s folder found, overwrite? (y/n): ", folder);
        char ans[8];
        if (!fgets(ans, sizeof(ans), stdin)) return;
        ans[strcspn(ans, "\n\r")] = '\0';
        if (ans[0] != 'y' && ans[0] != 'Y') {
            printf("Save dibatalkan.\n\n");
            return;
        }
        printf("Overwriting %s folder...\n", folder);
    }

    // Buat folder menggunakan fungsi pembantu bawaan template kelompokmu (sudah handle cross-OS internal)
    if (!make_dir_if_needed(folder)) {
        printf("Error: tidak bisa membuat folder %s\n\n", folder);
        return;
    }

    // Tulis config.txt (baris 1 dulu)
    config_save_line1(&app->cfg, path);

    // Tulis web_pages.csv
    build_path(path, sizeof(path), folder, "web_pages.csv");
    save_web_pages(&app->pages, path);

    // Tulis linked_pages.csv
    build_path(path, sizeof(path), folder, "linked_pages.csv");
    save_linked_pages(&app->linked, path);

    printf("Data saved!\n\n");
}

// F03 & F07 – Handle perintah 'open <url>' dengan siklus navigasi history
static void handle_open(AppState *app, const char *url) {
    // Langkah 1: Validasi awal, pastikan halaman pertama ada di RAM sebelum masuk ke history
    WebPage *check_exists = wpset_find_by_url(&app->pages, url);
    if (!check_exists) {
        printf("404 Not Found! Halaman tidak ditemukan.\n\n");
        return;
    }

    // Daftarkan URL utama ini ke dalam history tab
    f07_tambah_url(&app->history, url);

    while (1) {
        // Tarik URL aktif saat ini dari pointer penunjuk posisi_sekarang F07
        const char *current_url = f07_url_sekarang(&app->history);
        if (!current_url) return;

        OpenPageContext ctx;
        int cache_hit = 0;

        // Ambil data halaman (otomatis memproses sistem cache map di internal f03_open_page)
        int found = f03_open_page(current_url,
                                   &app->pages, &app->cache, &app->adj,
                                   &ctx, &cache_hit);
        if (!found) {
            printf("Error: Halaman dalam history tidak dapat dimuat.\n\n");
            return;
        }

        // Tampilkan konten halaman web ke layar
        f03_display_page(&ctx, cache_hit);

        char next_url[MAX_URL_LEN] = "";
        int  action = 0; // 0=home, 1=back, 2=forward, 3=openlinked
        
        // Membuka loop sesi interaktif di dalam halaman web
        f03_run_page_session(&ctx, next_url, &action);

        switch (action) {
            case 0: // home
                printf("Kembali ke menu utama.\n\n");
                return;

            case 1: // back – Integrasi Penuh F07
                f07_back(&app->history, &app->pages, &app->cache, &app->adj);
                break;

            case 2: // forward – Integrasi Penuh F07
                f07_forward(&app->history, &app->pages, &app->cache, &app->adj);
                break;

            case 3: // openlinked <x> – Rekam URL baru dan potong forward history
                f07_tambah_url(&app->history, next_url);
                break;
        }
    }
}

// D05 – Exit dengan opsi save
static void handle_exit(AppState *app) {
    printf("Save data before exiting? (yes/no): ");
    char ans[8];
    if (!fgets(ans, sizeof(ans), stdin)) goto bye;
    ans[strcspn(ans, "\n\r")] = '\0';

    if (ans[0] == 'y' || ans[0] == 'Y') {
        printf("Please input the save folder: ");
        char folder[512];
        if (!fgets(folder, sizeof(folder), stdin)) goto bye;
        folder[strcspn(folder, "\n\r")] = '\0';
        trim_str(folder);
        if (strlen(folder) > 0) do_save(app, folder);
    }

bye:
    printf("Good bye \n");
}

// Program utama browser
int main(int argc, char *argv[]) {
    AppState app;
    memset(&app, 0, sizeof(app));

    // Inisialisasi default parameter konfigurasi
    app.cfg.cache_max     = DEFAULT_CACHE_MAX;
    app.cfg.tabs_max      = DEFAULT_TABS_MAX;
    app.cfg.download_max  = DEFAULT_DOWNLOAD_MAX;
    app.cfg.max_web_pages = DEFAULT_MAX_PAGES;

    // Inisialisasi ADT Tracker History Tab
    f07_init(&app.history);

    // Tentukan folder parameter pembuka (D03 via Makefile / Default Folder)
    const char *start_folder = (argc >= 2) ? argv[1] : "config/";
    strncpy(app.current_folder, start_folder, 511);
    app.current_folder[511] = '\0';

    // Alokasi awal ruang data di RAM
    wpset_init(&app.pages,  app.cfg.max_web_pages);
    lpset_init(&app.linked);
    cache_init(&app.cache,  app.cfg.cache_max);
    adj_init(&app.adj,      app.cfg.max_web_pages);

    // D03 – Auto-load saat aplikasi dijalankan pertama kali
    do_load(&app, app.current_folder);

    // Loop menu utama terminal browser
    printf("--- BROWSER BELAJAR UTBK ---\n");
    printf("Perintah: open <url> | load <folder/> | save <folder/> | exit\n\n");

    char input[1024];
    while (1) {
        printf(">>> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        int len = (int)strlen(input);
        while (len > 0 && (input[len-1] == '\n' || input[len-1] == '\r'))
            input[--len] = '\0';
        trim_str(input);
        if (len == 0) continue;

        // Perintah: exit – D05
        if (strcmp(input, "exit") == 0) {
            handle_exit(&app);
            break;
        }

        // Perintah: open <url> – F03 & F07
        if (strncmp(input, "open ", 5) == 0) {
            char *url = input + 5;
            while (*url == ' ') url++;
            if (strlen(url) == 0) { printf("Usage: open <url>\n\n"); continue; }
            handle_open(&app, url);
            continue;
        }

        // Perintah: load <folder/> – D03
        else if (strncmp(input, "load ", 5) == 0) {
            char *folder = input + 5;
            while (*folder == ' ') folder++;
            if (strlen(folder) == 0) { printf("Usage: load <folder/>\n\n"); continue; }

            // Reset cache map karena data baru dari folder luar akan di-load
            cache_free(&app.cache);
            cache_init(&app.cache, app.cfg.cache_max);

            do_load(&app, folder);
            continue;
        }

        // Perintah: save <folder/> – D04
        else if (strncmp(input, "save ", 5) == 0) {
            char *folder = input + 5;
            while (*folder == ' ') folder++;
            if (strlen(folder) == 0) { printf("Usage: save <folder/>\n\n"); continue; }
            do_save(&app, folder);
            continue;
        }
        else {
            printf("Perintah tidak dikenali: '%s'\n\n", input);
        }
    }

    // Bersihkan seluruh alokasi memori RAM sebelum program selesai
    wpset_free(&app.pages);
    lpset_free(&app.linked);
    cache_free(&app.cache);
    adj_free(&app.adj);

    return 0;
}
