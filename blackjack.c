#include <stdio.h>

// * b 
// ^
// |- Mit dem Stern greift man bei einem Pointer auf den Wert hinter dem Pointer zu

// & a
// ^
// |- Erstellt einen Verweiß für die Variable a

// Wenn ein Point ohne * weitergegeben wird, wird nur der Verweiß weitergegeben

typedef struct Karte {
    char* symbol;
    char* farbe;
    char* wertname;
    int wert;
} Karte;

typedef struct Kartentyp {
    char* symbol;
    char* farbe;
} Kartentyp;

typedef struct Kartenwert {
    char* wertname;
    int wert;
} Kartenwert;

int initdeck(Kartentyp *kartentypen, int anzahlTypen, Kartenwert *kartenwerte, int anzahlWerte, Karte *spieldeck);
int mischen(Karte *spieldeck, int anzahlKarten);
int kartenTauschen(Karte *karteX, Karte *karteY);
int spielen(Karte *spieldeck, int *pointerToAnzahlKarten, int *SchwierigkeitsgradSpieler1, int *SchwierigkeitsgradSpieler2, int *pointerTospieler1Won, int *pointerTospieler2Won, int *pointerToSpielBeendet);
int karteZiehen(int *spielerstand, Karte *spieldeck, int *pointerToAnzahlKarten, int *SchwierigkeitsgradSpieler);
int zug(int *Spieler1, int *Spieler2, Karte *spieldeck, int *pointerToAnzahlKarten, int *SchwierigkeitsgradSpieler1, int *SchwierigkeitsgradSpieler2, int *pointerTospieler1Won, int *pointerTospieler2Won, int *pointerToSpielBeendet);
int willZiehen(int *spielerstand, int *SchwierigkeitsgradSpieler);

int main() {
    Kartentyp kartentypen[4] = { 
        {"Kreuz","Schwarz"},
        {"Piek", "Schwarz" },
        {"Herz", "Rot"},
        {"Karo", "Rot"}
    };
    int anzahlTypen = sizeof(kartentypen)/sizeof(kartentypen[0]);

    Kartenwert kartenwerte[13] = {
        {"Zwei", 2},
        {"Drei", 3},
        {"Vier", 4},
        {"Fünf", 5},
        {"Sechs", 6},
        {"Sieben", 7},
        {"Acht", 8},
        {"Neun", 9},
        {"Zehn", 10},
        {"Bube", 10},
        {"Dame", 10},
        {"König", 10},
        {"Ass", 11}        
    };
    int anzahlWerte = sizeof(kartenwerte)/sizeof(kartenwerte[0]);

    // Wähle Schwierigkeitsgrad
    int SchwierigkeitsgradSpieler1 = 0;
    int * pointerToSchwierigkeitsgradSpieler1 = &SchwierigkeitsgradSpieler1;

    while(SchwierigkeitsgradSpieler1<1 || SchwierigkeitsgradSpieler1>5 || SchwierigkeitsgradSpieler1 == 0) {
        printf("Welchen Schwierigkeitsgrad soll Spieler 1 haben ? [1...5]\n");
        scanf("%i", &SchwierigkeitsgradSpieler1);
    }
    printf("Spieler 1 hat den Schwierigkeitsgrad %i\n", SchwierigkeitsgradSpieler1);

    int SchwierigkeitsgradSpieler2 = 0;
    int * pointerToSchwierigkeitsgradSpieler2 = &SchwierigkeitsgradSpieler2;

    while(SchwierigkeitsgradSpieler2<1 || SchwierigkeitsgradSpieler2>5 || SchwierigkeitsgradSpieler2 == 0) {
        printf("Welchen Schwierigkeitsgrad soll Spieler 2 haben ? [1...5]\n");
        scanf("%i", &SchwierigkeitsgradSpieler2);
    }
    printf("Spieler 2 hat den Schwierigkeitsgrad %i\n", SchwierigkeitsgradSpieler2);

    Karte spieldeck[52];
    int anzahlKarten = sizeof(spieldeck)/sizeof(spieldeck[0]);
    int * pointerToAnzahlKarten = &anzahlKarten;

    // Initialisiere Spieldeck
    initdeck(kartentypen, anzahlTypen, kartenwerte, anzahlWerte, spieldeck);

    int i = 0;
    int spielbeendet = 0;
    int * pointerToSpielBeendet = &spielbeendet;

    int spieler1Won = 0;
    int * pointerTospieler1Won = &spieler1Won;
    int spieler2Won = 0;
    int * pointerTospieler2Won = &spieler2Won;

    for(i;i<1000;i++){
        spielbeendet = 0;
        *pointerToAnzahlKarten = 52;
        mischen(spieldeck, anzahlKarten);

        spielen(spieldeck, pointerToAnzahlKarten, pointerToSchwierigkeitsgradSpieler1, pointerToSchwierigkeitsgradSpieler2, pointerTospieler1Won, pointerTospieler2Won, pointerToSpielBeendet);

        while(spielbeendet == 0){}
    }

    printf("Gewonnene Spiele von Spieler 1: %i\n", spieler1Won);
    printf("Gewonnene Spiele von Spieler 2: %i\n", spieler2Won);
}

