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

            // 1. Calcolo la Distanza di Manhattan dal giocatore
            int distanza = std::abs(x - playerX) + std::abs(y - playerY);

            // 2. Logica di "Aggro" (Risveglio)
            if (distanza <= 6) {
                isSveglio = true;
                symbol = 'I'; // MAIUSCOLA quando ti insegue
            } else {
                isSveglio = false;
                symbol = 'i'; // minuscola quando dorme
            }

            if (!isSveglio) {
                frameCounter = 0;
                return; // Se dorme, sta fermo
            }

            // 3. Logica di Inseguimento
            int diffX = playerX - x;
            int diffY = playerY - y;

            int nextX = x;
            int nextY = y;

            if (std::abs(diffX) > std::abs(diffY)) {
                nextX += (diffX > 0) ? 1 : -1;
                if (livello->griglia[y][nextX] == 'M' || livello->griglia[y][nextX] == 'D' || livello->griglia[y][nextX] == 'H') {
                    nextX = x;
                    nextY += (diffY > 0) ? 1 : -1;
                }
            } else {
                nextY += (diffY > 0) ? 1 : -1;
                if (livello->griglia[nextY][x] == 'M' || livello->griglia[nextY][x] == 'D' || livello->griglia[nextY][x] == 'H' ) {
                    nextY = y;
                    nextX += (diffX > 0) ? 1 : -1;
                }
            }

            char ostacolo = livello->griglia[nextY][nextX];
            if (ostacolo != 'M' && ostacolo != 'D' && ostacolo != 'H' && ostacolo != 'T' && ostacolo != 'U') {
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