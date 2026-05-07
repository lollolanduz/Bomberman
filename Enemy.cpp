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
    // Array per memorizzare le direzioni valide (0: su, 1: giù, 2: sinistra, 3: destra)
    int validDirs[4];
    int validCount = 0;

    // Incrementi per Y e X in base alla direzione
    int dy[] = {-1, 1, 0, 0};
    int dx[] = {0, 0, -1, 1};

    //controlla le 4 celle adiacenti
    for(int i = 0; i < 4; i++) {
        int checkY = y + dy[i];
        int checkX = x + dx[i];
        char ostacolo = currentLevel->griglia[checkY][checkX];

        if(ostacolo != 'M' && ostacolo != 'D' && ostacolo != 'T' && ostacolo != 'U') {
            validDirs[validCount] = i; // Salva la direzione se è libera
            validCount++;
        }
    }

    // Se c'è almeno una strada libera sceglie a caso tra quelle disponibili
    if(validCount > 0) {
        int scelta = rand() % validCount;
        int dirScelta = validDirs[scelta];
        y += dy[dirScelta];
        x += dx[dirScelta];
    }
}