int initdeck(Kartentyp *kartentypen, int anzahlTypen, Kartenwert *kartenwerte, int anzahlWerte, Karte *spieldeck) {
    int i = 0;
    for(i;i<anzahlTypen;i++)
    {
        int j = 0;
        for(j;j<anzahlWerte;j++){
            // DEGBUG: printf("Erstelle Karte %s mit Farbe %s und dem Wert %s\n", kartentypen[i].symbol, kartentypen[i].farbe, kartenwerte[j].wertname);
            spieldeck[(i*anzahlWerte)+j].symbol = kartentypen[i].symbol;
            spieldeck[(i*anzahlWerte)+j].farbe = kartentypen[i].farbe;
            spieldeck[(i*anzahlWerte)+j].wertname = kartenwerte[j].wertname;
            spieldeck[(i*anzahlWerte)+j].wert = kartenwerte[j].wert;
        }
    }
    return 0;
}

int mischen(Karte *spieldeck, int anzahlKarten) {
    srand(time(NULL));
    
    int i = 0;
    int mischZyklen = 4;
    int a,b;
    Karte *karteX, *karteY;
    for(i;i<anzahlKarten*mischZyklen;i++){
        a = rand()%anzahlKarten;
        b = rand()%anzahlKarten;
        // DEGBUG: printf("Verschiebe Karte von %i nach %i\n", a,b);

        karteX = &spieldeck[a];
        karteY = &spieldeck[b];
        kartenTauschen(karteX, karteY);
    }
}

int kartenTauschen(Karte *karteX, Karte *karteY) {
    Karte temp;
    temp = *karteX;
    *karteX = *karteY;
    *karteY = temp;
    return 0;
}

int spielen(Karte *spieldeck, int *pointerToAnzahlKarten, int *SchwierigkeitsgradSpieler1, int *SchwierigkeitsgradSpieler2,  int *pointerTospieler1Won, int *pointerTospieler2Won, int *pointerToSpielBeendet) {
    int spieler1 = 0;
    int * pointertoSpieler1 = &spieler1;
    int spieler2 = 0;
    int * pointertoSpieler2 = &spieler2;

    // Spieler 1 bekommt erste Karte
    *pointertoSpieler1 = karteZiehen(pointertoSpieler1, spieldeck, pointerToAnzahlKarten, SchwierigkeitsgradSpieler1);
    // Spieler 2 bekommt erste Karte
    *pointertoSpieler2 = karteZiehen(pointertoSpieler2, spieldeck, pointerToAnzahlKarten, SchwierigkeitsgradSpieler2);

    printf("Spielstand Spieler 1: %i\n", spieler1);
    printf("Spielstand Spieler 2: %i\n", spieler2);

    zug(pointertoSpieler1, pointertoSpieler2, spieldeck, pointerToAnzahlKarten, SchwierigkeitsgradSpieler1, SchwierigkeitsgradSpieler2, pointerTospieler1Won, pointerTospieler2Won, pointerToSpielBeendet);
}

