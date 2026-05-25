//
// Created by lenovo on 24/05/2026.
//

#ifndef BOMBERMAN_ENEMYINTELLIGENTE_H
#define BOMBERMAN_ENEMYINTELLIGENTE_H

#include "Entity.h"
#include "Livello.h"
#include <cmath> // Per usare la funzione abs()

class EnemyIntelligente : public Entity {
private:
    Livello* livelloCorrente;
    bool isSveglio;

public:
    EnemyIntelligente(char S, Livello* livello);

    // Ora il nemico ha bisogno delle coordinate del Player per seguirlo!
    void move(int playerX, int playerY);
    void draw(int startY, int startX);
};

#endif