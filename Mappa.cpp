//
// Created by Utente on 19/02/2026.
//
#include <pdcurses.h>
#include "Mappa.h"
#include "Livello.h"

Mappa::Mappa() {
    //primo livello
    testa = new Livello(1);
    Livello* temp = testa;

    // Creiamo gli altri 4 livelli e li colleghiamo
    for (int i = 2; i <= 5; i++) {
        Livello* nuovoLivello = new Livello(i);

        temp->successivo = nuovoLivello;
        nuovoLivello->precedente = temp;

        temp = nuovoLivello;
    }

    // Iniziamo al Livello 1
    livelloCorrente = testa;
}

void Mappa::vaiAlProssimo() {
    if (livelloCorrente->successivo != nullptr) {
        livelloCorrente = livelloCorrente->successivo;
        livelloCorrente->disegna();
    }
}

void Mappa::tornaAlPrecedente() {
    if (livelloCorrente->precedente != nullptr) {
        livelloCorrente = livelloCorrente->precedente;
        livelloCorrente->disegna();
    }
}

Mappa::~Mappa() {
    Livello* temp = testa;

    // Scorriamo tutta la lista finché non finisce
    while (temp != nullptr) {
        Livello* livelloDaCancellare = temp;
        temp = temp->successivo;
        delete livelloDaCancellare;
    }
}

