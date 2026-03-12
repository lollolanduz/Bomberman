//

#include "Entity.h"

Entity::Entity(int X, int Y, char S) {
    x = X;
    y = Y;
    symbol = S;
}
void Entity::draw() {
    mvaddch(y, x, symbol);
}

int Entity::getX() {
    return x;
}

int Entity::getY() {
    return y;
}



