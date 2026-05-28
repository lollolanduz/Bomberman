#ifndef BOMBERMAN_ENEMYINTELLIGENTE_H
#define BOMBERMAN_ENEMYINTELLIGENTE_H

#include "Entity.h"
#include "Costanti.h"
#include "Livello.h"
#include <cmath>

class EnemyIntelligente : public Entity {
private:
    int frameCounter;
    bool isSveglio;

public:
    EnemyIntelligente(int start_x, int start_y, char sim, int col)
            : Entity(start_x, start_y, sim, col) { // Passa 'col' a Entity
        frameCounter = 0;
        isSveglio = false;
    }

    void move(Livello* livello, int playerX, int playerY) override {
        frameCounter++;

        if (frameCounter >= FRAME_NEMICO_I) {
            int distanza = std::abs(x - playerX) + std::abs(y - playerY);

            if (distanza <= 6) {
                isSveglio = true;
                symbol = 'I';
            } else {
                isSveglio = false;
                symbol = 'i';
            }

            if (!isSveglio) {
                frameCounter = 0;
                return;
            }

            int diffX = playerX - x;
            int diffY = playerY - y;
            int nextX = x;
            int nextY = y;

            if (std::abs(diffX) > std::abs(diffY)) {
                nextX += (diffX > 0) ? 1 : -1;

                // Controllo Bombe per asse X
                bool bombaX = false;
                for(int b=0; b<10; b++) if(livello->isBombActive[b] && livello->bombX[b] == nextX && livello->bombY[b] == y) bombaX = true;

                if (livello->griglia[y][nextX] == 'M' || livello->griglia[y][nextX] == 'D' || livello->griglia[y][nextX] == 'H' || bombaX) {
                    nextX = x;
                    nextY += (diffY > 0) ? 1 : -1;
                }
            } else {
                nextY += (diffY > 0) ? 1 : -1;

                // Controllo Bombe per asse Y
                bool bombaY = false;
                for(int b=0; b<10; b++) if(livello->isBombActive[b] && livello->bombX[b] == x && livello->bombY[b] == nextY) bombaY = true;

                if (livello->griglia[nextY][x] == 'M' || livello->griglia[nextY][x] == 'D' || livello->griglia[nextY][x] == 'H' || bombaY) {
                    nextY = y;
                    nextX += (diffX > 0) ? 1 : -1;
                }
            }

            char ostacolo = livello->griglia[nextY][nextX];

            // Controllo Finale Misto (Griglia + Bomba)
            bool ostacoloBomba = false;
            for(int b=0; b<10; b++) if(livello->isBombActive[b] && livello->bombX[b] == nextX && livello->bombY[b] == nextY) ostacoloBomba = true;

            if (ostacolo != 'M' && ostacolo != 'D' && ostacolo != 'H' && ostacolo != 'T' && ostacolo != 'U' && !ostacoloBomba) {
                x = nextX;
                y = nextY;
            }
            frameCounter = 0;
        }
    }

    int getPunti() override {
        return 300;
    }

    // Aggiungi questo metodo dentro la classe EnemyIntelligente in EnemyIntelligente.h
    void draw(int offsetY, int offsetX) override {
        if (isSveglio) {
            attron(COLOR_PAIR(COLORE_I) | A_BOLD); // Rosso (o il colore che usavi prima)
            mvaddch(y + offsetY, x + offsetX, symbol);
            attroff(COLOR_PAIR(COLORE_I) | A_BOLD);
        } else {
            attron(COLOR_PAIR(COLORE_BASE)); // Bianco (o il colore base)
            mvaddch(y + offsetY, x + offsetX, symbol);
            attroff(COLOR_PAIR(COLORE_BASE));
        }
    }
};

#endif //BOMBERMAN_ENEMYINTELLIGENTE_H