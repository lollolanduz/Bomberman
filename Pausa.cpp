//
// Created by Utente on 18/04/2026.
//

#include "Pausa.h"
#include <iostream>
#include <cstring>
#include <pdcurses.h>


Pausa::Pausa() {
    //Apre con scelta su "Continua"
    scelta_pausa=0;

    in_pausa = false;

    strcpy(opzioni_pausa[0], "Continua");
    strcpy(opzioni_pausa[1], "Ricomincia");
    strcpy(opzioni_pausa[2], "Torna al menu");
}

void Pausa::disegnaPause() {
    //Coordinate da cui far partire la nuova finestra
    int start_y = (getmaxy(stdscr) - num_righe) / 2;
    int start_x = (getmaxx(stdscr) - num_colonne) / 2;

    //Creazione nuova finestra
    WINDOW* finestra = newwin(num_righe, num_colonne, start_y, start_x);
    box(finestra, 0, 0);

    //Titolo centrato
    mvwprintw(finestra, 2, (num_colonne - 5) / 2, "PAUSA");

    //Linea sottolineata sotto il titolo
    mvwhline(finestra, 3, 1, ACS_HLINE, num_colonne - 2);

    // 3. Stampiamo le opzioni
    for (int i = 0; i < 3; i++) {
        int riga_y = 6 + (i * 2);

        //Inizializza la colonna di partenza per tutte le stringhe
        //Così che siano allineate
        int start_col = 4;

        if (i == scelta_pausa) {
            wattron(finestra, A_REVERSE);
            //%-13s serve a riempire di spazi vuoti così da creare rettangoli di ugual misura
            mvwprintw(finestra, riga_y, start_col, " ► %-13s ", opzioni_pausa[i]);
            wattroff(finestra, A_REVERSE);
        }
        else {
            mvwprintw(finestra, riga_y, start_col, "   %-13s ", opzioni_pausa[i]);
        }
    }

    wrefresh(finestra);

    //Liberiamo la memoria
    delwin(finestra);
}

int Pausa::gestisciPause() {
    in_pausa=true;

    timeout(-1);

    while (in_pausa) {
        disegnaPause();

        int input = getch();

        if (input == KEY_UP || input == 'w') {
            scelta_pausa--;
            if (scelta_pausa < 0) {
                scelta_pausa = 2;
            }
        }
        else if (input == KEY_DOWN || input == 's') {
            scelta_pausa++;
            if (scelta_pausa > 2) {
                scelta_pausa = 0;
            }
        }
        else if (input == '\n') {
            return scelta_pausa;
        }
    }
    return 0;
}