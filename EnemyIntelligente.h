#ifndef BOMBERMAN_ENEMYINTELLIGENTE_H
#define BOMBERMAN_ENEMYINTELLIGENTE_H

#include "Entity.h"
#include "Costanti.h"
#include "Livello.h"
#include <cmath>
#include <cstdlib>
#include <queue> // <--- NUOVO! La libreria standard per l'algoritmo di Pathfinding

// Struttura di supporto per ricordare le coordinate nella mappa
struct Punto {
    int x, y;
};

class EnemyIntelligente : public Entity {
private:
    int frameCounter;
    bool isSveglio;

    // Helper per verificare se una casella è calpestabile
    bool puoMuoversi(int targetX, int targetY, Livello* livello, bool inFuria) {
        // Evita di uscire dalla matrice
        if (targetX < 0 || targetX >= Livello::max_x || targetY < 0 || targetY >= Livello::max_y) return false;

        char ostacolo = livello->griglia[targetY][targetX];
        if (ostacolo == 'M' || ostacolo == 'D' || ostacolo == 'H' || ostacolo == 'T' || ostacolo == 'U') return false;

        // Se non è in furia, vede le bombe come muri invalicabili
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

            int distanza = std::abs(x - playerX) + std::abs(y - playerY);

            bool inFuria = (distanza <= 3); // Mantiene la geniale meccanica della "Furia"
            isSveglio = (distanza <= 12);
            symbol = isSveglio ? 'I' : 'i';

            if (!isSveglio) { frameCounter = 0; return; }

            // ====================================================================
            // --- VERA INTELLIGENZA ARTIFICIALE: BREADTH-FIRST SEARCH (BFS) ---
            // ====================================================================

            // 1. Prepariamo la "memoria" della mappa
            bool visited[Livello::max_y][Livello::max_x] = {false};
            Punto parent[Livello::max_y][Livello::max_x]; // Per ricordare da dove siamo arrivati

            std::queue<Punto> coda;
            coda.push({x, y});
            visited[y][x] = true;

            bool trovato = false;

            int dx[] = {0, 0, -1, 1}; // Su, Giù, Sinistra, Destra
            int dy[] = {-1, 1, 0, 0};

            // 2. Il nemico inonda la mappa mentalmente per trovare il giocatore
            while (!coda.empty()) {
                Punto corrente = coda.front();
                coda.pop();

                // Trovato il giocatore!
                if (corrente.x == playerX && corrente.y == playerY) {
                    trovato = true;
                    break;
                }

                // Esplora le 4 direzioni
                for (int i = 0; i < 4; i++) {
                    int nx = corrente.x + dx[i];
                    int ny = corrente.y + dy[i];

                    if (puoMuoversi(nx, ny, livello, inFuria) && !visited[ny][nx]) {
                        visited[ny][nx] = true;
                        parent[ny][nx] = corrente; // Segna come ci è arrivato
                        coda.push({nx, ny});
                    }
                }
            }

            // 3. Se ha trovato un percorso, va a ritroso per capire il PRIMO passo da fare
            if (trovato) {
                Punto step = {playerX, playerY};
                // Torna indietro dalla posizione del player fino ad arrivare a una casella di distanza dal nemico
                while (parent[step.y][step.x].x != x || parent[step.y][step.x].y != y) {
                    step = parent[step.y][step.x];
                }

                // Ecco la mossa perfetta!
                x = step.x;
                y = step.y;
            }
            // 4. Se il giocatore è completamente irraggiungibile (es. chiuso tra i muri)
            else {
                // Fai un passo casuale per non restare paralizzato
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