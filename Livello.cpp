//
// Created by Utente on 25/02/2026.
//
#include <iostream>
#include <cstdlib>
#include "Livello.h"

Livello::Livello(int id) {
    idLivello = id;
    //Puntatori che inizialmente non puntano a nulla
    successivo = nullptr;
    precedente = nullptr;

    //Stampo lo spazio vuoto in tutta la griglia, per evitare
    //che sia formata esclusivamente di "spazzatura"
    for (int y=0; y<max_y; y++) {
        for (int x=0; x<max_x; x++) {
            griglia[y][x]= ' ';
        }
    }

    //M sta per muro indistruttibilie
    //Ciclo for per generazione delle mura perimetrali
    for (int y=0; y < max_y; y++) {
        for (int x=0; x < max_x; x++) {
            if (x==0 || x == max_x - 1 || y == 0 || y == max_y - 1) {
                griglia[y][x] = 'M';
            }
        }
    }

    //Mura indistruttibili nella mappa (a scacchiera)
    for (int y=4; y< max_y; y=y+4) {
        for (int x=4; x < max_x; x=x+4) {
            griglia[y][x] = 'M';
        }
    }

    int wall_cap = 10 + 2*idLivello;

    if ( wall_cap > 70) {
        wall_cap = 70;
    }

    //D = Distruttibile
    for (int y=1; y < max_y - 1; y++) {
        for (int x=1; x < max_x - 1; x++) {
            int wall_rate= rand() % 100;
            if (wall_rate < wall_cap && griglia[y][x] != 'M') {
                griglia[y][x] = 'D';
            }
        }
    }

    //S = spazio vitale
    griglia[1][1]= 'S';
    griglia[1][2]= 'S';
    griglia[2][1]= 'S';

    if (idLivello >= 4) {
        bool teletrasporto_piazzato = false;

        while (!teletrasporto_piazzato) {

            //Coordinate casuali del primo teletrasporto
            int rand_y = (rand() % (max_y - 2)) + 1;
            int rand_x = (rand() % (max_x - 2)) + 1;

            //Coordinate del suo opposto
            int opp_y = max_y - 1 - rand_y;
            int opp_x = max_x - 1 - rand_x;

            //Verifico se sono spazi vuoti
            if (griglia[rand_y][rand_x] == ' ' && griglia[opp_y][opp_x] == ' ') {

                //Entrambi diversi dalla zona di spawn
                if (griglia[rand_y][rand_x] != 'S' && griglia[opp_y][opp_x] != 'S') {

                    griglia[rand_y][rand_x] = 'T';
                    griglia[opp_y][opp_x] = 'T';

                    //Serve ad uscire dal while
                    teletrasporto_piazzato = true;
                }
            }
        }
    }

}

void Livello::disegna() {
    clear();
    box(stdscr, 0, 0);

    //Coordinate del centro dello schermo
    int start_y = getmaxy(stdscr) / 2 - max_y / 2;
    int start_x = getmaxx(stdscr) / 2 - max_x / 2;

    for (int y = 0; y < max_y; y++) {
        for (int x = 0; x < max_x; x++) {
            if (griglia[y][x] == 'M') {
                // A_REVERSE serve per rendere il blocco pieno e senza fessure
                attron(COLOR_PAIR(1) | A_REVERSE);
                //utilizzo ' ' poichè serve a riempire il 'pixel' nella schermata altrimenti con
                //ACS_BLOCK ci sarebbero delle piccole fessure
                mvaddch(start_y + y, start_x + x, ' ');
                attroff(COLOR_PAIR(1) | A_REVERSE);
            }
            else if (griglia[y][x] == 'D') {
                attron(COLOR_PAIR(2) | A_REVERSE);
                mvaddch(start_y + y, start_x + x, ' ');
                attroff(COLOR_PAIR(2) | A_REVERSE);
            }
            else if (griglia[y][x] == 'T') {
                attron(COLOR_PAIR(6));
                mvaddch(start_y + y, start_x + x, '0');
                attroff(COLOR_PAIR(6));
            }
        }
    }

    //Stampa del livello in cui è il giocatore
    mvprintw(start_y - 2, start_x + 5, " BOMBERMAN ASCII - LIVELLO %d ", idLivello);

    refresh();
}