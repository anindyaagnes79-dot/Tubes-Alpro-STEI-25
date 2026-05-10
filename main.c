#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#include "web_pages.h"
#include "cache.h"
#include "adj_matrix.h"
#include "config.h"
#include "f03.h"

//State aplikasi global (dilewatkan antar fungsi via pointer) //
typedef struct {
    AppConfig     cfg;
    WebPageSet    pages;
    LinkedPageSet linked;
    CacheMap      cache;
    AdjMatrix     adj;
    char          current_folder[512];
} AppState;

static void trim_str(char *s) {
    int len = (int)strlen(s);
    while (len > 0 && isspace((unsigned char)s[len-1])) s[--len] = '\0';
    int st = 0;
    while (s[st] && isspace((unsigned char)s[st])) st++;
    if (st > 0) memmove(s, s + st, len - st + 1);
}

// D03 – Muat semua file dari folder //
static void do_load(AppState *app, const char *folder) {
    char path[1024];

    // Cek folder ada //
    struct stat st_check;
    if (stat(folder, &st_check) != 0) {
        printf("Error: %s folder not found!\n\n", folder);
        return;
    }

    printf("Loading new data from %s folder...\n", folder);

    // Simpan nama folder aktif //
    strncpy(app->current_folder, folder, 511);
    app->current_folder[511] = '\0';

    // Bersihkan data lama //
    wpset_free(&app->pages);
    lpset_free(&app->linked);
    cache_free(&app->cache);
    adj_free(&app->adj);

    // Baca config.txt (baris 1) //
    build_path(path, sizeof(path), folder, "config.txt");
    config_load(&app->cfg, path);

    // Re-init dengan konfigurasi baru //
    wpset_init(&app->pages,  app->cfg.max_web_pages);
    lpset_init(&app->linked);
    cache_init(&app->cache,  app->cfg.cache_max);
    adj_init(&app->adj,      app->cfg.max_web_pages);

    // Baca web_pages.csv //
    build_path(path, sizeof(path), folder, "web_pages.csv");
    load_web_pages(&app->pages, path);

    // Baca linked_pages.csv //
    build_path(path, sizeof(path), folder, "linked_pages.csv");
    load_linked_pages(&app->linked, path);

    // Bangun AdjMatrix (D01) //
    adj_build(&app->adj, &app->pages, &app->linked);

    printf("New data loaded\n");
    printf("  Halaman web  : %d (maks %d)\n", app->pages.size, app->cfg.max_web_pages);
    printf("  Linked pages : %d\n", app->linked.size);
    printf("  Cache maks   : %d\n\n", app->cfg.cache_max);
}

// D04 – Simpan semua file ke folder //
static void do_save(AppState *app, const char *folder) {
    char path[1024];

    printf("Saving data into %s folder...\n", folder);

    // Cek apakah folder sudah ada //
    struct stat st_check;
    if (stat(folder, &st_check) == 0) {
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

    // Buat folder jika belum ada //
    if (!make_dir_if_needed(folder)) {
        printf("Error: tidak bisa membuat folder %s\n\n", folder);
        return;
    }

    // Tulis config.txt (baris 1 dulu) //
    build_path(path, sizeof(path), folder, "config.txt");
    config_save_line1(&app->cfg, path);

    // Tulis web_pages.csv //
    build_path(path, sizeof(path), folder, "web_pages.csv");
    save_web_pages(&app->pages, path);

    // Tulis linked_pages.csv //
    build_path(path, sizeof(path), folder, "linked_pages.csv");
    save_linked_pages(&app->linked, path);

    printf("Data saved!\n\n");
}

// F03 – Handle perintah 'open <url>' . Termasuk loop session (home/back/forward/openlinked) //
static void handle_open(AppState *app, const char *url) {
    char current_url[MAX_URL_LEN];
    strncpy(current_url, url, MAX_URL_LEN - 1);
    current_url[MAX_URL_LEN - 1] = '\0';

    while (1) {
        OpenPageContext ctx;
        int cache_hit = 0;

        int found = f03_open_page(current_url,
                                   &app->pages, &app->cache, &app->adj,
                                   &ctx, &cache_hit);
        if (!found) {
            printf("404 Not Found! Halaman tidak ditemukan.\n\n");
            return;
        }

        f03_display_page(&ctx, cache_hit);

        char next_url[MAX_URL_LEN] = "";
        int  action = 0;
        f03_run_page_session(&ctx, next_url, &action);

        switch (action) {
            case 0: // home //
                printf("Kembali ke menu utama.\n\n");
                return;

            case 1: // back – integrasi dengan F07 (ADT Stack per tab) //
                printf("[Back] Akan diintegrasikan dengan modul F07.\n\n");
                return;

            case 2: // forward //
                printf("[Forward] Akan diintegrasikan dengan modul F07.\n\n");
                return;

            case 3: // openlinked <x> //
                strncpy(current_url, next_url, MAX_URL_LEN - 1);
                current_url[MAX_URL_LEN - 1] = '\0';
                break;
        }
    }
}

// D05 – Exit dengan opsi save //
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

// main //
int main(int argc, char *argv[]) {
    AppState app;
    memset(&app, 0, sizeof(app));

    // Default konfigurasi sebelum load //
    app.cfg.cache_max     = DEFAULT_CACHE_MAX;
    app.cfg.tabs_max      = DEFAULT_TABS_MAX;
    app.cfg.download_max  = DEFAULT_DOWNLOAD_MAX;
    app.cfg.max_web_pages = DEFAULT_MAX_PAGES;

    // Tentukan folder dari argv[1] (D03 – makefile pass CONFIG_FOLDER) //
    const char *start_folder = (argc >= 2) ? argv[1] : "config/";
    strncpy(app.current_folder, start_folder, 511);
    app.current_folder[511] = '\0';

    // Inisialisasi awal dengan default, lalu load //
    wpset_init(&app.pages,  app.cfg.max_web_pages);
    lpset_init(&app.linked);
    cache_init(&app.cache,  app.cfg.cache_max);
    adj_init(&app.adj,      app.cfg.max_web_pages);

    // D03 – Auto-load saat startup //
    do_load(&app, app.current_folder);

    // Loop menu utama //
    printf("--- Browser Kelompok C06 ---\n");
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

        // exit – D05 //
        if (strcmp(input, "exit") == 0) {
            handle_exit(&app);
            break;
        }

        // open <url> – F03 //
        if (strncmp(input, "open ", 5) == 0) {
            char *url = input + 5;
            while (*url == ' ') url++;
            if (strlen(url) == 0) { printf("Usage: open <url>\n\n"); continue; }
            handle_open(&app, url);
            continue;
        }

        // load <folder/> – D03 //
        if (strncmp(input, "load ", 5) == 0) {
            char *folder = input + 5;
            while (*folder == ' ') folder++;
            if (strlen(folder) == 0) { printf("Usage: load <folder/>\n\n"); continue; }

            // Reset cache karena data baru di-load //
            cache_free(&app.cache);
            cache_init(&app.cache, app.cfg.cache_max);

            do_load(&app, folder);
            continue;
        }

        // save <folder/> – D04 //
        if (strncmp(input, "save ", 5) == 0) {
            char *folder = input + 5;
            while (*folder == ' ') folder++;
            if (strlen(folder) == 0) { printf("Usage: save <folder/>\n\n"); continue; }
            do_save(&app, folder);
            continue;
        }

        printf("Perintah tidak dikenali: '%s'\n\n", input);
    }

    // Bersihkan memori //
    wpset_free(&app.pages);
    lpset_free(&app.linked);
    cache_free(&app.cache);
    adj_free(&app.adj);

    return 0;
}
