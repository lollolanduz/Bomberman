#ifndef BOMBERMAN_ENEMYINTELLIGENTE_H
#define BOMBERMAN_ENEMYINTELLIGENTE_H

#include "Entity.h"
#include "Costanti.h"
#include "Livello.h"
#include <cmath>
#include <cstdlib>

// Struttura di supporto per ricordare le coordinate nella mappa
struct Punto {
    int x, y;
};

class EnemyIntelligente : public Entity {
private:
    int frameCounter;
    bool isSveglio;

    bool puoMuoversi(int targetX, int targetY, Livello* livello, bool inFuria) {
        if (targetX < 0 || targetX >= Livello::max_x || targetY < 0 || targetY >= Livello::max_y) return false;

        char ostacolo = livello->griglia[targetY][targetX];
        if (ostacolo == 'M' || ostacolo == 'D' || ostacolo == 'H' || ostacolo == 'T' || ostacolo == 'U') return false;

        if (!inFuria) {
            for (int b = 0; b < 10; b++) {
                if (livello->isBombActive[b] && livello->bombX[b] == targetX && livello->bombY[b] == targetY) return false;
            }
        }
        return true;
    }

public:
    EnemyIntelligente(int start_x, int start_y, char sim, int col)
            : Entity(start_x, start_y, sim, col) {
        frameCounter = 0;
        isSveglio = false;
    }

    void move(Livello* livello, int playerX, int playerY) override {
        frameCounter++;

        if (frameCounter >= FRAME_NEMICO_I) {

            // --- FIX DEFINITIVO DEL CRASH ---
            // Se il nemico si trova GIA' sulle coordinate del giocatore (es. al respawn),
            // non ha senso cercare un percorso. La matrice 'parent' risulterebbe vuota
            // e manderebbe in SegFault il gioco. Lo blocchiamo subito!
            if (x == playerX && y == playerY) {
                frameCounter = 0;
                return;
            }

            int distanza = std::abs(x - playerX) + std::abs(y - playerY);

            bool inFuria = (distanza <= 3);
            isSveglio = (distanza <= 12);
            symbol = isSveglio ? 'I' : 'i';

            if (!isSveglio) { frameCounter = 0; return; }

            // ====================================================================
            // --- BFS CON CODA CUSTOM (Array Statico, niente std::queue) ---
            // ====================================================================

            bool visited[Livello::max_y][Livello::max_x] = {false};
            Punto parent[Livello::max_y][Livello::max_x];

            // Creiamo la nostra coda manuale. Max celle possibili = 40x24 = 960
            Punto coda[960];
            int inizioCoda = 0;
            int fineCoda = 0;

            // "Push" della posizione iniziale nella coda
            coda[fineCoda] = {x, y};
            fineCoda++;
            visited[y][x] = true;

            bool trovato = false;

            int dx[] = {0, 0, -1, 1};
            int dy[] = {-1, 1, 0, 0};

            // Il ciclo continua finché ci sono elementi nella coda
            while (inizioCoda < fineCoda) {
                // "Pop" dell'elemento corrente
                Punto corrente = coda[inizioCoda];
                inizioCoda++;

                if (corrente.x == playerX && corrente.y == playerY) {
                    trovato = true;
                    break;
                }

                for (int i = 0; i < 4; i++) {
                    int nx = corrente.x + dx[i];
                    int ny = corrente.y + dy[i];

                    if (puoMuoversi(nx, ny, livello, inFuria) && !visited[ny][nx]) {
                        visited[ny][nx] = true;
                        parent[ny][nx] = corrente;

                        // "Push" del nuovo punto nella coda
                        coda[fineCoda] = {nx, ny};
                        fineCoda++;
                    }
                }
            }

            // --- Ricostruzione del percorso ---
            if (trovato) {
                Punto step = {playerX, playerY};
                while (parent[step.y][step.x].x != x || parent[step.y][step.x].y != y) {
                    step = parent[step.y][step.x];
                }
                x = step.x;
                y = step.y;
            }
            else {
                // Passi casuali se è bloccato
                int stradeLibere[4];
                int countLibere = 0;
                for (int i = 0; i < 4; i++) {
                    int tx = x + dx[i]; int ty = y + dy[i];
                    if (puoMuoversi(tx, ty, livello, inFuria)) stradeLibere[countLibere++] = i;
                }
                if (countLibere > 0) {
                    int scelta = stradeLibere[rand() % countLibere];
                    x += dx[scelta]; y += dy[scelta];
                }
            }

            frameCounter = 0;
        }
    }

    int getPunti() override { return 300; }

    void draw(int offsetY, int offsetX) override {
        if (isSveglio) {
            attron(COLOR_PAIR(COLORE_I) | A_BOLD);
            mvaddch(y + offsetY, x + offsetX, symbol);
            attroff(COLOR_PAIR(COLORE_I) | A_BOLD);
        } else {
            attron(COLOR_PAIR(COLORE_BASE));
            mvaddch(y + offsetY, x + offsetX, symbol);
            attroff(COLOR_PAIR(COLORE_BASE));
        }
    }
};

#endif