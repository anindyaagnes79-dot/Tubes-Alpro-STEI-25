#ifndef F11_H
#define F11_H

#include "f00.h"

void MuatDataDariCSV(const char *pathFile, ListHalaman_Web *database);

void MuatGrafDariCSV(const char *pathFile, ListHalaman_Web *database);

void EksekusiSaveData(char *folderTarget, ListHalaman_Web *database);

int JalankanExitManager(ListHalaman_Web *database);

#endif
