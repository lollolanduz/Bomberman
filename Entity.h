//
// Created by loren on 10/03/2026.
//

#ifndef BOMBERMAN_ENTITY_H
#define BOMBERMAN_ENTITY_H

// Forward declaration per evitare problemi di inclusione circolare con Livello.h
class Livello;

class Entity {
protected:
    int x;
    int y;
    char symbol;
    int colorPair;

public:
    Entity(int X, int Y, char S, int CP = 0);

    // Distruttore virtuale (indispensabile per il polimorfismo, altrimenti sclera coi puntatori dei nemici)
    virtual ~Entity() {}

    int getX();
    int getY();

    virtual void draw(int offsetY, int offsetX);

    // Funzioni base che Player e Nemici andranno a sovrascrivere (override)
    virtual void move(Livello* livello, int playerX, int playerY) {}
    virtual int getPunti() { return 0; }
};

#endif //BOMBERMAN_ENTITY_H