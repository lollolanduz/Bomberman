//
// Created by Utente on 25/02/2026.
//
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include "Livello.h"
#include "Costanti.h"
#include "EnemyIntelligente.h"
#include "EnemyRandom.h"

Livello::Livello(int id) {
    idLivello = id;
    contatoreItems = 0;
    successivo = nullptr;
    precedente = nullptr;

    tempodiInizio = -1;
    tempoLivello = -1;

    //Serve a gestire il portale per concludere il livello
    Portale = false;

    //Inizialmente, la posizione salvata è lo spawn (1,1)
    player_save_x = 1;
    player_save_y = 1;

    for (int i = 0; i < 10; i++) {
        isBombActive[i] = false;
        bombTimer[i] = 0;
        bombRadius[i] = 1;
    }

    playerRadiusTimer = 0;

    //Matrice mutatori
    //Colonne: {Normale (1), Mura Dure (2), Carestia (3), Blackout (4)}
    int probabilita[5][4] = {
        {85, 15,  0,  0},  // Livello 1 (facile)
        {75, 15, 10,  0},  // Livello 2 (facile)
        {60, 20, 15,  5},  // Livello 3 (medio)
        {50, 20, 15, 15},  // Livello 4 (difficile)
        {40, 20, 20, 20}   // Livello 5 (caos)
    };

    //Guardia per assicurarsi di non sforare l'array
    int indiceLiv;
    if (idLivello > 5) {
        indiceLiv = 4;
    } else {
        indiceLiv = idLivello - 1;
    }

    //Tiro un dado da 0 a 99
    int randVal = rand() % 100;

    // Probabilità Pesata dei mutatori
    if (randVal < probabilita[indiceLiv][0]) {
        mutatore = 1; // Normale
    }
    else if (randVal < probabilita[indiceLiv][0] + probabilita[indiceLiv][1]) {
        mutatore = 2; // Mura Dure (Zona Blindata)
    }
    else if (randVal < probabilita[indiceLiv][0] + probabilita[indiceLiv][1] + probabilita[indiceLiv][2]) {
        mutatore = 3; // Carestia (Nessun Drop)
    }
    else {
        mutatore = 4; // Blackout
    }

    genera_griglia_vuota();
    genera_MuriFissi();
    genera_MuraDistruttibili();
    imposta_Spawn();

    if (idLivello >= 4) {
        genera_teletrasporto();
    }

    // --- GENERAZIONE POLIMORFICA DEI NEMICI ---
    contatoreNemici = 0;

    int numeroNemici = 2 + idLivello;
    for (int i = 0; i < numeroNemici; i++) {
        int temp_x, temp_y;
        // Trova una casella vuota casuale per il nemico
        do {
            temp_x = (rand() % (max_x - 2)) + 1;
            temp_y = (rand() % (max_y - 2)) + 1;
        } while (griglia[temp_y][temp_x] != ' ');

        if (i % 2 == 0) {
            nemici[contatoreNemici] = new EnemyRandom(temp_x, temp_y, 'X', 100, FRAME_NEMICO_X, COLORE_X);
        } else {
            nemici[contatoreNemici] = new EnemyRandom(temp_x, temp_y, 'Z', 150, FRAME_NEMICO_Z, COLORE_Z);
        }
        contatoreNemici++;
    }

    //Generazione del nemico Intelligente
    if (idLivello >= 4) {
        int numIntelligenti = idLivello - 3;
        for (int i = 0; i < numIntelligenti; i++) {
            int temp_x, temp_y;
            do {
                temp_x = (rand() % (max_x - 2)) + 1;
                temp_y = (rand() % (max_y - 2)) + 1;
            } while (griglia[temp_y][temp_x] != ' ' || temp_x < 15 && temp_y < 15);

            nemici[contatoreNemici] = new EnemyIntelligente(temp_x, temp_y, 'i', COLORE_I);
            contatoreNemici++;
        }
    }
}
Livello::~Livello() {
    // Distrugge tutti i nemici in un colpo solo, indipendentemente dal tipo!
    for (int i = 0; i < contatoreNemici; i++) {
        delete nemici[i];
    }
    for (int i = 0; i < contatoreItems; i++) {
        delete itemsATerra[i];
    }
}



