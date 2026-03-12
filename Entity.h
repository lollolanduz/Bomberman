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
    char symbol;

public:
    Entity(int X, int Y, char S);   //costruttore

    int getX();                     //funzioni utili alla mappa per sapere
                                    //dove si trova l'entità
    int getY();

    void draw();                    //funzione per disegnare il simbolo sulla mappa

};


#endif //BOMBERMAN_ENTITA_H