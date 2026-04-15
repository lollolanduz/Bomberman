//
// Created by loren on 01/04/2026.
//

#ifndef BOMBERMAN_ENEMY_H
#define BOMBERMAN_ENEMY_H
#include "Entity.h"
#include "Livello.h"

class Enemy : public Entity {
public:
    // Passiamo il livello al costruttore
    Enemy(char S, Livello* currentLevel);
    void move(Livello* currentLevel);
};

#endif //BOMBERMAN_ENEMY_H