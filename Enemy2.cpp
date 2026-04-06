//
// Created by loren on 03/04/2026.
//
#include "Enemy2.h"
#include <cstdlib>

Enemy2::Enemy2(int X, int Y, char S) : Entity(X, Y, S){
}

void Enemy2::move(Livello* currentLevel) {
    int nuovaX = x;
    int nuovaY = y;

    if (direzioneAttuale == 0) nuovaY--;      // Su
    else if (direzioneAttuale == 1) nuovaY++; // Giù
    else if (direzioneAttuale == 2) nuovaX--; // Sinistra
    else if (direzioneAttuale == 3) nuovaX++; // Destra

    // Guardo la mappa
    if (currentLevel->griglia[nuovaY][nuovaX] != 'M' && currentLevel->griglia[nuovaY][nuovaX] != 'D') {
        // La strada è libera: il nemico fa un passo avanti confermando le coordinate.
        x = nuovaX;
        y = nuovaY;
    }
    else {
        // Ha sbattuto contro un muro.
        // Il nemico resta fermo per questo turno, ma coglie l'occasione
        // per tirare il dado e cambiare la sua memoria per il prossimo giro!
        direzioneAttuale = rand() % 4;
    }
}



