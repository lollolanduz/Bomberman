//
// Created by Utente on 27/03/2026.
//

#ifndef BOMBERMAN_COSTANTI_H
#define BOMBERMAN_COSTANTI_H

#define MURO 1
#define MURO_DISTRUTTIBILE 2

//Colori titolo BOMBERMAN
#define LAYER_1 101
#define LAYER_2 102
#define LAYER_3 103
#define LAYER_4 104

#define SCELTA_MENU 3

#define TELETRASPORTO 4

#define PORTALE 5

#define COLORE_X 6
#define COLORE_Z 7

#define COLORE_BOMBA 8
#define COLORE_ESPLOSIONE 9

#define ITEM_COMUNE 10
#define ITEM_RARO 11
#define ITEM_EPICO 12

#define ITEM_RARO_ATTIVO 13

static const int mps=100;

// --- TIMING DELLE BOMBE (in frame) ---
const int TEMPO_ESPLOSIONE_BOMBA = 25;
const int INIZIO_BOMBA_PALPITANTE = 15;
const int INIZIO_BOMBA_PANICO = 20;

// --- VELOCITA' DEI NEMICI (frame da aspettare prima di muoversi) ---
// Più il numero è basso, più il nemico è veloce
const int FRAME_NEMICO_X = 7;
const int FRAME_NEMICO_Z = 3;
const int FRAME_NEMICO_I = 5;

#endif //BOMBERMAN_COSTANTI_H