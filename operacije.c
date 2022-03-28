#include "operacije.h"

void kreirajDatoteku(char* filename)
{

    // Otvaramo fajl za pisanje
    FILE* fajl = fopen(filename, "wb");
    // Proveravamo ako je NULL, ispisujemo gresku
    if(fajl == NULL)
    {
        printf("[Error] Greska pri kreiranju datoteke pod imenom: %s \n", filename);
    }
    // Ako smo ga uspesno kreirali,
    // napravicemo novi blok u koji cemo upisati -1 tj Oznaku kraja datoteke
    else
    {
        BLOK blok;
        blok.slogovi[0].sifraLeta = OZNAKA_KRAJA_DATOTEKE;
        fwrite(&blok, sizeof(BLOK),1, fajl);
        printf("Datoteka pod imenom: %s je uspesno kreirana \n", filename);
        fclose(fajl);
    }

}

FILE* otvoriDatoteku(char* filename){
    // Otvaramo fajl u rezimu sa pisanje  i + moze i  citanje
    FILE* fajl = fopen(filename, "rb+");
    if(fajl == NULL){
        printf("[Error] Datoteka pod imenom %s ne postoji \n", filename);
    }else{
        printf("Datoteka %s je otvorena \n", filename);
    }

    return fajl;
}

SLOG* pronadjiSlog(FILE* fajl, int sifraLeta){
    if(fajl == NULL){
        return NULL;
    }


    BLOK blok;

    //Poziciju fajla pre citanja stavljamo na pocetak.
    fseek(fajl,0,SEEK_SET);
    while(fread(&blok, sizeof(BLOK),1, fajl)){

        for(int i=0; i<FBLOKIRANJA; i++){
            if( blok.slogovi[i].sifraLeta == OZNAKA_KRAJA_DATOTEKE  ){
                // znaci ako smo dosli do kraja, nismo pronasli slog!
                return NULL;
            }

            if( blok.slogovi[i].sifraLeta == sifraLeta ){
                // Ako smo pronasli slog koji je korisnik uneo
                // vraticemo ga u return, ali cemo pre toga sa malloc kreirati memoriju za njega.

                SLOG* slog = (SLOG*)malloc(sizeof(SLOG));
                memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
                return slog;
            }

        }


    }
        return NULL;

}

void dodajSlog(FILE *fajl, SLOG *slog){
    // Provera da li imamo otvorenu datoteku
    if(fajl == NULL){
        printf("Datoteka nije otvorena. \n");
        return;
    }

    SLOG* slogProvera = pronadjiSlog(fajl,slog->sifraLeta);
    // Prvo proveramo da li postoji taj slog vec u datoteci
    if(slogProvera != NULL){
        printf("Slog sa istom sifrom leta vec postoji u datoteci. \n");
        return;
    }

    BLOK blok;
    fseek(fajl, -sizeof(BLOK), SEEK_END);
    fread(&blok, sizeof(BLOK), 1, fajl);

    // Prolazimo kroz ceo blok i poredimo da li smo dosli do kraja datoteke
    int i;
    for(i=0; i<FBLOKIRANJA; i++){
        if( blok.slogovi[i].sifraLeta == OZNAKA_KRAJA_DATOTEKE ){
            // Ako jesmo dosli do kraja, onda umesto znaka kraja datoteke cemo upisati nas slog
            memcpy(&blok.slogovi[i], slog, sizeof(SLOG));
            break;
        }
    }
    // Sad se pomeramo za jedno mesto unapred, jer tu hocemo da upisemo oznaku kraja datoteke.
    i++;
    // Ako imamo jos mesta u bloku, upisacemo tu oznakuKD
    // Ako nemamo vise mesta u bloku, napravicemo novBlok u koji cemo na pocetku upisati oznakuKD
    // Znaci zapisacemo je u novBlok.slogovi[0].sifraLeta
    if( i < FBLOKIRANJA){

        blok.slogovi[i].sifraLeta = OZNAKA_KRAJA_DATOTEKE;

        // Vratimo samo blok u datoteku.
        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK), 1, fajl);
        fflush(fajl);

    }else{
        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK), 1, fajl);

        BLOK noviBlok;
        noviBlok.slogovi[0].sifraLeta = OZNAKA_KRAJA_DATOTEKE;
        fwrite(&noviBlok, sizeof(BLOK), 1, fajl);
    }
    if (ferror(fajl)){
        printf("[Error] Greska pri upisu u fajl. \n");
    }else{
        printf("Slog je uspesno upisan u fajl. \n");
    }


}

