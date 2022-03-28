#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "operacije.c"



int main()
{
    int running = 1;
    int userInput;

    FILE* fajl = NULL;

    while(running){
        printf("Odaberite opciju: \n");
        printf("1. Kreiranje datoteke \n");
        printf("2. Otvaranje datoteke \n");
        printf("3. Unos novog sloga \n");
        printf("4. Ispisi sve slogove \n");
        printf("5. Obrisi fizicki slog \n");
        printf("6. Izracunaj prosecan broj putnika \n");
        printf("7. Obrisi odredjene letove \n");
        printf("8. Ispisi letove po tipu u 2021g \n");
        printf("0 - Izlaz\n");



        if(fajl ==NULL){
            printf("\n**Trenutno ni jedna datoteka nije otvorena** \n");
        }
        printf("Vasa opcija: ");
        scanf("%d", &userInput);
        getc(stdin);

        switch(userInput){
            case 1:
                {
                char filename[20];
                printf("Unesite ime datoteke koju zelite da kreirate: ");
                scanf("%s", filename);
                kreirajDatoteku(filename);
                printf("\n\n");
                break;
                }

            case 2:
                {
                char filename[20];
                printf("Unesite ime datoteke koju zelite da otvorite: ");
                scanf("%s", &filename[0]);
                fajl = otvoriDatoteku(filename);
                printf("\n\n");
                break;
                }

            case 3:
                {
                    SLOG slog;
					printf("Sifra leta: ");
					scanf("%d", &slog.sifraLeta);

                    printf("Datum i vreme dolaska (YYYY-mm-dd HH:mm): ");
                    scanf("%d-%d-%d %d:%d",
                    &slog.datum->godina,
                    &slog.datum->mesec,
                    &slog.datum->dan,
                    &slog.datum->sati,
                    &slog.datum->minuti);

                    printf("Tip aviona (Max 7 karaktera) : ");
                    scanf("%s", slog.tipAviona);

                    printf("Broj putnika:  ");
					scanf("%d", &slog.brojPutnika);

                    printf("Kod Aerodroma polaska (tacno 3 karaktera): ");
					scanf("%s", slog.kodAerodromaPolaska);

                    printf("Kod Aerodroma polaska (tacno 3 karaktera): ");
					scanf("%s", slog.kodAerodromaDolaska);

					dodajSlog(fajl, &slog);
					printf("\n");
					break;

                }
            case 4:
                {
                    ispisiSveSlogove(fajl);
					printf("\n");
					break;

                }
            case 5:
                {
                    int sifraLeta = 0;
					printf("Unesite sifru leta koji hocete da obriste: \n ");
					scanf("%d", &sifraLeta);
					obrisiSlogFizicki(fajl, sifraLeta);
					printf("\n");
					break;

                }
            case 6:
                {
                    char tip_aviona[7];
                    printf("Unesite tip aviona(max 7 karaktera):\n");
                    scanf("%s", &tip_aviona);
                    int rez = prosecanBrojPutnika(fajl, tip_aviona);
                    if(rez == 0){
                        printf("Ne postoji avion sa unesenim tipom \n\n\n");
                    }else{
                    printf("Prosecan tip putnika za tip aviona - %s", tip_aviona);
                    printf(" je: %d \n\n\n", rez);
                    }
                    break;
                }
            case 7:
                {
                    najmanjiBrojPutnika(fajl);
                    printf("Uspesno ste obrisali odredjene letove ! \n");
                    break;
                }
            case 8:
                {
                    ispisiPoTipuAviona(fajl);
                    printf("Uspesno ispisano \n");
                    break;
                }


            case 0:
                {
                running = 0;
                if(fajl != NULL)
                fclose(fajl);

                }


        }

    }






    return 0;
}
