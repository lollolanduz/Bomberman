//
// Created by loren on 12/03/2026.
//

#ifndef BOMBERMAN_PLAYER_H
#define BOMBERMAN_PLAYER_H
#include "Entity.h"
#include "Livello.h"

class Player : public Entity {
protected:
    int life;
    bool isInvincible;      // Gestione degli i-frames (invincibilità post-danno)
    int invincibilityTimer;
    int raggioBomba;
    int maxBombe;
    int radiusTimer;        // Timer per il potenziamento del raggio
    int punteggio;

public:
    Player(int X, int Y, char S, int LIFE);

    // Forza le coordinate del giocatore (es. per il cambio livello)
    void set_position(int newX, int newY);

    // Riporta il giocatore al punto di spawn
    void reset_position();

    // Ragazzi, usate questi getter nel main/game per stampare l'HUD
    int getlife();
    int getPunteggio();

    int getRaggioBomba(Livello* currentLevel);
    int getMaxBombe();

    // Togle una vita e attiva lo scudo temporaneo
    void take_damage();

    void collect_item(char tipoCasella, Livello* currentLevel);
    void move(int input, Livello* currentLevel);

    bool getIsInvincible();
    void tickInvincibility();

    // Avvia la logica del teletrasporto passandogli la matrice
    void check_teleport(Livello* currentLevel);

    // Disegno grafico (passate isBlinking a true per fargli fare l'animazione di danno)
    void draw(int offsetY, int offsetX, bool isBlinking = false);

    bool isRadiusBoosted(Livello* currentLevel);
    void addPunteggio(int punti);
};

#endif //BOMBERMAN_PLAYER_H