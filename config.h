#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

#define DEFAULT_CACHE_MAX    10
#define DEFAULT_TABS_MAX     10
#define DEFAULT_DOWNLOAD_MAX  5
#define DEFAULT_MAX_PAGES   100

typedef struct {
    int cache_max;     
    int tabs_max;     
    int download_max;  
    int max_web_pages; 
} AppConfig;

// Baca baris 1 config.txt dari filepath. Jika file tidak ada, gunakan nilai default. Return 1 sukses, 0 file tidak ditemukan (pakai default). //

int  config_load(AppConfig *cfg, const char *filepath);

// Tulis baris 1 config.txt. Baris selanjutnya (tab state) ditulis oleh modul tabs. //

void config_save_line1(const AppConfig *cfg, const char *filepath);

// Buat folder jika belum ada (POSIX mkdir) //
int  make_dir_if_needed(const char *folder);

// Gabungkan folder_path + filename → full path //
void build_path(char *dst, int dst_size,
                const char *folder, const char *filename);

#endif 