//
// Created by loren on 12/03/2026.
//

#ifndef BOMBERMAN_PLAYER_H
#define BOMBERMAN_PLAYER_H
#include "Entity.h"
#include "Livello.h"


class Player : public Entity {
protected:
    int life;               //numero di vite

public:

    Player(int X, int Y, char S, int LIFE);          //costruttore

    int getlife();         //funzione utile al main per sapere quante vite ha il player

    void take_damage();   //funzione per togliere una vita quando subisce danno

    void bomb_placement();   //piazzamento bombe

    void collect_item();    //funzione per collezionare oggetti

    void move(int input, Livello* currentLevel );
};


#endif //BOMBERMAN_PLAYER_H