//
// Created by loren on 12/03/2026.
//

#include "Player.h"
#include "Costanti.h"

Player::Player(int X, int Y, char S, int LIFE) : Entity(X, Y, S) {
    life = LIFE;
    isInvincible = false;
    invincibilityTimer = 0;
    raggioBomba = 1;
    maxBombe = 1;
    radiusTimer = 0;
    punteggio = 0;
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

    // 1. Calcolo la direzione
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
        //se ha premuto un tasto a caso, non faccio nulla
        return;
    }

    // 2. Controllo l'ostacolo NELLA NUOVA POSIZIONE
    char ostacolo = currentLevel->griglia[nuovaY][nuovaX];

    // Se la casella non è un muro solido ('M') e non è un muro da rompere ('D')
    if (ostacolo != 'M' && ostacolo != 'D') {
        // Mi sposto fisicamente
        x = nuovaX;
        y = nuovaY;

        // 3. ORA che mi sono spostato, controllo se sono finito su un ITEM
        char casella = currentLevel->griglia[y][x];
        if (casella == 'C' || casella == 'R' || casella == 'E') {
            collect_item(casella); // Raccoglie l'item
            currentLevel->griglia[y][x] = ' '; // Rimuove l'item dalla griglia
        }
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

void Player::collect_item(char tipoCasella) {
    switch(tipoCasella) {
        case 'C':
            //score += 100; (se hai una variabile score)
            break;
        case 'R':
            radiusTimer = 100;
            break;
        case 'E':
            life++; // Aggiunge una vita!
            break;
    }
}

int Player::getRaggioBomba() {
    if (radiusTimer > 0) {
        return 2; // Raggio potenziato (4 caselle) nei 10 secondi
    }
    return 1; // Raggio classico quando il tempo è scaduto
}

int Player::getMaxBombe() {
    return maxBombe;
}

void Player::tickRadiusTimer() {
    if (radiusTimer > 0) {
        radiusTimer--;
    }
}

bool Player::isRadiusBoosted() {
    return radiusTimer > 0;
}

void Player::addPunteggio(int punti) {
    punteggio += punti;
}

int Player::getPunteggio() {
    return punteggio;
}