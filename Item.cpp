//
// Created by lenovo on 13/05/2026.
//

#include "Item.h"

// Il costruttore chiama quello di Entity e poi calcola il drop
Item::Item(int X, int Y) : Entity(X, Y, ' ') {
    calcolaDrop();
    timerVita = 50;
}

// Logica delle probabilità
void Item::calcolaDrop() {
    int roll = rand() % 100 + 1;

    if (roll <= 50) {
        tipo = TipoItem::NESSUNO;
        symbol = ' ';
    }
    else if (roll > 50 && roll <= 80) {
        tipo = TipoItem::PUNTI_EXTRA;
        symbol = 'C';
    }
    else if (roll > 80 && roll <= 95) {
        tipo = TipoItem::RAGGIO_BOMBA;
        symbol = 'R';
    }
    else {
        tipo = TipoItem::VITA_EXTRA;
        symbol = 'E';
    }
}

// Ritorna il tipo di oggetto
TipoItem Item::getTipo() {
    return tipo;
}