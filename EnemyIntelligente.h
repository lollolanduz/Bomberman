#ifndef BOMBERMAN_ENEMYINTELLIGENTE_H
#define BOMBERMAN_ENEMYINTELLIGENTE_H

#include "Entity.h"
#include "Costanti.h"
#include "Livello.h"

// Struttura di supporto per ricordare le coordinate nella mappa
struct Punto {
    int x, y;
};

class EnemyIntelligente : public Entity {
private:
    int frameCounter;
    bool isSveglio;

    // Dichiarazione della funzione helper
    bool puoMuoversi(int targetX, int targetY, Livello* livello, bool inFuria);

public:
    // Dichiarazione del costruttore
    EnemyIntelligente(int start_x, int start_y, char sim, int col);

    // Dichiarazioni dei metodi sovrascritti (override)
    void move(Livello* livello, int playerX, int playerY) override;
    int getPunti() override;
    void draw(int offsetY, int offsetX) override;
};

#endif // BOMBERMAN_ENEMYINTELLIGENTE_H