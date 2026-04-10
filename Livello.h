//
// Created by Utente on 25/02/2026.
//

#ifndef BOMBERMAN_LIVELLI_H
#define BOMBERMAN_LIVELLI_H

#pragma once
#include <pdcurses.h>

class Livello {
private:
    void genera_griglia_vuota(); //Stampa ' ' su tutta la griglia

    void genera_MuriFissi(); //Genera i muri indistruttibili

    void genera_MuraDistruttibili(); //Genera i muri distruttibili

    void imposta_Spawn(); //Imposta le coordinate di spawn per non morire all'istante

    void genera_teletrasporto(); //Randomizza la posizione del teletrasporto

    //Momentaneo possibilmente rimosso in futuro
    void randomizza_portale(); //Randomizza il portale di fine livello

public:
    int idLivello;
    //I due puntatori per lista bidirezionale
    Livello* successivo;
    Livello* precedente;

    int start_y;
    int start_x;

    int x_portal;
    int y_portal;

    static const int max_y=20;
    static const int max_x=40;

    char griglia[max_y][max_x];

    Livello(int id);
    void disegna();

    void controllaEsplosione(int y_esplosione, int x_esplosione);

    void gestisciTeletrasporto(int &giocatore_y, int &giocatore_x);
    int tempodiInizio;
};

#endif
