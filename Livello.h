//
// Created by Utente on 25/02/2026.
//

#ifndef BOMBERMAN_LIVELLI_H
#define BOMBERMAN_LIVELLI_H

#pragma once
#include <pdcurses.h>

class Livello {
public:
    int idLivello;

    //I due puntatori per lista bidirezionale
    Livello* successivo;
    Livello* precedente;

    static const int max_y=20;
    static const int max_x=40;

    char griglia[max_y][max_x];

    Livello(int id);
    void disegna();
};

#endif
