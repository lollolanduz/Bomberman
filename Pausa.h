//
// Created by Utente on 18/04/2026.
//

#ifndef BOMBERMAN_PAUSE_H
#define BOMBERMAN_PAUSE_H

class Pausa {
private:
    static const int num_righe=16;
    static const int num_colonne=25;

    int scelta_pausa;
    bool in_pausa;

    //4 opzioni
    char opzioni_pausa[4][20];

    void disegnaPause();

    //Funzione per la Legenda
    void mostraLegenda();

public:
    Pausa();
    int gestisciPause();
};

#endif //BOMBERMAN_PAUSE_H