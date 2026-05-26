//
// Created by lenovo on 24/05/2026.
//

#include "EnemyIntelligente.h"
#include <pdcurses.h>
#include "Costanti.h"

EnemyIntelligente::EnemyIntelligente(char S, Livello* livello) : Entity(1, 1, S) {
    livelloCorrente = livello;
    isSveglio = false;

    // Generazione casuale dello spawn (simile a Enemy.cpp)
    bool spawnTrovato = false;
    while (!spawnTrovato) {
        int rand_y = (rand() % (Livello::max_y - 2)) + 1;
        int rand_x = (rand() % (Livello::max_x - 2)) + 1;

        // Evita di spawnare sui muri o troppo vicino al giocatore (zona 'S')
        if (livelloCorrente->griglia[rand_y][rand_x] == ' ' &&
            livelloCorrente->griglia[rand_y][rand_x] != 'S') {
            x = rand_x;
            y = rand_y;
            spawnTrovato = true;
        }
    }
}

void EnemyIntelligente::move(int playerX, int playerY) {
    // 1. Calcolo la Distanza di Manhattan dal giocatore
    int distanza = std::abs(x - playerX) + std::abs(y - playerY);

    // 2. Logica di "Aggro" (Risveglio)
    if (distanza <= 6) {
        isSveglio = true;
        symbol = 'I'; // Quando è sveglio, la lettera diventa MAIUSCOLA
    } else {
        isSveglio = false;
        symbol = 'i'; // Quando dorme, la lettera è minuscola
    }

    if (!isSveglio) {
        return; // Se dorme, salta il turno e sta fermo
    }

    // 3. Logica di Inseguimento (Greedy Pathfinding)
    int diffX = playerX - x;
    int diffY = playerY - y;

    int nextX = x;
    int nextY = y;

    // Decide se è meglio muoversi prima in orizzontale o in verticale
    if (std::abs(diffX) > std::abs(diffY)) {
        nextX += (diffX > 0) ? 1 : -1;

        // Se c'è un muro sull'asse X, prova ad aggirarlo muovendosi sull'asse Y
        if (livelloCorrente->griglia[y][nextX] == 'M' || livelloCorrente->griglia[y][nextX] == 'D' || livelloCorrente->griglia[y][nextX] == 'H') {
            nextX = x; // Annulla mossa X
            nextY += (diffY > 0) ? 1 : -1; // Prova mossa Y
        }
    } else {
        nextY += (diffY > 0) ? 1 : -1;

        // Se c'è un muro sull'asse Y, prova ad aggirarlo muovendosi sull'asse X
        if (livelloCorrente->griglia[nextY][x] == 'M' || livelloCorrente->griglia[nextY][x] == 'D' || livelloCorrente->griglia[nextY][x] == 'H' ) {
            nextY = y; // Annulla mossa Y
            nextX += (diffX > 0) ? 1 : -1; // Prova mossa X
        }
    }

    // Esegue il movimento effettivo solo se la destinazione finale è libera
    char ostacolo = livelloCorrente->griglia[nextY][nextX];
    if (ostacolo != 'M' && ostacolo != 'D') {
        x = nextX;
        y = nextY;
    }
}

void EnemyIntelligente::draw(int startY, int startX) {
    // Disegna il nemico di rosso se è sveglio, grigio/bianco se dorme
    if (isSveglio) {
        attron(COLOR_PAIR(LAYER_3) | A_BOLD);
        mvaddch(startY + y, startX + x, symbol);
        attroff(COLOR_PAIR(LAYER_3) | A_BOLD);
    } else {
        mvaddch(startY + y, startX + x, symbol);
    }
}