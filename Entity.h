//
// Created by loren on 10/03/2026.
//

#ifndef BOMBERMAN_ENTITY_H
#define BOMBERMAN_ENTITY_H

class Livello; // "Forward declaration": avvisa il compilatore che esiste la classe Livello

class Entity {
protected:
    int x;
    int y;
    char symbol;
    int colorPair;

public:
    Entity(int X, int Y, char S,int CP = 0);
    virtual ~Entity() {} // Distruttore virtuale (IMPORTANTISSIMO per il polimorfismo!)

    int getX();
    int getY();

    virtual void draw(int offsetY, int offsetX);

    // Funzioni vuote di base. Ogni nemico le "sovrascriverà" (override) a modo suo.
    virtual void move(Livello* livello, int playerX, int playerY) {}
    virtual int getPunti() { return 0; }
};


#endif //BOMBERMAN_ENTITY_H