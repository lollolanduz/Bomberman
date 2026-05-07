//
// Created by loren on 01/04/2026.
//

#ifndef BOMBERMAN_ENEMY_H
#define BOMBERMAN_ENEMY_H
#include "Entity.h"
#include "Livello.h"

class Enemy : public Entity {
protected:
    int lastDir; // Memoria dell'ultima direzione
public:
    Enemy(char S, Livello* currentLevel);
    void move(Livello* currentLevel);
    void draw(int offsetY, int offsetX) override;
};

#endif //BOMBERMAN_ENEMY_H