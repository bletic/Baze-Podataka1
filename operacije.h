#ifndef OPERACIJE_H_INCLUDED
#define OPERACIJE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include "slog.h"



FILE *otvoriDatoteku(char *filename);
SLOG *pronadjiSlog(FILE *fajl, int sifraLeta);
void dodajSlog(FILE *fajl, SLOG *slog);
void ispisiSveSlogove(FILE *fajl);
void obrisiSlogFizicki(FILE *fajl, int sifraLeta);

int prosecanBrojPutnika(FILE* fajl, char* tip_aviona);
void najmanjiBrojPutnika(FILE* fajl);


// Pomocne operacije
void kreirajDatoteku(char* filename);
void ispisiSlog(SLOG* slog);

#endif // OPERACIJE_H_INCLUDED
