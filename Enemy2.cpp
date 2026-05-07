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
    int dy[] = {-1, 1, 0, 0};
    int dx[] = {0, 0, -1, 1};

    int nuovaY = y + dy[direzioneAttuale];
    int nuovaX = x + dx[direzioneAttuale];
    char ostacoloCorrente = currentLevel->griglia[nuovaY][nuovaX];

    // Se la sua strada è libera, continua ad andare dritto
    if (ostacoloCorrente != 'M' && ostacoloCorrente != 'D' && ostacoloCorrente != 'T' && ostacoloCorrente != 'U') {
        y = nuovaY;
        x = nuovaX;
    }
    else {
        // Se sbatte accende il radar per cercare una nuova via
        int validDirs[4];
        int validCount = 0;

        for(int i = 0; i < 4; i++) {
            int checkY = y + dy[i];
            int checkX = x + dx[i];
            char ostacolo = currentLevel->griglia[checkY][checkX];

            if(ostacolo != 'M' && ostacolo != 'D' && ostacolo != 'T' && ostacolo != 'U') {
                validDirs[validCount] = i;
                validCount++;
            }
        }

        // Sceglie la nuova strada e fa subito il passo per non fermarsi
        if(validCount > 0) {
            int scelta = rand() % validCount;
            direzioneAttuale = validDirs[scelta];
            y += dy[direzioneAttuale];
            x += dx[direzioneAttuale];
        }
    }
}