//
// Created by Utente on 05/03/2026.
//

#include "Menu.h"
#include <iostream>
#include <pdcurses.h>

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
    attron(COLOR_PAIR(4));
    mvprintw(title_y, center_x - 24,   "    __   ___   _  _  __  ___  __   _  _   __   _   ");
    attroff(COLOR_PAIR(4));

    if (!animazioneCompletata) {
        refresh();
        napms(100);
    }

    attron(COLOR_PAIR(5) | A_BOLD);
    mvprintw(title_y+1, center_x - 24, "   |__) / _ \\ | \\/ ||__)|___ |__)\ | \\/ | |__| | \\ |");
    attroff(COLOR_PAIR(5) | A_BOLD);

    if (!animazioneCompletata) {
        refresh();
        napms(100);
    }

    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(title_y+2, center_x - 24, "   |__) \\___/ |    ||__)|___ |  \\ |    | |  | |  \\|");
    attroff(COLOR_PAIR(3) | A_BOLD);

    if (!animazioneCompletata) {
        refresh();
        napms(100);
    }

    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(title_y+3, center_x - 24, "  [________________________________________________]");
    attroff(COLOR_PAIR(3) | A_BOLD);

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
            attron(COLOR_PAIR(4)| A_BOLD | A_BLINK);
            mvprintw(riga_y, center_x - 8, " ► %s ", opzioni[i]);
            attroff(COLOR_PAIR(4) | A_BOLD | A_BLINK);
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
