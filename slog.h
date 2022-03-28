#ifndef SLOG_H_INCLUDED
#define SLOG_H_INCLUDED
#endif // SLOG_H_INCLUDED


#define FBLOKIRANJA 3
#define OZNAKA_KRAJA_DATOTEKE -1

typedef struct DateTime {

	int godina; // YYYY-MM-DD
	int mesec;
	int dan;
	int sati;   // HH:mm
	int minuti;
} DATETIME;


typedef struct {
    int sifraLeta; // kljuc
    DATETIME datum[17];
    char tipAviona[7];
    int brojPutnika;
    char kodAerodromaPolaska[4];
    char kodAerodromaDolaska[4];
} SLOG;


typedef struct Blok {
    SLOG slogovi[FBLOKIRANJA];
} BLOK;

typedef struct Map {
    char tipAviona[7];
    SLOG* slogovi;
    int sizeSlogovi;
} Map;

