#include "f11.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void EksekusiSaveData(char *folderTarget, ListHalaman_Web *database) {
    printf("Saving data into %s folder...\n", folderTarget);

    char pathFile[150];
    sprintf(pathFile, "%s/web_pages.csv", folderTarget); 

    FILE *fOut = fopen(pathFile, "w");
    if (fOut != NULL) {
        int i;
        for (i = 0; i < database->nEff; i++) {
            fprintf(fOut, "%s,%s\n", database->HW[i].web_url, database->HW[i].content);
        }
        fclose(fOut);
    }
    printf("Data saved!\n\n");
}

int JalankanExitManager(ListHalaman_Web *database) {
    char pilihan;
    char folderSave[100];

    printf("Save data before exiting? (y/n)\n>>> ");
    scanf(" %c", &pilihan);

    if (pilihan == 'y' || pilihan == 'Y') {
        printf("\nPlease input the save folder\n>>> ");
        scanf("%s", folderSave);

        printf("\n");
        EksekusiSaveData(folderSave, database);
        
        printf("Goodbye~\n");
        return 0; 
    } 
    else if (pilihan == 'n' || pilihan == 'N') {
        printf("\nGoodbye~\n");
        return 0; 
    }
    
    printf("Pilihan tidak valid. Kembali ke browser.\n\n");
    return 1; 
}
