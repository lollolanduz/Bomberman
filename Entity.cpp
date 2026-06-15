#include "Entity.h"
#include "terminal_config.h"

// Costruttore base condiviso da Player e Nemici
Entity::Entity(int X, int Y, char S, int CP) :
x(X), y(Y), symbol(S), colorPair(CP) {}

void Entity::draw(int offsetY, int offsetX) {
    mvaddch(y + offsetY, x + offsetX, symbol);
}

int Entity::getX() { return x; }
int Entity::getY() { return y; }