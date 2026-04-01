//
// Created by loren on 01/04/2026.
//

#include "Enemy.h"
#include <cstdlib>

Enemy::Enemy(int X, int Y, char S) : Entity(X, Y, S){
}

void Enemy::move(Livello *currentLevel) {
    int direzione = rand() % 4;
    int nuovaY = y;
    int nuovaX = x;

    if (direzione == 0) nuovaY--;
    else if (direzione == 1) nuovaY++;
    else if (direzione == 2) nuovaX--;
    else if (direzione == 3) nuovaX++;

    if (currentLevel->griglia[nuovaY][nuovaX] != 'M' && currentLevel->griglia[nuovaY][nuovaX] != 'D') {
        x = nuovaX;
        y = nuovaY;
    }




}





