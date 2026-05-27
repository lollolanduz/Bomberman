#ifndef BOMBERMAN_ENEMYRANDOM_H
#define BOMBERMAN_ENEMYRANDOM_H

#include "Entity.h"
#include "Costanti.h"
#include "Livello.h" // Assicurati di includere Livello.h per leggere la griglia
#include <cstdlib>   // Per rand()

class EnemyRandom : public Entity {
private:
    int frameCounter;
    int velocita;
    int puntiGarantiti;
    int lastDir; // <--- La variabile che ti serve per non farli tornare indietro!

public:
    EnemyRandom(int start_x, int start_y, char sim, int punti, int vel, int col)
        : Entity(start_x, start_y, sim, col) {
        frameCounter = 0;
        puntiGarantiti = punti;
        velocita = vel;
        lastDir = -1; // Inizializziamo a -1 come nel tuo vecchio codice
    }

    void move(Livello* livello, int playerX, int playerY) override {
        frameCounter++;

        if (frameCounter >= velocita) {

            // --- LA TUA LOGICA DI MOVIMENTO ANTI-RITORNO ---
            int dy[] = {-1, 1, 0, 0};
            int dx[] = {0, 0, -1, 1};
            int opposto[] = {1, 0, 3, 2}; // L'opposto di Su(0) è Giù(1) etc...

            int stradeValide[4];
            int countValide = 0;
            int backDir = (lastDir != -1) ? opposto[lastDir] : -1;

            for(int i = 0; i < 4; i++) {
                int cY = y + dy[i];
                int cX = x + dx[i];

                // Usiamo livello->griglia invece di currentLevel
                char ostacolo = livello->griglia[cY][cX];

                if(ostacolo != 'M' && ostacolo != 'D' && ostacolo != 'T' && ostacolo != 'U' && ostacolo != 'H') {
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

            frameCounter = 0;
        }
    }

    int getPunti() override {
        return puntiGarantiti;
    }
    void draw(int offsetY, int offsetX) override {
        int col = (symbol == 'X') ? COLORE_X : COLORE_Z;
        attron(COLOR_PAIR(col) | A_BOLD);
        mvaddch(y + offsetY, x + offsetX, symbol);
        attroff(COLOR_PAIR(col) | A_BOLD);
    }
};

#endif // BOMBERMAN_ENEMYRANDOM_H