void ispisiSlog(SLOG* slog){
    printf("\t%d\t\t%02d-%02d-%02d\t%02d:%02d\t%6s\t\t%d\t%3s\t\t%3s",
           slog->sifraLeta,
           slog->datum->godina,
           slog->datum->mesec,
           slog->datum->dan,
           slog->datum->sati,
           slog->datum->minuti,
           slog->tipAviona,
           slog->brojPutnika,
           slog->kodAerodromaPolaska,
           slog->kodAerodromaDolaska );

}

void ispisiSveSlogove(FILE *fajl){
    if(fajl == NULL){
        printf("Datoteka nije otvorena. \n");
        return;
    }
    fseek(fajl, 0, SEEK_SET);
    BLOK blok;
    int rbBloka = 0;
    printf("Blok\tSlog\tSifra_leta\tDatum\t\tVreme\tTip\tBroj_putnika\tKOD_polaska\tKOD_dolaska\n");
    while(fread(&blok, sizeof(BLOK),1,fajl)){
        for(int i=0; i<FBLOKIRANJA; i++){
            if( blok.slogovi[i].sifraLeta == OZNAKA_KRAJA_DATOTEKE ){
                printf("%d\t%d\n", rbBloka, i);
                break;
            }

            else{
                printf("%d\t%d ", rbBloka, i);
                ispisiSlog(&blok.slogovi[i]);
                printf("\n");
            }

        }
        rbBloka++;

    }

}


void obrisiSlogFizicki(FILE *fajl, int sifraLeta){
        // Proveramo da li postoji slog koji treba da se brise
        SLOG* slog = pronadjiSlog(fajl, sifraLeta);
        if( slog == NULL){
            printf("Ne postoji trazeni slog \n");
            return;
        }
        // Ako postoji, pomericemo sve slogove ispred njega za jedno mesto u nazad.

        BLOK blok, naredniBlok;
        int sifraLetaZaBrisanje;
        sifraLetaZaBrisanje = sifraLeta;

        fseek(fajl, 0, SEEK_SET);
        while(fread(&blok, 1, sizeof(BLOK), fajl)){
                for(int i=0; i<FBLOKIRANJA; i++){
                    if( blok.slogovi[i].sifraLeta ==  OZNAKA_KRAJA_DATOTEKE ){
                        if ( i==0 ){
                            // Ako u poslednjem bloku smo imali kao prvi slog OznakuKD
                            // Pa kad sve pomerimo za jedno mesto u levo, taj nam vise ne treba
                            fseek(fajl, -sizeof(BLOK), SEEK_END);
                            long bytesToKeep = ftell(fajl);
                            ftruncate(fileno(fajl), bytesToKeep);
                            fflush(fajl); // refreshamo promene
                        }
                        printf("Slog je fizicki obrisan\n");
                        return;
                    }
                    if( blok.slogovi[i].sifraLeta == sifraLetaZaBrisanje ){
                        for(int j=i+1; j<FBLOKIRANJA; j++){
                                // Povlacimo ih sve za jedno mesto u levo
                            memcpy(  &(blok.slogovi[j-1]),  &(blok.slogovi[j]), sizeof(SLOG));
                        }
                         // naredniBlok ucitavamo
                        int procitao = fread(&naredniBlok, sizeof(BLOK), 1, fajl);
                        // Ako ima , uradicemo fseek, da vratimo poziciju na prethodni, tj trenutni blok
                        // Uzeti njegov prvi slog i staviti na poslednje mesto trenutnog
                        if (procitao)
                        {
                          fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                          memcpy(&(blok.slogovi[FBLOKIRANJA-1]), &(naredniBlok.slogovi[0]), sizeof(SLOG));
                          sifraLetaZaBrisanje = naredniBlok.slogovi[0].sifraLeta;
                        }

                        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                        fwrite(&blok, sizeof(BLOK), 1, fajl);
                        fflush(fajl);

                        if (!procitao)
                        {
                          printf("Slog je fizicki obrisan. \n");
                          return;
                        }

                        break;
                    }
                }

        }




}





 // Pronaci i ispisati prosecan broj putnika za zadati tip aviona
