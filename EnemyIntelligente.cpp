#include "EnemyIntelligente.h"
#include <cmath>
#include <cstdlib>
#include "terminal_config.h"

EnemyIntelligente::EnemyIntelligente(int start_x, int start_y, char sim, int col)
        : Entity(start_x, start_y, sim, col) {
    frameCounter = 0;
    isSveglio = false;
}

bool EnemyIntelligente::puoMuoversi(int targetX, int targetY, Livello* livello, bool inFuria) {
    // Fuori dai bordi?
    if (targetX < 0 || targetX >= Livello::max_x || targetY < 0 || targetY >= Livello::max_y) return false;

    char ostacolo = livello->griglia[targetY][targetX];
    if (ostacolo == 'M' || ostacolo == 'D' || ostacolo == 'H' || ostacolo == 'T' || ostacolo == 'U') return false;

    // Se è "in furia" se ne sbatte e passa pure sopra le bombe, altrimenti le evita
    if (!inFuria) {
        for (int b = 0; b < 10; b++) {
            if (livello->isBombActive[b] && livello->bombX[b] == targetX && livello->bombY[b] == targetY) return false;
        }
    }
    return true;
}

void EnemyIntelligente::move(Livello* livello, int playerX, int playerY) {
    frameCounter++;

    if (frameCounter >= FRAME_NEMICO_I) {

        // Se sono già addosso al player esco per evitare calcoli inutili o divisioni per zero
        if (x == playerX && y == playerY) {
            frameCounter = 0;
            return;
        }

        int distanza = std::abs(x - playerX) + std::abs(y - playerY);

        bool inFuria = (distanza <= 3);
        isSveglio = (distanza <= 12);
        symbol = isSveglio ? 'I' : 'i';

        // Se è lontano dorme ('i' minuscola) e salta il turno
        if (!isSveglio) { frameCounter = 0; return; }

        // Ragazzi, visto che vector/queue sono vietati, ho simulato la coda della BFS a mano
        // con un array statico da 960 (40x24) per essere sicuro di non andare mai in buffer overflow
        bool visited[Livello::max_y][Livello::max_x] = {false};
        Punto parent[Livello::max_y][Livello::max_x];

        Punto coda[960];
        int inizioCoda = 0;
        int fineCoda = 0;

        // Infilo la partenza nella coda
        coda[fineCoda] = {x, y};
        fineCoda++;
        visited[y][x] = true;

        bool trovato = false;
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        while (inizioCoda < fineCoda) {
            Punto corrente = coda[inizioCoda];
            inizioCoda++;

            if (corrente.x == playerX && corrente.y == playerY) {
                trovato = true;
                break;
            }

            // Esploro le celle adiacenti
            for (int i = 0; i < 4; i++) {
                int nx = corrente.x + dx[i];
                int ny = corrente.y + dy[i];

                if (puoMuoversi(nx, ny, livello, inFuria) && !visited[ny][nx]) {
                    visited[ny][nx] = true;
                    parent[ny][nx] = corrente;

                    coda[fineCoda] = {nx, ny};
                    fineCoda++;
                }
            }
        }

        // Se ha trovato la strada fa "rewind" dell'array parent per capire quale passo fare ora
        if (trovato) {
            Punto step = {playerX, playerY};
            while (parent[step.y][step.x].x != x || parent[step.y][step.x].y != y) {
                step = parent[step.y][step.x];
            }
            x = step.x;
            y = step.y;
        }
        else {
            // Se è incastrato (es. player coperto da bombe) fa passi a caso per non rimanere bloccato
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

int EnemyIntelligente::getPunti() { return 300; }

void EnemyIntelligente::draw(int offsetY, int offsetX) {
    if (isSveglio) {
        attron(COLOR_PAIR(COLORE_I) | A_BOLD); // Rosso quando caccia ('I')
        mvaddch(y + offsetY, x + offsetX, symbol);
        attroff(COLOR_PAIR(COLORE_I) | A_BOLD);
    } else {
        attron(COLOR_PAIR(COLORE_BASE)); // Bianco quando dorme ('i')
        mvaddch(y + offsetY, x + offsetX, symbol);
        attroff(COLOR_PAIR(COLORE_BASE));
    }
}