//
// Created by loren on 12/03/2026.
//

#include "Player.h"

Player::Player(int X, int Y, char S, int LIFE) : Entity(X, Y, S) {
    life = LIFE;
}

int Player::getlife() {
    return life;
}

void Player::take_damage() {
    if (life > 0) {
        life = life - 1;
    }
}

void Player::bomb_placement() {

}

void Player::collect_item() {

}

void Player::move(int input, Livello *currentLevel) {
    int nuovaX = x;
    int nuovaY = y;


    //con KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT si possono usare anche le freccette direzionali della tastiera
    if (input == 'w' || input == KEY_UP) {
        nuovaY--; // vado su
    }
    else if (input == 's' || input == KEY_DOWN) {
        nuovaY++; // vado giù
    }
    else if (input == 'a' || input == KEY_LEFT) {
        nuovaX--; // vado a sinistra
    }
    else if (input == 'd' || input == KEY_RIGHT) {
        nuovaX++; // vado a destra
    }
    else {
        //se ha premuto un tasto a caso, non faccio nulla e fermo la funzione
        return;
    }

    //guardo dentro la mappa
    char ostacolo = currentLevel->griglia[nuovaY][nuovaX];

    //se la casella non è un muro solido ('M') e non è un muro da rompere ('D')
    if (ostacolo != 'M' && ostacolo != 'D') {
        x = nuovaX;
        y = nuovaY;
    }
}


void Player::reset_position() {
    // Riportiamo il giocatore nello spazio vitale iniziale
    x = 1;
    y = 1;
}


//Per la funzione teletrasporto in livello.cpp
void Player::check_teleport(Livello* currentLevel) {
    currentLevel->gestisciTeletrasporto(y,x);
}






