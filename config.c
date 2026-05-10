#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

int config_load(AppConfig *cfg, const char *filepath) {
    // Default values dulu //
    cfg->cache_max     = DEFAULT_CACHE_MAX;
    cfg->tabs_max      = DEFAULT_TABS_MAX;
    cfg->download_max  = DEFAULT_DOWNLOAD_MAX;
    cfg->max_web_pages = DEFAULT_MAX_PAGES;

    FILE *fp = fopen(filepath, "r");
    if (!fp) return 0;

    // Baca baris 1: CACHE_MAX TABS_MAX DOWNLOAD_MAX MAX_WEB_PAGES //
    int c, t, d, m;
    if (fscanf(fp, "%d %d %d %d", &c, &t, &d, &m) == 4) {
        cfg->cache_max     = c;
        cfg->tabs_max      = t;
        cfg->download_max  = d;
        cfg->max_web_pages = m;
    }
    fclose(fp);
    return 1;
}

// Overwrite baris 1 config.txt.Baris 2+ (tab state) ditulis oleh modul tabs secara terpisah. Di sini hanya baris 1. //

void config_save_line1(const AppConfig *cfg, const char *filepath) {
    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        fprintf(stderr, "Error: tidak bisa menulis %s\n", filepath);
        return;
    }
    fprintf(fp, "%d %d %d %d\n",
            cfg->cache_max, cfg->tabs_max,
            cfg->download_max, cfg->max_web_pages);
    fclose(fp);
}

// Buat folder jika belum ada //
int make_dir_if_needed(const char *folder) {
    struct stat st;
    if (stat(folder, &st) == 0) {
        // Sudah ada – pastikan memang directory //
        return S_ISDIR(st.st_mode) ? 1 : 0;
    }
    // Buat folder dengan permission 0755 //
    if (mkdir(folder, 0755) == 0) return 1;
    return 0;
}

// Gabung folder + "/" + filename //
void build_path(char *dst, int dst_size,
                const char *folder, const char *filename) {
    int flen = (int)strlen(folder);
    if (flen > 0 && folder[flen - 1] == '/') {
        snprintf(dst, dst_size, "%s%s", folder, filename);
    } else {
        snprintf(dst, dst_size, "%s/%s", folder, filename);
    }
}