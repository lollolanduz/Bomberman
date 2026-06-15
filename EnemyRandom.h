#ifndef BOMBERMAN_ENEMYRANDOM_H
#define BOMBERMAN_ENEMYRANDOM_H

#include "Entity.h"
#include "Livello.h"

class EnemyRandom : public Entity {
private:
    int frameCounter;
    int velocita;
    int puntiGarantiti;
    int lastDir;

public:
    EnemyRandom(int start_x, int start_y, char sim, int punti, int vel, int col);

    void move(Livello* livello, int playerX, int playerY) override;
    int getPunti() override;
    void draw(int offsetY, int offsetX) override;
};

#endif // BOMBERMAN_ENEMYRANDOM_H