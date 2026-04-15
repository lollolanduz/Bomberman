//
// Created by loren on 03/04/2026.
//

#ifndef BOMBERMAN_ENEMY2_H
#define BOMBERMAN_ENEMY2_H
#include "Entity.h"
#include "Livello.h"

class Enemy2 : public Entity{
protected:
    int direzioneAttuale;
public:
    Enemy2(char S, Livello* currentLevel);
    void move(Livello* currentLevel);
};

#endif //BOMBERMAN_ENEMY2_H