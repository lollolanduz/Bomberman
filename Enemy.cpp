//
// Created by loren on 01/04/2026.
//

#include "Enemy.h"
#include <cstdlib>
#include "Costanti.h"

Enemy::Enemy(char S, Livello* currentLevel) : Entity(0, 0, S) {
    lastDir = -1;
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
    int dy[] = {-1, 1, 0, 0};
    int dx[] = {0, 0, -1, 1};
    int opposto[] = {1, 0, 3, 2}; // L'opposto di Su(0) è Giù(1) etc...

    int stradeValide[4];
    int countValide = 0;
    int backDir = (lastDir != -1) ? opposto[lastDir] : -1;

    for(int i = 0; i < 4; i++) {
        int cY = y + dy[i];
        int cX = x + dx[i];
        char ostacolo = currentLevel->griglia[cY][cX];

        if(ostacolo != 'M' && ostacolo != 'D' && ostacolo != 'T' && ostacolo != 'U') {
            // Se ho più scelte, non tornare indietro
            stradeValide[countValide++] = i;
        }
    }

    if (countValide > 0) {
        // Se c'è più di una strada e una è quella da cui vengo, la scarto
        int sceltaFinale = -1;
        if (countValide > 1 && backDir != -1) {
            int stradeSenzaRitorno[4];
            int countSenzaRitorno = 0;
            for(int i=0; i<countValide; i++) {
                if(stradeValide[i] != backDir) stradeSenzaRitorno[countSenzaRitorno++] = stradeValide[i];
            }
            sceltaFinale = stradeSenzaRitorno[rand() % countSenzaRitorno];
        } else {
            sceltaFinale = stradeValide[rand() % countValide];
        }

        lastDir = sceltaFinale;
        y += dy[lastDir];
        x += dx[lastDir];
    }
}

void Enemy::draw(int offsetY, int offsetX) {
    attron(COLOR_PAIR(COLORE_X) | A_BOLD);
    mvaddch(y + offsetY, x + offsetX, symbol);
    attroff(COLOR_PAIR(COLORE_X) | A_BOLD);

}