int prosecanBrojPutnika(FILE* fajl, char* tip_aviona){
    if(fajl == NULL){
        printf("[Error] Greska kod fajla, nije otvoren ili ne postoji. \n");
    }

    int broj_aviona = 0; // tip_aviona,  mislim na broj slogova sa istim tipom aviona
    int broj_putnika= 0; // Ukupan broj putnika za tip_aviona, ukupna vrednost za svaki slog koji ima tip aviona = tip_aviona
	BLOK blok;

	fseek(fajl,0,SEEK_SET);
	while(fread(&blok,sizeof(BLOK),1,fajl)){
        for(int i=0; i<FBLOKIRANJA; i++){
            if(strcmp(blok.slogovi[i].tipAviona, tip_aviona)==0){
                broj_aviona++;
                broj_putnika += blok.slogovi[i].brojPutnika;
            }
        }

	}

    if(broj_aviona == 0)
        return 0;


    int rezultat = broj_putnika / broj_aviona;
	return rezultat;


}


int* dynamic_array_append(int* array, int size, int value)
{
    int new_size = size + 1;

    //kada dodajemo novi element realociramo nas trenutni array
    //na adresu koja sada ima velicinu novog size-a
    //i onda na zadnje mesto ubacimo novi value-element niza
    int* new_addr = (int *) realloc(array, new_size * (int)sizeof(int));

    if (new_addr == NULL) {
        printf("[Error] unable to realloc memory \n");
        return NULL;
    }

    new_addr[size] = value;
    return new_addr;
}

// Najmanji broj putnika zabelezen u martu mesecu 2020 godine = Y ,recimo
// Ova funkcija brise sve ostale letove koji imaju manji broj putnika od Y.
void najmanjiBrojPutnika(FILE* fajl){
    if(fajl == NULL){
        printf("[Error] Greska kod fajla, verovatno nije otvoren! \n");
    }

    BLOK blok;
    int najmanji_broj_putnika = -1;

    fseek(fajl,0,SEEK_SET);
    while(fread(&blok, sizeof(BLOK), 1, fajl)){
        for(int i=0; i<FBLOKIRANJA; i++){
            if(blok.slogovi[i].datum->mesec == 3 && blok.slogovi[i].datum->godina == 2020){
                if(najmanji_broj_putnika == -1)
                    najmanji_broj_putnika = blok.slogovi[i].brojPutnika;

                if(blok.slogovi[i].brojPutnika < najmanji_broj_putnika)
                    najmanji_broj_putnika = blok.slogovi[i].brojPutnika;

            }
        }
    }

    int size=0, *array = NULL;

    // Druga while petlja namenjena za punjenje niza za brisanje
    fseek(fajl,0,SEEK_SET);
    while(fread(&blok, sizeof(BLOK), 1, fajl)){
        for(int i=0; i<FBLOKIRANJA; i++){
            if(blok.slogovi[i].brojPutnika < najmanji_broj_putnika){
                array = dynamic_array_append(array, size, blok.slogovi[i].sifraLeta);
                size++;
            }
        }
    }
    // Sad nam ostaje da jos obrisemo taj niz
    for (int i=0 ; i<size ; i++)
    {
        obrisiSlogFizicki(fajl, array[i]);
    }


}