void Livello::genera_griglia_vuota() {
    //Stampo lo spazio vuoto in tutta la griglia, per evitare
    //che sia formata esclusivamente di "spazzatura"
    for (int y=0; y<max_y; y++) {
        for (int x=0; x<max_x; x++) {
            griglia[y][x]= ' ';
        }
    }
}

void Livello::genera_MuriFissi() {
    //M sta per muro indistruttibilie
    //Ciclo for per generazione delle mura perimetrali
    for (int y=0; y < max_y; y++) {
        for (int x=0; x < max_x; x++) {
            if (x==0 || x == max_x - 1 || y == 0 || y == max_y - 1) {
                griglia[y][x] = 'M';
            }
        }
    }

    //Mura indistruttibili nella mappa (a scacchiera)
    if (idLivello <= 3) {
        for (int y=1; y< max_y; y++) {
            for (int x=1; x < max_x; x++) {
                if ( y%4 == 0 && x%4==0) {
                    griglia[y][x] = 'M';
                }
            }
        }
    } else if (idLivello >=4) {
        for (int y=1; y< max_y; y++) {
            for (int x=1; x < max_x; x++) {
                if (y%3 == 0 && x%3==0) {
                    griglia[y][x] = 'M';
                }
            }
        }
    }
}

void Livello::genera_MuraDistruttibili() {
    int wall_cap = 10 + 2*idLivello;

    if (wall_cap > 70) {
        wall_cap = 70;
    }

    //Calcolo probabilità delle mura DURE
    // L1 e L2: 0% , L3: 10% , L4: 20% , L5: 30%
    int prob_muro_duro = 0;
    if (idLivello >= 3) {
        prob_muro_duro = (idLivello - 2) * 10;
    }

    //D = Distruttibile Normale, H = Distruttibile Rinforzato
    for (int y=1; y < max_y - 1; y++) {
        for (int x=1; x < max_x - 1; x++) {
            int wall_rate= rand() % 100;
            if (wall_rate < wall_cap && griglia[y][x] != 'M') {
                // Mutatore 2: Zona blindata, forziamo tutte le mura a essere Dure!
                if (mutatore == 2) {
                    griglia[y][x] = 'H';
                } else {
                    // Altrimenti logica classica
                    int hard_rate = rand() % 100;
                    if (hard_rate < prob_muro_duro) {
                        griglia[y][x] = 'H';
                    } else {
                        griglia[y][x] = 'D';
                    }
                }
            }
        }
    }
}

void Livello::imposta_Spawn() {
    //S = spazio vitale per il giocatore
    griglia[1][1]= 'S';
    griglia[1][2]= 'S';
    griglia[2][1]= 'S';
}