int zug(int *pointertoSpieler1, int *pointertoSpieler2, Karte *spieldeck, int *pointerToAnzahlKarten, int *SchwierigkeitsgradSpieler1, int *SchwierigkeitsgradSpieler2, int *pointerTospieler1Won, int *pointerTospieler2Won, int *pointerToSpielBeendet) {    
    // Spieler 1 bekommt erste Karte
    int kartenwertSpieler1 = karteZiehen(pointertoSpieler1, spieldeck, pointerToAnzahlKarten, SchwierigkeitsgradSpieler1);
    if(kartenwertSpieler1 != 0)
    {
        // DEBUG: printf("%i\n", kartenwertSpieler1);
        *pointertoSpieler1 += kartenwertSpieler1;
    }

    // Spieler 2 bekommt erste Karte
    int kartenwertSpieler2 = karteZiehen(pointertoSpieler2, spieldeck, pointerToAnzahlKarten, SchwierigkeitsgradSpieler2);
    if(kartenwertSpieler2 != 0)
    {
        // DEBUG: printf("%i\n", kartenwertSpieler2);
        *pointertoSpieler2 += kartenwertSpieler2;
    }

    printf("Spielstand Spieler 1: %i\n", *pointertoSpieler1);
    printf("Spielstand Spieler 2: %i\n", *pointertoSpieler2);

    if((kartenwertSpieler1 != 0 && kartenwertSpieler2 != 0) && *pointertoSpieler1 < 21 && *pointertoSpieler2 < 21)    
    {
        zug(pointertoSpieler1, pointertoSpieler2, spieldeck, pointerToAnzahlKarten, SchwierigkeitsgradSpieler1, SchwierigkeitsgradSpieler2, pointerTospieler1Won, pointerTospieler2Won, pointerToSpielBeendet);
    }
    else {
        *pointerToSpielBeendet = 1;

        if(*pointertoSpieler1 > 21 && *pointertoSpieler2 <= 21){
            printf("Spieler 2 hat gewonnen ! Spieler 1 hat überworfen\n");
            *pointerTospieler2Won+=1;
        }
        else if(*pointertoSpieler2 > 21 && *pointertoSpieler1 <= 21){
            printf("Spieler 1 hat gewonnen ! Spieler 2 hat überworfen\n");
            *pointerTospieler1Won+=1;
        }
        else if(*pointertoSpieler1 > *pointertoSpieler2 && *pointertoSpieler1 <= 21)
        {
            printf("Spieler 1 hat gewonnen !\n");
            *pointerTospieler1Won+=1;
        }
        else if(*pointertoSpieler2 > *pointertoSpieler1 && *pointertoSpieler2 <= 21)
        {            
            printf("Spieler 2 hat gewonnen !\n");
            *pointerTospieler2Won+=1;
        }
        else if(*pointertoSpieler2 == *pointertoSpieler1 && *pointertoSpieler1 <= 21) {
            printf("Gleichstand !\n");
        } 
        else {
            printf("Beide haben verloren !\n");
        }
    }
}

int karteZiehen(int *spielerstand, Karte *spieldeck, int *pointerToAnzahlKarten, int *SchwierigkeitsgradSpieler) {
    /* To-Do Letztes Element aus dem Array Spieldeck löschen */
    if(*spielerstand == 0){
        *pointerToAnzahlKarten-=1;
        return spieldeck[*pointerToAnzahlKarten].wert;
    }
    else {
        if(willZiehen(spielerstand, SchwierigkeitsgradSpieler) == 1){
            *pointerToAnzahlKarten-=1;
            // DEBUG: printf("Ausgegebener Kartenwert: %i\n", spieldeck[*pointerToAnzahlKarten].wert);
            return spieldeck[*pointerToAnzahlKarten].wert;
        }
        else {
            // Spieler will nicht ziehen
            // DEBUG: printf("Will nicht ziehen !\n");
            return 0;
        }
    }
}

int willZiehen(int *spielerstand, int *SchwierigkeitsgradSpieler) {
    switch(*SchwierigkeitsgradSpieler){
        case 1: {
            if(*spielerstand >= 15)
            {
                return 0;
            }
            else
            {
                return 1;
            }
            break;
        }
        case 2: {
            if(*spielerstand >= 16)
            {
                return 0;
            }
            else
            {
                return 1;
            }
            break;
        }
        case 3: {
            if(*spielerstand >= 17)
            {
                return 0;
            }
            else
            {
                return 1;
            }
            break;
        }
        case 4: {            
            if(*spielerstand >= 18)
            {
                return 0;
            }
            else
            {
                return 1;
            }
            break;
        }
        case 5: {            
            if(*spielerstand >= 19)
            {
                return 0;
            }
            else
            {
                return 1;
            }
            break;
        }
    }
}