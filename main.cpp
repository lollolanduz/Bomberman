#include <pdcurses.h>
#include "Mappa.h"
#include "Menu.h"

int main() {
    initscr();
    start_color();
    //Per attivare l'effetto lampeggiante
    PDC_set_blink(TRUE);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, 28, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, 208 , COLOR_BLACK); //oppure 214

    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    bool chiudiTutto = false;

    //Ciclo principale del programma
    while (!chiudiTutto) {

        Menu menuPrincipale;

        //Funzione che disegna il menu e rimane attiva fino a che non si esegue una scelta
        int scelta = menuPrincipale.gestisciInput();

        //Se si sceglie gioca
        if (scelta == 0) {
            clear();
            //Eseguiamo il codice della mappa
            Mappa gestoreMappa;
            gestoreMappa.livelloCorrente->disegna();

            bool inGioco = true;
            while(inGioco) {
                int input = getch();

                switch(input) {
                    case 'd':
                        gestoreMappa.vaiAlProssimo();
                        break;
                    case 'a':
                        gestoreMappa.tornaAlPrecedente();
                        break;
                    case 'q':
                        // Tasto 'q' esce dalla partita e torna al menù (per ora)
                        inGioco = false;
                        break;
                }
            }
            clear();

        }
        //Se si sceglie classifica
        else if (scelta == 1) {
            clear();
            mvprintw(10, 10, "Schermata Classifica in costruzione! Premi un tasto per tornare indietro...");
            refresh();
            getch();
            clear();
        }
        //Se si sceglie esci
        else if (scelta == 2) {
            //Interrompe il ciclo while
            chiudiTutto = true;
        }
    }

    endwin();
    return 0;
}