//
// Created by loren on 12/03/2026.
//

#include "Player.h"
#include "Costanti.h"

Player::Player(int X, int Y, char S, int LIFE) : Entity(X, Y, S) {
    life = LIFE;
    isInvincible = false;
    invincibilityTimer = 0;

}

void Player::set_position(int newX, int newY) {
    x = newX;
    y = newY;
}

void Player::reset_position() {
    x = 1;
    y = 1;
}

int Player::getlife() {
    return life;
}

void Player::take_damage() {
    if (life > 0 && !isInvincible) {
        life = life - 1;
        isInvincible = true;
        invincibilityTimer = 30; // 3 secondi di scudo
    }
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



//Utile per l'immortalità
void Player::draw(int offsetY, int offsetX, bool isBlinking) {
    if (isBlinking) {
        // Usiamo il colore del teletrasporto E il lampeggio
        attron(A_BLINK | A_BOLD);
    }

    mvaddch(y + offsetY, x + offsetX, symbol);

    if (isBlinking) {
        attroff(A_BLINK | A_BOLD);
    }
}


//Per la funzione teletrasporto in livello.cpp
void Player::check_teleport(Livello* currentLevel) {
    currentLevel->gestisciTeletrasporto(y,x);
}


// Funzioni dell'immortalità
bool Player::getIsInvincible() {
    return isInvincible;
}

void Player::tickInvincibility() {
    if (isInvincible) {
        invincibilityTimer--;
        if (invincibilityTimer <= 0) {
            isInvincible = false; // Torna mortale
        }
    }
}
