//
// Created by Utente on 27/03/2026.
//

#ifndef BOMBERMAN_COSTANTI_H
#define BOMBERMAN_COSTANTI_H

#define MURO 1
#define MURO_DURO 2

#define MURO_DISTRUTTIBILE 3

//Colori titolo BOMBERMAN
#define LAYER_1 101
#define LAYER_2 102
#define LAYER_3 103
#define LAYER_4 104

#define SCELTA_MENU 4

#define TELETRASPORTO 5

#define PORTALE 6

#define COLORE_X 7
#define COLORE_Z 8

#define COLORE_BOMBA 9
#define COLORE_ESPLOSIONE 10

#define ITEM_COMUNE 11
#define ITEM_RARO 12
#define ITEM_EPICO 13

#define ITEM_RARO_ATTIVO 14

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