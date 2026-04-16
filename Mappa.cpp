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
    if (livelloCorrente == nullptr) {
        return;
    }

    Livello* daEliminare = livelloCorrente;

    //Stacco il livello dalla lista
    if (daEliminare->precedente != nullptr) {
        //Imposto il livello successivo ad daEliminare come il successivo del suo precedente
        daEliminare->precedente->successivo = daEliminare->successivo;
    } else {
        //Se è il primo elemento da eliminare allora la testa diventa il livello successivo
        testa = daEliminare->successivo;
    }

    if (daEliminare->successivo != nullptr) {
        //Imposto il livello precedente ad daEliminare come il precedente del suo successivo
        daEliminare->successivo->precedente = daEliminare->precedente;
    }

    //Valuto dove mandare il giocatore
    if (daEliminare->successivo != nullptr) {
        // Caso normale: c'è un livello dopo quello corrente
        livelloCorrente = daEliminare->successivo;
    }
    else if (testa != nullptr) {
        // Caso in cui finiamo l'ultimo livello saltandoli tutti
        // Ritorniamo al primo livello
        livelloCorrente = testa;
    }
    else {
        // Non c'è un livello successivo e la testa è nullptr.
        // La lista è completamente vuota
        livelloCorrente = nullptr;
    }

    //Distruggiamo fisicamente il livello dalla memoria
    delete daEliminare;

    //Disegniamo il nuovo livello in cui siamo finiti se il gioco non è finito
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

