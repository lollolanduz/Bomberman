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

    //Inizia al livello 1
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

void Mappa::eliminaLivelloCorrenteEAvanti() {
    if (livelloCorrente == nullptr) return;

    Livello* daEliminare = livelloCorrente;

    // 1. STACCHIAMO IL LIVELLO DALLA LISTA (Ricolleghiamo i puntatori)
    if (daEliminare->precedente != nullptr) {
        daEliminare->precedente->successivo = daEliminare->successivo;
    } else {
        // Se stiamo eliminando il primissimo elemento, la nuova testa diventa il successivo!
        testa = daEliminare->successivo;
    }

    if (daEliminare->successivo != nullptr) {
        daEliminare->successivo->precedente = daEliminare->precedente;
    }

    // 2. DECIDIAMO DOVE MANDARE IL GIOCATORE
    if (daEliminare->successivo != nullptr) {
        // Caso normale: c'è un livello dopo di questo, andiamo lì!
        livelloCorrente = daEliminare->successivo;
    }
    else if (testa != nullptr) {
        // GIOCO DI PRESTIGIO: Siamo all'ultimo livello della lista, MA la testa esiste ancora.
        // Significa che abbiamo saltato dei livelli! Ricominciamo dall'inizio.
        livelloCorrente = testa;
    }
    else {
        // Non c'è un livello successivo e la testa è nullptr.
        // La lista è completamente vuota. ORA abbiamo finito davvero il gioco!
        livelloCorrente = nullptr;
    }

    // 3. Distruggiamo fisicamente il livello per liberare RAM
    delete daEliminare;

    // 4. Se il gioco non è finito, disegniamo il nuovo livello in cui siamo finiti
    if (livelloCorrente != nullptr) {
        livelloCorrente->disegna();
    }
}

Mappa::~Mappa() {
    Livello* temp = testa;

    //Scorre tutta la lista finchè non finisce
    while (temp != nullptr) {
        Livello* livelloDaCancellare = temp;
        temp = temp->successivo;
        delete livelloDaCancellare;
    }
}