Map* dynamic_array_map_append(Map* array, int size, Map value)
{
    int new_size = size + 1;

    //kada dodajemo novi element realociramo nas trenutni array
    //na adresu koja sada ima velicinu novog size-a
    //i onda na zadnje mesto ubacimo novi value-element niza
    Map* new_addr = (Map *) realloc(array, new_size * (int)sizeof(Map));

    if (new_addr == NULL) {
        printf("[Error] unable to realloc memory \n");
        return NULL;
    }

    new_addr[size] = value;

    return new_addr;
}

SLOG* dynamic_array_slog_append(SLOG* array, int size, SLOG value)
{
    int new_size = size + 1;

    //kada dodajemo novi element realociramo nas trenutni array
    //na adresu koja sada ima velicinu novog size-a
    //i onda na zadnje mesto ubacimo novi value-element niza
    SLOG* new_addr = (SLOG *) realloc(array, new_size * (int)sizeof(SLOG));

    if (new_addr == NULL) {
        printf("[Error] unable to realloc memory \n");
        return NULL;
    }

    new_addr[size] = value;
    return new_addr;
}


// Za svaki tip aviona prikazati letove koji su obavljeni u 2021. godini.
void ispisiPoTipuAviona(FILE* fajl){
    if(fajl == NULL){
        printf("[Error] Greska kod fajla, verovatno nije otvoren. \n");
    }


    Map* tipoviAviona2021 = NULL;

    int mapSize = 0;
    BLOK blok;

    fseek(fajl,0, SEEK_SET);
    while(fread(&blok, sizeof(BLOK),1, fajl)){
        for(int i=0; i<FBLOKIRANJA; i++){
            if(blok.slogovi[i].datum->godina == 2021){

                int tipAvionaVecPostojiUMapi = 0;

                for (int j=0 ; j<mapSize; j++){
                    if(strcmp(tipoviAviona2021[j].tipAviona, blok.slogovi[i].tipAviona) == 0)
                    {
                        tipoviAviona2021[j].slogovi = dynamic_array_slog_append(tipoviAviona2021[j].slogovi, tipoviAviona2021[j].sizeSlogovi, blok.slogovi[i]);

                        tipoviAviona2021[j].sizeSlogovi++;
                        tipAvionaVecPostojiUMapi = 1;

                    }
                }


                if(!tipAvionaVecPostojiUMapi){

                    Map tipAvionaMap;

                    strcpy(tipAvionaMap.tipAviona, blok.slogovi[i].tipAviona);

                    tipAvionaMap.sizeSlogovi=0;
                    tipAvionaMap.slogovi = NULL;

                    tipoviAviona2021 = dynamic_array_map_append(tipoviAviona2021, mapSize, tipAvionaMap);
                    mapSize++;

                    //pa dodaj slog
                    //mapSize-1 poslednje dodati element
                    tipoviAviona2021[mapSize-1].slogovi = dynamic_array_slog_append(tipoviAviona2021[i].slogovi, tipoviAviona2021[i].sizeSlogovi, blok.slogovi[i]);
                    tipoviAviona2021[mapSize-1].sizeSlogovi++;

                }

            }
        }
    }

    for(int i=0; i < mapSize ; i++)
    {
        printf("Letovi za tip aviona %s su:\n", tipoviAviona2021[i].tipAviona);

        for(int j=0; j < tipoviAviona2021[i].sizeSlogovi; j++)
        {
            printf("Let broj [%d] \n", tipoviAviona2021[i].slogovi[j].sifraLeta);
        }

        printf("************************************\n");
    }


}








