//
// Created by Utente on 05/03/2026.
//

#include "Menu.h"
#include "Costanti.h"
#include <iostream>
#include <pdcurses.h>
#include <fstream>
#include <cstring>

Menu::Menu() {
    //Apre con "GIOCA" già selezionato
    sceltaSelezionata = 0;

    //Copio le stringhe dentro la matrice
    strcpy(opzioni[0], "GIOCA");
    strcpy(opzioni[1], "CLASSIFICA");
    strcpy(opzioni[2], "ESCI");
}

void Menu::disegnaMenu() {
    clear();

    //Per ottenere il centro dello schermo
    int max_y = getmaxy(stdscr);
    int max_x = getmaxx(stdscr);
    int center_y = max_y / 2;
    int center_x = max_x / 2;

    //Serve a stampare la scritta bomberman al centro dello schermo
    //animazione stile retro in cui si genera la scritta dall'alto verso il basso
    int title_y = center_y - 8;
    attron(COLOR_PAIR(LAYER_1));
    mvprintw(title_y, center_x - 24,   "  ___  ___  __  __ ___ ___ ___ __  __   _   _  _ ");
    attroff(COLOR_PAIR(LAYER_1));

    if (!animazioneCompletata) {
        refresh();
        napms(100);
    }

    attron(COLOR_PAIR(LAYER_2) | A_BOLD);
    mvprintw(title_y+1, center_x - 24, " | _ )/ _ \\|  \\/  | _ ) __| _ \\  \\/  | /_\\ | \\| |");
    attroff(COLOR_PAIR(LAYER_2) | A_BOLD);

    if (!animazioneCompletata) {
        refresh();
        napms(100);
    }

    attron(COLOR_PAIR(LAYER_3) | A_BOLD);
    mvprintw(title_y+2, center_x - 24, " | _ \\ (_) | |\\/| | _ \\ _||   / |\\/| |/ _ \\| .` |");
    attroff(COLOR_PAIR(LAYER_3) | A_BOLD);

    if (!animazioneCompletata) {
        refresh();
        napms(100);
    }

    attron(COLOR_PAIR(LAYER_4) | A_BOLD);
    mvprintw(title_y+3, center_x - 24, " |___/\\___/|_|  |_|___/___|_|_\\_|  |_/_/ \\_\\_|\\_|");
    attroff(COLOR_PAIR(LAYER_4) | A_BOLD);

    if (!animazioneCompletata) {
        refresh();
        napms(100);
    }

    animazioneCompletata = true;

    //stampa la freccia che indica le varie scelte
    for (int i = 0; i < 3; i++) {
        int riga_y = center_y + (i * 2);

        if (i == sceltaSelezionata) {
            //Colore giallo per mettere in risalto la scelta selezionata
            attron(COLOR_PAIR(SCELTA_MENU)| A_BOLD | A_BLINK);
            mvprintw(riga_y, center_x - 8, " ► %s ", opzioni[i]);
            attroff(COLOR_PAIR(SCELTA_MENU) | A_BOLD | A_BLINK);
        } else {
            mvprintw(riga_y, center_x - 8, "   %s ", opzioni[i]);
        }
    }

    refresh();
}



int Menu::gestisciInput() {
    while (true) {
        disegnaMenu();

        int input = getch();

        switch (input) {
            case KEY_UP:
            case 'w':
                sceltaSelezionata--;
                // Effetto Ciclico: se vado sopra il primo, salto all'ultimo
                if (sceltaSelezionata < 0) {
                    sceltaSelezionata = 2;
                }
                break;

            case KEY_DOWN:
            case 's':
                sceltaSelezionata++;
                // Effetto Ciclico: se vado sotto l'ultimo, salto al primo
                if (sceltaSelezionata > 2) {
                    sceltaSelezionata = 0;
                }
                break;

            //Gestione del tasto ENTER (Invio).
            case 10:      // Windows
            case 13:      // Linux / Mac
            case KEY_ENTER:
                //Serve a restituire al main cosa si ha scelto
                return sceltaSelezionata;
        }
    }
}



struct RecordClassifica {
    char nome[11];
    int punti;
};

void Menu::mostraClassifica() {
    clear();

    attron(COLOR_PAIR(LAYER_1) | A_BOLD);
    mvprintw(5, 20, "--- C L A S S I F I C A ---");
    attroff(COLOR_PAIR(LAYER_1) | A_BOLD);

    mvprintw(8, 15, "Quanti giocatori vuoi visualizzare? (Premi da 1 a 9): ");
    refresh();

    int n_da_visualizzare = 0;

    flushinp();
    timeout(-1); // Aspettiamo un tasto all'infinito

    while (n_da_visualizzare < 1 || n_da_visualizzare > 9) {
        int ch = getch();
        if (ch >= '1' && ch <= '9') {
            n_da_visualizzare = ch - '0';
        }
    }

    mvprintw(8, 69, "%d", n_da_visualizzare);
    refresh();

    RecordClassifica arrayScore[100];
    int tot_record = 0;

    std::ifstream fileIn("classifica.txt");
    if (fileIn.is_open()) {
        char nomeTemp[11]; // Aggiornato a 11
        int puntiTemp;

        while (fileIn >> nomeTemp >> puntiTemp && tot_record < 100) {
            strcpy(arrayScore[tot_record].nome, nomeTemp);
            arrayScore[tot_record].punti = puntiTemp;
            tot_record++;
        }
        fileIn.close();
    }

    for (int i = 0; i < tot_record - 1; i++) {
        for (int j = 0; j < tot_record - i - 1; j++) {
            if (arrayScore[j].punti < arrayScore[j+1].punti) {
                RecordClassifica temp = arrayScore[j];
                arrayScore[j] = arrayScore[j+1];
                arrayScore[j+1] = temp;
            }
        }
    }

    int righe_stampa = (n_da_visualizzare < tot_record) ? n_da_visualizzare : tot_record;

    if (tot_record == 0) {
        mvprintw(12, 20, "Nessun punteggio salvato finora!");
    } else {
        attron(COLOR_PAIR(SCELTA_MENU) | A_BOLD);
        for (int i = 0; i < righe_stampa; i++) {
            // %-10s formatta la stringa per occupare sempre 10 spazi, allineando bene i punteggi
            mvprintw(11 + (i*2), 25, "%d. %-10s  -  %05d pt", i+1, arrayScore[i].nome, arrayScore[i].punti);
        }
        attroff(COLOR_PAIR(SCELTA_MENU) | A_BOLD);
    }

    mvprintw(11 + (righe_stampa*2) + 3, 20, "Premi un tasto per tornare al menu...");
    refresh();


    flushinp();  //  Svuota qualsiasi tasto tu abbia premuto prima per sbaglio
    timeout(-1); // Dice a ncurses "Aspetta per sempre finché non premo un tasto"
    getch();     //  Il blocco effettivo
    clear();
}