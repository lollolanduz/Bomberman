//
// Created by Utente on 25/02/2026.
//

#ifndef BOMBERMAN_LIVELLI_H
#define BOMBERMAN_LIVELLI_H

#pragma once
#include <pdcurses.h>
#include "item.h"
#include "Entity.h"

// Forward declaration per evitare inclusioni circolari
class Enemy;
class Enemy2;
class EnemyIntelligente;


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

    // 1=Normale, 2=Frenesia, 3=Mura Dure, 4=Nessun item
    int mutatore;

    // Memoria posizione giocatore per questo livello
    int player_save_x;
    int player_save_y;

    // --- VARIABILI DELLA BOMBA ---
    bool isBombActive[10];

    int playerRadiusTimer;

    int bombX[10];
    int bombY[10];
    int bombTimer[10];
    int bombRadius[10];

    // Array dei nemici (ora appartengono al Livello!)
    Entity* nemici[300];
    int contatoreNemici;

    //Array Item
    Item* itemsATerra[50];
    int contatoreItems;

    long tempoLivello;

    int getTempoMaxLivello(); // Restituisce il tempo massimo in secondi

    Livello(int id);
    ~Livello(); // Distruttore per pulire i nemici quando il livello viene eliminato

    void disegna(int playerX = -1, int playerY = -1);
    void gestisciTeletrasporto(int &giocatore_y, int &giocatore_x);
    void generaDrop(int y, int x);


    //Genera portale per concludere il livello
    void apriPortaUscita();
    bool Portale; // Una variabile per sapere se è già aperta

    int tempodiInizio;
};

#endif