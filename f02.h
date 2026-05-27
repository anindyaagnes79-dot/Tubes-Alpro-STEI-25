#ifndef F02_H
#define F02_H

#include "f00.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void cari(ListHalaman_Web l, char* query) {
    int i, j;
    bool ketemu = false;
    int len_q = 0;
    while (query[len_q] != '\0') {
        len_q++;
    }

    printf("Hasil cari '%s':\n", query);

    for (i = 0; i < l.nEff; i++) {
        bool sama = true;
        for (j = 0; j < len_q; j++) {
            if (l.HW[i].web_url[j] != query[j]) {
                sama = false;
                break;
            }
        }
        if (sama) {
            printf("- %s\n", l.HW[i].web_url);
            ketemu = true;
        }
    }
    if (!ketemu) {
        printf("Gak ketemu.\n");
    }
}

#endif
