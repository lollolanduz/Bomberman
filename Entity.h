//
// Created by loren on 10/03/2026.
//

#ifndef BOMBERMAN_ENTITY_H
#define BOMBERMAN_ENTITY_H
#include <pdcurses.h>

class Entity {
protected:          //protected così solo le sottoclassi possono ereditare
    int x;
    int y;
    char simbolo;

public:
    Entity(int X, int Y, char S) {
        x = X;
        y = Y;
        simbolo = S;
    }
    int getX() {            //
        return x;           //
    }                       //funzioni utili alla mappa per sapere
                            //dove si trova l'entità
    int getY() {            //
        return y;           //
    }

};


#endif //BOMBERMAN_ENTITA_H