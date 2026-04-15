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
    bool isBombActive;      // Mi dice se c'è una bomba a terra
    int bombX;              // Coordinata X della bomba
    int bombY;              // Coordinata Y della bomba

    int bombTimer;          // Cronometro della bomba
public:

    Player(int X, int Y, char S, int LIFE);          //costruttore

    // Nuovo metodo per impostare la posizione salvata
    void set_position(int newX, int newY);

    int getlife();         //funzione utile al main per sapere quante vite ha il player

    void take_damage();    //funzione per togliere una vita quando subisce danno

    void reset_position(); // Riporta il giocatore al punto di spawn

    void bomb_placement();   //piazzamento bombe

    void collect_item();    //funzione per collezionare oggetti

    void move(int input, Livello* currentLevel );

    void check_teleport(Livello* currentLevel); //Controlla se attivare la funzione teletrasporto

    bool getIsBombActive();

    int getBombX();

    int getBombY();

    int getBombTimer();    // Per leggere a che punto è il timer

    void tickBomb();       // Per far scorrere il tempo

    void resetBomb();      // Per far sparire la bomba dopo l'esplosione
};


#endif //BOMBERMAN_PLAYER_H