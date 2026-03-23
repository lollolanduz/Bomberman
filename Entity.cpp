//

#include "Entity.h"

Entity::Entity(int X, int Y, char S) {
    x = X;
    y = Y;
    symbol = S;
}
void Entity::draw(int offsetY, int offsetX) {
    mvaddch(y + offsetY, x + offsetX, symbol);
}

int Entity::getX() {
    return x;
}

int Entity::getY() {
    return y;
}



