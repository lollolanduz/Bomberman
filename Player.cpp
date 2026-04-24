//
// Created by loren on 12/03/2026.
//

#include "Player.h"
#include "Costanti.h"

Player::Player(int X, int Y, char S, int LIFE) : Entity(X, Y, S) {
    life = LIFE;
    isBombActive = false; // All'inizio non ci sono bombe
    bombX = 0;
    bombY = 0;
    bombTimer = 0;
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
    if (life > 0) {
        life = life - 1;
    }
}

void Player::bomb_placement() {
    if (isBombActive == false) {
        isBombActive = true;
        bombX = x;
        bombY = y;
        bombTimer = 0;
    }
}

// Funzione che fa scorrere il tempo
void Player::tickBomb() {
    if (isBombActive == true) {
        bombTimer++; // Aumenta di 1 ad ogni frame
    }
}

// Funzione per spegnere la bomba
void Player::resetBomb() {
    isBombActive = false;
    bombTimer = 0;
}

int Player::getBombTimer() { return bombTimer; }

//Funzioni che restituiscono i valori al main
bool Player::getIsBombActive() { return isBombActive; }

int Player::getBombX() { return bombX; }

int Player::getBombY() { return bombY; }

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
