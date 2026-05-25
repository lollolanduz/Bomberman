//
// Created by lenovo on 13/05/2026.
//

#ifndef BOMBERMAN_ITEM_H
#define BOMBERMAN_ITEM_H


#pragma once
#include "Entity.h"
#include <cstdlib>

enum class TipoItem {
    NESSUNO,
    PUNTI_EXTRA,
    RAGGIO_BOMBA,
    VITA_EXTRA
};

class Item : public Entity {
protected:
    TipoItem tipo;
    void calcolaDrop();

public:
    int timerVita;
    Item(int X, int Y);
    TipoItem getTipo();
    char getSymbol() { return symbol; }
};


#endif //BOMBERMAN_ITEM_H