#include <pdcurses.h>
#include "Mappa.h"
#include "Menu.h"
#include "Player.h"

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
    init_pair(6, COLOR_BLUE, COLOR_BLACK);

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
        // ... (codice precedente del main) ...

        // Se si sceglie gioca
        if (scelta == 0) {
            clear();
            Mappa gestoreMappa;

            // 1. FAI NASCERE IL TUO EROE (fuori dal ciclo)
            Player giocatore(1, 1, '@', 3);

            bool inGioco = true;
            while(inGioco) {
                int input = getch();

                // ho sostituito lo switch con un solo if
                if (input == 'q') {
                    inGioco = false; //esce dalla partita
                }
                else {
                    giocatore.move(input, gestoreMappa.livelloCorrente);
                }

                clear(); //pulisce lo schermo vecchio

                //Disegna la mappa (Questo fa calcolare a Simone start_y e start_x)
                gestoreMappa.livelloCorrente->disegna();

                //Disegna il giocatore passandogli l'offset!
                giocatore.draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);

                refresh(); // Mostra il fotogramma aggiornato
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