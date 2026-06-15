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

int Player::getlife() { return life; }

void Player::take_damage() {
    // Togliamo vita solo se non ha già lo scudo attivo
    if (life > 0 && !isInvincible) {
        life = life - 1;
        isInvincible = true;
        invincibilityTimer = 30; // Circa 3 secondi di scudo
    }
}

void Player::move(int input, Livello *currentLevel) {
    int nuovaX = x;
    int nuovaY = y;

    // Calcolo dove sta cercando di andare
    if (input == 'w' || input == KEY_UP) nuovaY--;
    else if (input == 's' || input == KEY_DOWN) nuovaY++;
    else if (input == 'a' || input == KEY_LEFT) nuovaX--;
    else if (input == 'd' || input == KEY_RIGHT) nuovaX++;
    else return; // Tasto a caso ignorato

    // Controllo in anticipo (look-ahead) cosa c'è nella casella di destinazione
    // per evitare compenetrazioni grafiche fastidiose
    char ostacolo = currentLevel->griglia[nuovaY][nuovaX];

    if (ostacolo != 'M' && ostacolo != 'D' && ostacolo != 'H') {
        x = nuovaX;
        y = nuovaY;

        // Se ci siamo spostati e c'è un drop, lo raccogliamo e lo togliamo dalla mappa
        // Simo/Ricky: ricordate di passare sempre il currentLevel quando chiamate move!
        char casella = currentLevel->griglia[y][x];
        if (casella == 'C' || casella == 'R' || casella == 'E') {
            collect_item(casella, currentLevel);
            currentLevel->griglia[y][x] = ' ';
        }
    }
}

void Player::draw(int offsetY, int offsetX, bool isBlinking) {
    if (isBlinking) {
        attron(A_BLINK | A_BOLD); // Effetto lampeggiante quando è invincibile
    }

    mvaddch(y + offsetY, x + offsetX, symbol);

    if (isBlinking) {
        attroff(A_BLINK | A_BOLD);
    }
}

void Player::check_teleport(Livello* currentLevel) {
    currentLevel->gestisciTeletrasporto(y,x);
}

bool Player::getIsInvincible() {
    return isInvincible;
}

void Player::tickInvincibility() {
    // Da chiamare nel game loop ad ogni giro
    if (isInvincible) {
        invincibilityTimer--;
        if (invincibilityTimer <= 0) {
            isInvincible = false;
        }
    }
}

void Player::collect_item(char tipoCasella, Livello* currentLevel) {
    switch(tipoCasella) {
        case 'C':
            addPunteggio(50);
            break;
        case 'R':
            currentLevel->playerRadiusTimer = 100; // Il buff è legato al livello corrente
            break;
        case 'E':
            life++;
            break;
    }
}

int Player::getRaggioBomba(Livello* currentLevel) {
    if (currentLevel->playerRadiusTimer > 0) return 2;
    return 1;
}

int Player::getMaxBombe() { return maxBombe; }

bool Player::isRadiusBoosted(Livello* currentLevel) {
    return currentLevel->playerRadiusTimer > 0;
}

void Player::addPunteggio(int punti) { punteggio += punti; }
int Player::getPunteggio() { return punteggio; }