void Livello::genera_teletrasporto() {
    bool teletrasporto_piazzato = false;

    while (!teletrasporto_piazzato) {

        //Coordinate casuali del primo teletrasporto
        int rand_y = (rand() % (max_y - 2)) + 1;
        int rand_x = (rand() % (max_x - 2)) + 1;

        //Coordinate del suo opposto
        int opp_y = max_y - 1 - rand_y;
        int opp_x = max_x - 1 - rand_x;

        //Guardia per garantire una distanza minima tra i portali
        //Calcolo distanza
        int distanza = std::abs(rand_x - opp_x) + std::abs(rand_y - opp_y);

        //Se sono a meno di 25 passi di distanza, saltiamo il resto del codice
        //e il ciclo while ricomincerà istantaneamente a cercare nuove coordinate
        if (distanza < 25) {
            continue;
        }

        //Verifico se sono spazi vuoti
        if (griglia[rand_y][rand_x] == ' ' && griglia[opp_y][opp_x] == ' ') {

            //Entrambi diversi dalla zona di spawn
            if (griglia[rand_y][rand_x] != 'S' && griglia[opp_y][opp_x] != 'S') {

                griglia[rand_y][rand_x] = 'T';
                griglia[opp_y][opp_x] = 'T';

                //Creare la safe zone per il teletrasporto
                if (griglia[rand_y+1][rand_x] == 'D' || griglia[rand_y+1][rand_x] == 'H') {
                    griglia[rand_y+1][rand_x] = ' ';
                }
                if (griglia[rand_y-1][rand_x] == 'D' || griglia[rand_y-1][rand_x] == 'H') {
                    griglia[rand_y-1][rand_x] = ' ';
                }
                if (griglia[rand_y][rand_x+1] == 'D' || griglia[rand_y][rand_x+1] == 'H') {
                    griglia[rand_y][rand_x+1] = ' ';
                }
                if (griglia[rand_y][rand_x-1] == 'D' || griglia[rand_y][rand_x-1] == 'H') {
                    griglia[rand_y][rand_x-1] = ' ';
                }

                //Creazione safe zone per l'opposto
                if (griglia[opp_y+1][opp_x] == 'D' || griglia[opp_y+1][opp_x] == 'H') {
                    griglia[opp_y+1][opp_x] = ' ';
                }
                if (griglia[opp_y-1][opp_x] == 'D' || griglia[opp_y-1][opp_x] == 'H') {
                    griglia[opp_y-1][opp_x] = ' ';
                }
                if (griglia[opp_y][opp_x+1] == 'D' || griglia[opp_y][opp_x+1] == 'H') {
                    griglia[opp_y][opp_x+1] = ' ';
                }
                if (griglia[opp_y][opp_x-1] == 'D' || griglia[opp_y][opp_x-1] == 'H') {
                    griglia[opp_y][opp_x-1] = ' ';
                }

                //Serve ad uscire dal while
                teletrasporto_piazzato = true;
            }
        }
    }
}

void Livello::gestisciTeletrasporto(int &giocatore_y, int &giocatore_x) {
    if (griglia[giocatore_y][giocatore_x] == 'T') {
        if (tempodiInizio == 0) {
            tempodiInizio = (int)std::time(nullptr);
        }
        else if (tempodiInizio > 0) {
            int tempoAttuale = (int)std::time(nullptr);
            if (tempoAttuale - tempodiInizio >= 2) {
                giocatore_y = max_y - 1 - giocatore_y;
                giocatore_x = max_x - 1 - giocatore_x;

                //Per non far ripartire il timer al prossimo giro
                tempodiInizio = -1;
            }
        }
    }
    else {
        // Appena il giocatore fa un passo fuori dalla 'T' reimposto tutto a 0
        tempodiInizio = 0;
    }
}

void Livello::apriPortaUscita() {
    mutatore=1;
    if (!Portale) {
        //Genera il portale di distruzione del livello sul bordo a destra
        for (int y = (max_y/2) - 2; y <= (max_y/2) + 2; y++) {
            griglia[y][max_x - 1] = 'U'; // 'U' come Uscita
        }
        Portale = true;
    }
}

