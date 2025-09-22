#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Domyślny rozmiar kostki.
#ifndef N
#define N 5
#endif
#define SCIANY 6
#define CYKL 4
#define KONIEC '.'
#define POLPELNY '"'
#define LEWO '\''

// Początkowe wypełnianie ścian kostki cyframi symbolizującymi kolory.
void koloruj(int sciany[SCIANY][N][N]) {
    for (int k = 0; k < SCIANY; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                sciany[k][i][j] = k;
            }
        }
    }
}

// Dla podanej strony ściany zwracana jest odpowiadająca jej cyfra.
int konwersjaStrony(int litera) {
    if (litera == 'u') {
        return 0;
    }
    else if (litera == 'l') {
        return 1;
    }
    else if (litera == 'f') {
        return 2;
    }
    else if (litera == 'r') {
        return 3;
    }
    else if (litera == 'b') {
        return 4;
    }
    else {
        return 5;
    }
}

// Wypisywanie ściany odpowiednie tylko dla ścian górnej i dolnej.
void wypiszGoraDol(int sciana[N][N]) {
    for (int i = 0; i < N; i++) {
        printf("\n");
        for (int k = 0; k < N + 1; k++) {
            printf(" ");
        }
        for (int j = 0; j < N; j++) {
            printf("%d", sciana[i][j]);
        }
    }
}

void wypiszKostke(int sciany[SCIANY][N][N]) {
    // Wypisywanie górnej ściany.
    wypiszGoraDol(sciany[0]);
    // Iterowanie się wierszami kolejnych ścian.
    for (int i = 0; i < N; i++) {
        printf("\n");
        for (int k = 0; k < SCIANY; k++) {
            if (k != 0 && k!= 5) {
                for (int j = 0; j < N; j++) {
                    printf("%d", sciany[k][i][j]);
                }
                if (k != 4) {
                    printf("|");
                }
            }
        }
    }
    // Wypisywanie dolnej ściany.
    wypiszGoraDol(sciany[5]);
    printf("\n");
}

void obrotSciany(int sciana[N][N], int kat) {
    // Kat przyjmuje następujące wartości dla odpowiadających stopni:
    // -1 lub 3 dla -90, 1 dla 90, 2 dla 180, 0 lub 4 dla 0.
    // Obrót o 0 stopni nic nie modyfikuje.
    if (kat == 0 || kat == 4) {
        return;
    }
    // Tworzenie pomocniczej kopii obracanej ściany.
    int pom[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pom[i][j] = sciana[i][j];
        }
    }
    // Obrót w lewo o 90 stopni.
    if (kat == -1 || kat == 3) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                sciana[i][j] = pom[j][N - 1 - i];
            }
        }
    }
    // Obrót w prawo o 90 stopni.
    else if (kat == 1) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                sciana[i][j] = pom[N - 1 - j][i];
            }
        }
    }
    // Obrót o 180 stopni.
    else {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                sciana[i][j] = pom[N - 1 - i][N - 1 - j];
            }
        }
    }
}

// Dla podanej cyfry strony ściany zwracana jest cyfra ściany przeciwnej. 
int przeciwna(int sciana) {
    if (sciana == 0 || sciana == 5) {
        return 5 - sciana;
    }
    else if (sciana % 2 == 1) {
        return 4 - sciana;
    }
    else {
        return 6 - sciana;
    }
}

// Kopiowanie tablicy wiersz do tablicy pom.
void kopiuj(int wiersz[N], int pom[N]) {
    for (int i = 0; i < N; i++) {
        pom[i] = wiersz[i];
    }
}

// Przesuwanie cykliczne ileWarst wartw o podany kąt w określonym cyklu ścian.
// Założenie: ściany są obrócone tak, że wystarczy przesunąć wiersze.
void przesun(int kat, int ileWarstw, int sciany[SCIANY][N][N], int cykl[CYKL]) {
    // Pomocnicza tablica na trzymanie wierszy pierwszej ściany cyklu.
    int pom[N];
    for (int k = 0; k < abs(kat); k++) {
        for (int i = 0; i < ileWarstw; i++) {
            kopiuj(sciany[cykl[0]][i], pom);
            for (int j = 0; j < CYKL - 1; j++) {
                kopiuj(sciany[cykl[j + 1]][i], sciany[cykl[j]][i]);
            }
            kopiuj(pom, sciany[cykl[CYKL - 1]][i]);
        }
    }
}

