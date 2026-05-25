//
// Created by Utente on 18/04/2026.
//

#include "Pausa.h"
#include <iostream>
#include <cstring>
#include <pdcurses.h>

Pausa::Pausa() {
    scelta_pausa = 0;
    in_pausa = false;

    // ORA CI SONO 4 OPZIONI
    strcpy(opzioni_pausa[0], "Continua");
    strcpy(opzioni_pausa[1], "Leggenda");
    strcpy(opzioni_pausa[2], "Ricomincia");
    strcpy(opzioni_pausa[3], "Torna al menu");
}

void Pausa::disegnaPause() {
    int start_y = (getmaxy(stdscr) - num_righe) / 2;
    int start_x = (getmaxx(stdscr) - num_colonne) / 2;

    WINDOW* finestra = newwin(num_righe, num_colonne, start_y, start_x);
    box(finestra, 0, 0);

    mvwprintw(finestra, 2, (num_colonne - 5) / 2, "PAUSA");
    mvwhline(finestra, 3, 1, ACS_HLINE, num_colonne - 2);

    // IL CICLO ORA GIRA FINO A 4
    for (int i = 0; i < 4; i++) {
        int riga_y = 6 + (i * 2);
        int start_col = 4;

        if (i == scelta_pausa) {
            wattron(finestra, A_REVERSE);
            mvwprintw(finestra, riga_y, start_col, " ► %-13s ", opzioni_pausa[i]);
            wattroff(finestra, A_REVERSE);
        }
        else {
            mvwprintw(finestra, riga_y, start_col, "   %-13s ", opzioni_pausa[i]);
        }
    }

    wrefresh(finestra);
    delwin(finestra);
}

// LA NUOVA FINESTRA DELLA LEGENDA
void Pausa::mostraLegenda() {
    int height = 14;
    int width = 45;
    int start_y = (getmaxy(stdscr) - height) / 2;
    int start_x = (getmaxx(stdscr) - width) / 2;

    // Creiamo una finestra un po' più larga per farci stare le spiegazioni
    WINDOW* win_legenda = newwin(height, width, start_y, start_x);
    box(win_legenda, 0, 0);

    mvwprintw(win_legenda, 1, (width - 7) / 2, "LEGGENDA");
    mvwhline(win_legenda, 2, 1, ACS_HLINE, width - 2);

    // Elenco dei comandi
    mvwprintw(win_legenda, 4, 3, "W A S D / Frecce : Muovi il giocatore");
    mvwprintw(win_legenda, 6, 3, "Spazio           : Piazza una Bomba");
    mvwprintw(win_legenda, 8, 3, "T                : Pausa il gioco");
    mvwprintw(win_legenda, 10, 3, "+ / -            : Viaggia tra i livelli");

    // Istruzione per uscire (evidenziata)
    wattron(win_legenda, A_REVERSE);
    mvwprintw(win_legenda, 12, (width - 27) / 2, " Premi un tasto per uscire ");
    wattroff(win_legenda, A_REVERSE);

    wrefresh(win_legenda);

    // Aspetta che il giocatore prema un tasto qualsiasi
    getch();

    delwin(win_legenda);
}

int Pausa::gestisciPause() {
    in_pausa = true;
    timeout(-1);

    while (in_pausa) {
        disegnaPause();
        int input = getch();

        if (input == KEY_UP || input == 'w') {
            scelta_pausa--;
            if (scelta_pausa < 0) {
                scelta_pausa = 3; // ORA IL MASSIMO E' 3
            }
        }
        else if (input == KEY_DOWN || input == 's') {
            scelta_pausa++;
            if (scelta_pausa > 3) { // ORA IL MASSIMO E' 3
                scelta_pausa = 0;
            }
        }
        else if (input == '\n') {
            // GESTIONE INTELLIGENTE DEI RETURN
            if (scelta_pausa == 0) {
                return 0; // Continua
            }
            else if (scelta_pausa == 1) {
                // Apre la leggenda
                mostraLegenda();

                // --- IL TRUCCO ANTI-GLITCH ---
                // Diciamo a ncurses di rinfrescare l'intero schermo di base (stdscr)
                // Questo "cancella" visivamente la Legenda rimettendoci sopra la mappa!
                touchwin(stdscr);
                refresh();
            }
            else if (scelta_pausa == 2) {
                return 1; // Ricomincia (Come prima)
            }
            else if (scelta_pausa == 3) {
                return 2; // Torna al menu (Come prima)
            }
        }
    }
    return 0;
}