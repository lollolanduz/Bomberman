#ifndef BOMBERMAN_MENU_H
#define BOMBERMAN_MENU_H

#pragma once
#include <pdcurses.h>
#include <cstring>

class Menu {
private:
    int sceltaSelezionata;

    bool animazioneCompletata=false;

    // Matrice di char: 3 stringhe, ognuna lunga al massimo 20 caratteri
    char opzioni[3][20];

    void disegnaMenu();

public:
    Menu();
    int gestisciInput();
};

#endif // BOMBERMAN_MENU_H