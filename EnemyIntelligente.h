#ifndef BOMBERMAN_ENEMYINTELLIGENTE_H
#define BOMBERMAN_ENEMYINTELLIGENTE_H

#include "Entity.h"
#include "Costanti.h"
#include "Livello.h"

// Struct al volo per mappare le coordinate durante la ricerca del percorso
struct Punto {
    int x, y;
};

class EnemyIntelligente : public Entity {
private:
    int frameCounter;
    bool isSveglio; // Cambia da 'i' a 'I' se rileva il player nei paraggi

    // Funzione di comodo per capire se una casella è calpestabile
    bool puoMuoversi(int targetX, int targetY, Livello* livello, bool inFuria);

public:
    EnemyIntelligente(int start_x, int start_y, char sim, int col);

    void move(Livello* livello, int playerX, int playerY) override;
    int getPunti() override;
    void draw(int offsetY, int offsetX) override;
};

#endif // BOMBERMAN_ENEMYINTELLIGENTE_H