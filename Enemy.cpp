//
// Created by loren on 01/04/2026.
//

#include "Enemy.h"
#include <cstdlib>

Enemy::Enemy(char S, Livello* currentLevel) : Entity(0, 0, S) {
    bool spawn_trovato = false;

    // Il nemico cerca da solo una posizione valida sulla mappa
    while (!spawn_trovato) {
        int rand_y = (rand() % (Livello::max_y - 2)) + 1;
        int rand_x = (rand() % (Livello::max_x - 2)) + 1;

        // Verifica che sia vuoto e lontano dallo spawn del giocatore
        if (currentLevel->griglia[rand_y][rand_x] == ' ') {
            if (rand_x > 3 || rand_y > 3) {
                this->y = rand_y;
                this->x = rand_x;
                spawn_trovato = true;
            }
        }
    }
}

void Enemy::move(Livello *currentLevel) {
    int direzione = rand() % 4;
    int nuovaY = y;
    int nuovaX = x;

    if (direzione == 0) nuovaY--;
    else if (direzione == 1) nuovaY++;
    else if (direzione == 2) nuovaX--;
    else if (direzione == 3) nuovaX++;

    if (currentLevel->griglia[nuovaY][nuovaX] != 'M' && currentLevel->griglia[nuovaY][nuovaX] != 'D' && currentLevel->griglia[nuovaY][nuovaX] != 'T' && currentLevel->griglia[nuovaY][nuovaX] != 'U') {
        x = nuovaX;
        y = nuovaY;
    }
}