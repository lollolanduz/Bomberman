//
// Created by Utente on 19/02/2026.
//

#ifndef BOMBERMAN_MAPPA_H
#define BOMBERMAN_MAPPA_H

#endif //BOMBERMAN_MAPPA_H

#pragma once
#include "Livello.h"

class Mappa {
private:
    Livello* testa;

public:
    Livello* livelloCorrente; // Punta al livello in cui si trova il giocatore ora

    Mappa();
    ~Mappa();

    void vaiAlProssimo();
    void tornaAlPrecedente();
};