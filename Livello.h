//
// Created by Utente on 25/02/2026.
//

#ifndef BOMBERMAN_LIVELLI_H
#define BOMBERMAN_LIVELLI_H

#pragma once
#include <pdcurses.h>

// Forward declaration per evitare inclusioni circolari
class Enemy;
class Enemy2;

class Livello {
private:
    void genera_griglia_vuota();
    void genera_MuriFissi();
    void genera_MuraDistruttibili();
    void imposta_Spawn();
    void genera_teletrasporto();

public:
    int idLivello;
    Livello* successivo;
    Livello* precedente;

    int start_y;
    int start_x;

    //Grandezza Matrice
    static const int max_y=24;
    static const int max_x=40;

    char griglia[max_y][max_x];

    // Memoria posizione giocatore per questo livello
    int player_save_x;
    int player_save_y;

    // Array dei nemici (ora appartengono al Livello!)
    Enemy* nemiciX[25];
    int contatoreX;

    Enemy2* nemiciZ[25];
    int contatoreZ;

    Livello(int id);
    ~Livello(); // Distruttore per pulire i nemici quando il livello viene eliminato

    void disegna();
    void gestisciTeletrasporto(int &giocatore_y, int &giocatore_x);

    //Genera portale per concludere il livello
    void apriPortaUscita();
    bool Portale; // Una variabile per sapere se è già aperta

    int tempodiInizio;
};

#endif