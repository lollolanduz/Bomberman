//
// Created by loren on 03/04/2026.
//

#ifndef BOMBERMAN_ENEMY2_H
#define BOMBERMAN_ENEMY2_H
#include "Entity.h"
#include "Livello.h"


/*
 * Questo tipo di nemico, a differenza del primo, ha memoria. Sceglie una direzione e continua a camminare
 * dritto finché non sbatte la faccia contro un muro. Solo a quel punto si ferma e lancia
 * il dado per scegliere una nuova direzione.
 */

class Enemy2 : public Entity{
protected:
    int direzioneAttuale;
public:
    Enemy2(int X, int Y, char S);
    void move(Livello* currentLevel);

};


#endif //BOMBERMAN_ENEMY2_H