//Prendo in input le coordinate del player per il mutatore Blackout
void Livello::disegna(int playerX, int playerY) {
    box(stdscr, 0, 0);

    start_y = getmaxy(stdscr) / 2 - max_y / 2;
    start_x = getmaxx(stdscr) / 2 - max_x / 2;

    for (int y = 0; y < max_y; y++) {
        for (int x = 0; x < max_x; x++) {

            // Mutatore 4= Blackout
            if (mutatore == 4 && playerX != -1 && playerY != -1) {
                //Calcolo distanza
                int distX = std::abs(x - playerX);
                int distY = std::abs(y - playerY);

                // Raggio visivo attorno al player = quadrato visibile
                if (distX > DIAMETRO_VISIVO || distY > DIAMETRO_VISIVO) {
                    mvaddch(start_y + y, start_x + x, ' '); // Stampa il buio assoluto!
                    continue; // Salta il resto del disegno per questa casella
                }
            }

            char cella = griglia[y][x];

            switch (cella) {
                case 'M':
                    attron(COLOR_PAIR(MURO_INDISTRUTTIBILE) | A_REVERSE);
                    mvaddch(start_y + y, start_x + x, ' ');
                    attroff(COLOR_PAIR(MURO_INDISTRUTTIBILE) | A_REVERSE);
                    break;
                case 'D':
                    attron(COLOR_PAIR(MURO_DISTRUTTIBILE) | A_REVERSE);
                    mvaddch(start_y + y, start_x + x, ' ');
                    attroff(COLOR_PAIR(MURO_DISTRUTTIBILE) | A_REVERSE);
                    break;
                case 'H':
                    attron(COLOR_PAIR(MURO_DURO) | A_REVERSE);
                    mvaddch(start_y + y, start_x + x, ' ');
                    attroff(COLOR_PAIR(MURO_DURO) | A_REVERSE);
                    break;
                case 'T':
                    attron(COLOR_PAIR(TELETRASPORTO));
                    mvaddch(start_y + y, start_x + x, '0');
                    attroff(COLOR_PAIR(TELETRASPORTO));
                    break;
                case 'U':
                    attron(COLOR_PAIR(PORTALE));
                    mvaddch(start_y + y, start_x + x, ACS_BLOCK);
                    attroff(COLOR_PAIR(PORTALE));
                    break;
                case 'C':
                    attron(COLOR_PAIR(ITEM_COMUNE));
                    mvaddch(start_y + y, start_x + x, 'C');
                    attroff(COLOR_PAIR(ITEM_COMUNE));
                    break;
                case 'R':
                    attron(COLOR_PAIR(ITEM_RARO));
                    mvaddch(start_y + y, start_x + x, 'R');
                    attroff(COLOR_PAIR(ITEM_RARO));
                    break;
                case 'E':
                    attron(COLOR_PAIR(ITEM_EPICO) | A_BLINK);
                    mvaddch(start_y + y, start_x + x, 'E');
                    attroff(COLOR_PAIR(ITEM_EPICO) | A_BLINK);
                    break;
            }
        }
    }

    // --- L'ANNUNCIO DINAMICO DEL MUTATORE (TUTTO COLORE BASE) ---
    char nomeMutatore[30] = "";

    // Usiamo sempre e solo il COLORE_BASE per tutti!
    int coloreTitolo = COLOR_PAIR(COLORE_BASE) | A_BOLD;

    if (mutatore == 2) {
        strcpy(nomeMutatore, "- [ ZONA BLINDATA ]");
        coloreTitolo = COLOR_PAIR(COLORE_BASE) | A_BOLD;
    }
    else if (mutatore == 3) {
        strcpy(nomeMutatore, "- [ CARESTIA ]");
        coloreTitolo = COLOR_PAIR(COLORE_BASE) | A_BOLD;
    }
    else if (mutatore == 4) {
        strcpy(nomeMutatore, "- [ BLACKOUT ]");
        coloreTitolo = COLOR_PAIR(COLORE_BASE) | A_BOLD;
    }

    char stringa_titolo[80];
    if (mutatore == 1) {
        sprintf(stringa_titolo, "BOMBERMAN ASCII - LIVELLO %d", idLivello);
    } else {
        sprintf(stringa_titolo, "BOMBERMAN ASCII - LIVELLO %d %s", idLivello, nomeMutatore);
    }

    int lunghezza_stringa = strlen(stringa_titolo);
    int stampa = (max_x - lunghezza_stringa)/2;

    attron(coloreTitolo);
    mvprintw(start_y - 2, start_x + stampa, "%s", stringa_titolo);
    attroff(coloreTitolo);
}

void Livello::generaDrop(int y, int x) {
    // Mutatore 3: niente item per tutto il livello!
    if (mutatore == 3) {
        griglia[y][x] = ' ';
        return;
    }
    //Creiamo l'oggetto dinamicamente per mantenerlo in memoria
    Item* drop = new Item(x, y);

    if (drop->getTipo() != TipoItem::NESSUNO) {
        griglia[y][x] = drop->getSymbol();

        // Salviamo l'item nell'array per potergli togliere la vita in seguito
        if (contatoreItems < 50) {
            itemsATerra[contatoreItems] = drop;
            contatoreItems++;
        } else {
            delete drop; //Sicurezza: se ci sono già 50 item, non lo salviamo
        }
    } else {
        griglia[y][x] = ' ';
        delete drop;
    }
}

int Livello::getTempoMaxLivello() {
    return 150 + (idLivello * 30);
}