void rozkaz(int sciany[SCIANY][N][N], int sciana, int ileWarstw, int kat,
            int cykle[SCIANY][CYKL], int obroty[SCIANY][CYKL]) {
    // Zmiana orientacji ściany, którą obracamy, zgodnie z kątem. 
    obrotSciany(sciany[sciana], kat);
    // Zmiana orientacji ściany przeciwnej do tej, którą obracamy,
    // gdy liczba obracanych warstw jest równa rozmiarowi kostki.
    if (ileWarstw == N) {
        if (kat == 2) obrotSciany(sciany[przeciwna(sciana)], kat);
        else obrotSciany(sciany[przeciwna(sciana)], kat*(-1));
    }
    // Ściany, których warstwy cyklicznie przesuwamy dla danego obrotu.
    int cykl[CYKL];
    for (int i = 0; i < CYKL; i++) {
        if (kat == -1) {
            // Korzystanie z cyklu ściany przeciwnej, gdy obrót jest w lewo.
            cykl[i] = cykle[przeciwna(sciana)][i];
        }
        else {
            cykl[i] = cykle[sciana][i];
        }
    }
    // Obrócenie ścian do potrzebnej w funkcji przesun orientacji.
    for (int i = 0; i < CYKL; i++) {
        obrotSciany(sciany[cykle[sciana][i]], obroty[sciana][i]);
    }
    // Cykliczne przesunięcie wierszy ścian.
    przesun(kat, ileWarstw, sciany, cykl);
    // Obrócenie ścian z powrotem do poprawnej orientacji.
    for (int i = 0; i < CYKL; i++) {
        // (4 - obrót) zagwarantuje nam obrót odwrotny.
        obrotSciany(sciany[cykle[sciana][i]], 4 - obroty[sciana][i]);
    }
}

// Wczytywanie danych i kolejno realizowanie ruchów kostki zgodnie z danymi.
void obsluzRozkazy(int sciany[SCIANY][N][N], int cykle[SCIANY][CYKL],
                   int obroty[SCIANY][CYKL]) {
    int znak = 0;
    int ileWarstw = 0;
    int sciana = 0;
    int kat = 0;
    // Wczytywanie do momentu otrzymania znaku oznaczającego koniec danych.
    while(znak != KONIEC) {
        // Podejrzenie znaku i zapisanie go w zmiennej znak. 
        znak = getchar();
	    ungetc(znak, stdin);
	    if (znak == KONIEC) {
		    znak = getchar();
		    continue;
	    }
        // Wypisanie całej kostki w przypadku znaku nowej linii.
        else if (znak == '\n') {
            znak = getchar();
            wypiszKostke(sciany);
        }
        // Wczytanie całej liczby jako liczby warstw.
        else {
            znak = getchar();
            ileWarstw = 0;
            while (isdigit(znak)) {
                if (isdigit(znak)) {
                    ileWarstw = ileWarstw * 10 + znak - '0';
                }
                znak = getchar();
            }
            // Domyślna liczba warstw, w przypadku niepodania, ustawiona na 1.
            if (ileWarstw == 0) {
                ileWarstw = 1;
            }
            // Nadmiarowy znak wczytany przy liczbie warstw musi być literą
            // symbolizującą stronę ściany, którą obracamy.
            sciana = znak;
            znak = getchar();
            ungetc(znak, stdin);
            if (znak == POLPELNY || znak == LEWO) {
                // Wczytanie kąta.
                znak = getchar();
                if (znak == POLPELNY) {
                    kat = 2;
                }
                else if (znak == LEWO) {
                    kat = -1;
                }
            }
            else {
                // Domyślna wartość kąta to 1 (90 stopni w prawo).
                kat = 1;
            }
            // Wykonanie obrotu zgodnego ze wszystkimi danymi.
            rozkaz(sciany, konwersjaStrony(sciana),
                   ileWarstw, kat, cykle, obroty);
	    }
    }
}

int main() {
    // Tablica sześciu tablic odpowiadających sześciu ścianom kostki N x N.
    int sciany[SCIANY][N][N];
    // Wypełniamy ściany odpowiadającymi im liczbami.
    koloruj(sciany);
    // Tablica zawierająca podane w odpowiedniej kolejności dla każdej ściany
    // sąsiadujące ściany, których warstwy będą przesuwane, przy jej obrocie.
    int cykle[SCIANY][CYKL] = {
        {1, 2, 3, 4},
        {4, 5, 2, 0},
        {1, 5, 3, 0},
        {0, 2, 5, 4},
        {0, 3, 5, 1},
        {4, 3, 2, 1}
    };
    // Tablica zawierająca liczby obrotów odpowiadające ścianom z tablicy cykle 
    // potrzebne do uzyskania jednej konwencji orientacji ściany.
    int obroty[SCIANY][CYKL] = {
        {0, 0, 0, 0},
        {3, 1, 1, 1},
        {3, 0, 1, 2},
        {3, 3, 3, 1},
        {0, 3, 2, 1},
        {2, 2, 2, 2}
    };
    obsluzRozkazy(sciany, cykle, obroty);
    return 0;
}