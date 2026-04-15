//
// Created by loren on 03/04/2026.
//
#include "Enemy2.h"
#include <cstdlib>

Enemy2::Enemy2(char S, Livello* currentLevel) : Entity(0, 0, S) {
    direzioneAttuale = rand() % 4;
    bool spawn_trovato = false;

    while (!spawn_trovato) {
        int rand_y = (rand() % (Livello::max_y - 2)) + 1;
        int rand_x = (rand() % (Livello::max_x - 2)) + 1;

        if (currentLevel->griglia[rand_y][rand_x] == ' ') {
            if (rand_x > 3 || rand_y > 3) {
                this->y = rand_y;
                this->x = rand_x;
                spawn_trovato = true;
            }
        }
    }
}

void Enemy2::move(Livello* currentLevel) {
    int nuovaX = x;
    int nuovaY = y;

    if (direzioneAttuale == 0) nuovaY--;      // Su
    else if (direzioneAttuale == 1) nuovaY++; // Giù
    else if (direzioneAttuale == 2) nuovaX--; // Sinistra
    else if (direzioneAttuale == 3) nuovaX++; // Destra

    if (currentLevel->griglia[nuovaY][nuovaX] != 'M' && currentLevel->griglia[nuovaY][nuovaX] != 'D' && currentLevel->griglia[nuovaY][nuovaX] != 'T' && currentLevel->griglia[nuovaY][nuovaX] != 'U') {
        x = nuovaX;
        y = nuovaY;
    }
    else {
        direzioneAttuale = rand() % 4;
    }
}