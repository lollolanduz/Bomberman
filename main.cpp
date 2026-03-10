#include <iostream>
#include <pdcurses.h>
using namespace std;

int main() {
    initscr();            // Inizializza lo schermo
    printw("Ciao da PDCurses su CLion!");  // Stampa testo
    refresh();            // Aggiorna lo schermo per mostrare le modifiche
    getch();              // Aspetta che l'utente prema un tasto
    endwin();             // Chiude la modalità curses
    return 0;
}