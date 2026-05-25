#include <pdcurses.h>
#include <cstdlib>
#include <ctime>
#include "Mappa.h"
#include "Menu.h"
#include "Player.h"
#include "Costanti.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "EnemyIntelligente.h"
#include "Pausa.h"

int main() {
    initscr();
    start_color();
    PDC_set_blink(TRUE);

    init_color(MURO_DISTRUTTIBILE, 0, 460,0);

    init_color(LAYER_2, 1000, 500, 0);
    init_color(LAYER_4, 500, 0, 0);

    init_color(COLORE_BOMBA, 800, 0, 0);
    init_color(COLORE_ESPLOSIONE, 686, 0,0);

    init_pair(MURO, COLOR_WHITE, COLOR_BLACK);
    init_pair(MURO_DISTRUTTIBILE, MURO_DISTRUTTIBILE, COLOR_BLACK);

    init_pair(COLORE_X, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLORE_Z, COLOR_CYAN, COLOR_BLACK);

    init_pair(LAYER_1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(LAYER_2, LAYER_2 , COLOR_BLACK);
    init_pair(LAYER_3, COLOR_RED, COLOR_BLACK);
    init_pair(LAYER_4, LAYER_4, COLOR_BLACK);

    init_pair(SCELTA_MENU, COLOR_YELLOW, COLOR_BLACK);

    init_pair(TELETRASPORTO, COLOR_BLUE, COLOR_BLACK);

    init_pair(PORTALE, COLOR_RED, COLOR_BLACK);

    init_pair(99, COLOR_BLACK, COLOR_YELLOW);

    init_pair(COLORE_BOMBA, COLORE_BOMBA, COLOR_BLACK);
    init_pair(COLORE_ESPLOSIONE, COLORE_ESPLOSIONE, COLOR_BLACK);

    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    bool chiudiTutto = false;

    while (!chiudiTutto) {
        Menu menuPrincipale;
        int scelta = menuPrincipale.gestisciInput();

        if (scelta == 0) {
            bool vuoleRicominciare;

            do {
                vuoleRicominciare = false;
                clear();
                Mappa gestoreMappa;
                Player giocatore(1, 1, 'P', 5000);

                gestoreMappa.livelloCorrente->disegna();
                giocatore.draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);

                // Disegna i nemici iniziali
                for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreX; i++) {
                    gestoreMappa.livelloCorrente->nemiciX[i]->draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);
                }
                for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreZ; i++) {
                    gestoreMappa.livelloCorrente->nemiciZ[i]->draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);
                }

                refresh();

                bool inGioco = true;
                timeout(mps);
                int contatoreFrameX = 0;
                int contatoreFrameZ = 0;
                int contatoreFrameI = 0;


while(inGioco) {
    int input = getch();
    // 1. GESTIONE INPUT (Si attiva solo se premi un tasto)
    if (input != ERR) {
        switch (input) {

            case ' ': {
                // 1. Conta quante bombe hai già piazzato a terra
                int bombeAttive = 0;
                for (int i = 0; i < 10; i++) {
                    if (gestoreMappa.livelloCorrente->isBombActive[i]) bombeAttive++;
                }

                // 2. Se ne hai meno di 5 (maxBombe), cerca uno slot libero e piazzala
                if (bombeAttive < giocatore.getMaxBombe()) {
                    for (int i = 0; i < 10; i++) {
                        if (!gestoreMappa.livelloCorrente->isBombActive[i]) {
                            gestoreMappa.livelloCorrente->isBombActive[i] = true;
                            gestoreMappa.livelloCorrente->bombX[i] = giocatore.getX();
                            gestoreMappa.livelloCorrente->bombY[i] = giocatore.getY();
                            gestoreMappa.livelloCorrente->bombTimer[i] = 0;
                            gestoreMappa.livelloCorrente->bombRadius[i] = giocatore.getRaggioBomba();
                            break; // Esce dal ciclo dopo averla piazzata
                        }
                    }
                }
                break;
            }

                case 'q':

                inGioco = false;

                break;

                case '+':

                gestoreMappa.livelloCorrente->player_save_x = giocatore.getX();

                gestoreMappa.livelloCorrente->player_save_y = giocatore.getY();

                gestoreMappa.vaiAlProssimo();

                giocatore.set_position(gestoreMappa.livelloCorrente->player_save_x, gestoreMappa.livelloCorrente->player_save_y);

                break;

                case '-':

                gestoreMappa.livelloCorrente->player_save_x = giocatore.getX();

                gestoreMappa.livelloCorrente->player_save_y = giocatore.getY();

                gestoreMappa.tornaAlPrecedente();

                giocatore.set_position(gestoreMappa.livelloCorrente->player_save_x, gestoreMappa.livelloCorrente->player_save_y);

                break;

                case 't':
                case 'T':
                            {

                    time_t tempo_inizio_pausa = std::time(nullptr);

                    Pausa menuPausa;
                    int sceltaPausa = menuPausa.gestisciPause();
                    time_t tempo_fine_pausa = std::time(nullptr);
                    int durata_pausa = tempo_fine_pausa - tempo_inizio_pausa;
                    if (sceltaPausa == 0) {
                        if (gestoreMappa.livelloCorrente->tempodiInizio != -1) {
                            gestoreMappa.livelloCorrente->tempodiInizio += durata_pausa;
                        }
                        timeout(mps);
                        clear();
                    }
                    else if (sceltaPausa == 1) {
                        vuoleRicominciare = true;
                                    inGioco = false;
                                }
                                else if (sceltaPausa == 2) {
                                    inGioco = false;
                                }
                            }
                                break;
            default:
                giocatore.move(input, gestoreMappa.livelloCorrente);
                break;
        } // Fine switch
    } // Fine if(input != ERR)


    giocatore.tickInvincibility();
    giocatore.tickRadiusTimer();
    giocatore.check_teleport(gestoreMappa.livelloCorrente);

    // 2. LOGICA DI GIOCO (Gira SEMPRE, indipendentemente dall'input)

// --- LOGICA DELLE BOMBE MULTIPLE (Timer ed Esplosione) ---
    for (int b = 0; b < 10; b++) {
        if (gestoreMappa.livelloCorrente->isBombActive[b]) {

            gestoreMappa.livelloCorrente->bombTimer[b]++; // Il timer avanza per questa bomba

            if (gestoreMappa.livelloCorrente->bombTimer[b] >= 25) {
                int bX = gestoreMappa.livelloCorrente->bombX[b];
                int bY = gestoreMappa.livelloCorrente->bombY[b];
                int raggio = gestoreMappa.livelloCorrente->bombRadius[b];

                int esplosioneX[300];
                int esplosioneY[300];
                int numCelle = 0;

                for (int y = bY - raggio; y <= bY + raggio; y++) {
                    for (int x = bX - raggio; x <= bX + raggio; x++) {
                        if (y < 0 || y >= Livello::max_y || x < 0 || x >= Livello::max_x) continue;
                        int distQuadrata = (x - bX) * (x - bX) + (y - bY) * (y - bY);
                        if (distQuadrata <= raggio * raggio) {
                            if (gestoreMappa.livelloCorrente->griglia[y][x] == 'M') continue;
                            esplosioneX[numCelle] = x;
                            esplosioneY[numCelle] = y;
                            numCelle++;
                        }
                    }
                }

                // --- DISEGNO ONDA D'URTO CON FLASH ---
                attron(COLOR_PAIR(COLORE_ESPLOSIONE) | A_BOLD);
                attron(A_REVERSE); // Accende l'inversione colore (Sfondo Rosso)

                for (int i = 0; i < numCelle; i++) {
                    mvaddch(gestoreMappa.livelloCorrente->start_y + esplosioneY[i],
                            gestoreMappa.livelloCorrente->start_x + esplosioneX[i], '#');
                }

                attroff(A_REVERSE); // Spegne l'inversione colore per non buggare il gioco
                attroff(COLOR_PAIR(COLORE_ESPLOSIONE) | A_BOLD);
                refresh();
                napms(150); // Mantiene il flash a schermo per una frazione di secondo

                for (int i = 0; i < numCelle; i++) {
                    int eX = esplosioneX[i];
                    int eY = esplosioneY[i];

                    if (gestoreMappa.livelloCorrente->griglia[eY][eX] == 'D') gestoreMappa.livelloCorrente->generaDrop(eY, eX);

                    if (giocatore.getX() == eX && giocatore.getY() == eY && !giocatore.getIsInvincible()) {
                        giocatore.take_damage();
                        if (giocatore.getlife() > 0) giocatore.reset_position();
                        else { clear(); mvprintw(Livello::max_y / 2, Livello::max_x / 2 - 5, "G A M E   O V E R"); refresh(); napms(2000); inGioco = false; }
                    }
                    for (int k = 0; k < gestoreMappa.livelloCorrente->contatoreX; k++) {
                        if (gestoreMappa.livelloCorrente->nemiciX[k]->getX() == eX && gestoreMappa.livelloCorrente->nemiciX[k]->getY() == eY) {
                            gestoreMappa.livelloCorrente->generaDrop(eY, eX); delete gestoreMappa.livelloCorrente->nemiciX[k];
                            for (int j = k; j < gestoreMappa.livelloCorrente->contatoreX - 1; j++) gestoreMappa.livelloCorrente->nemiciX[j] = gestoreMappa.livelloCorrente->nemiciX[j + 1];
                            gestoreMappa.livelloCorrente->contatoreX--; k--;
                        }
                    }
                    for (int k = 0; k < gestoreMappa.livelloCorrente->contatoreZ; k++) {
                        if (gestoreMappa.livelloCorrente->nemiciZ[k]->getX() == eX && gestoreMappa.livelloCorrente->nemiciZ[k]->getY() == eY) {
                            gestoreMappa.livelloCorrente->generaDrop(eY, eX); delete gestoreMappa.livelloCorrente->nemiciZ[k];
                            for (int j = k; j < gestoreMappa.livelloCorrente->contatoreZ - 1; j++) gestoreMappa.livelloCorrente->nemiciZ[j] = gestoreMappa.livelloCorrente->nemiciZ[j + 1];
                            gestoreMappa.livelloCorrente->contatoreZ--; k--;
                        }
                    }
                    for (int k = 0; k < gestoreMappa.livelloCorrente->contatoreI; k++) {
                        if (gestoreMappa.livelloCorrente->nemiciI[k]->getX() == eX && gestoreMappa.livelloCorrente->nemiciI[k]->getY() == eY) {
                            gestoreMappa.livelloCorrente->generaDrop(eY, eX); delete gestoreMappa.livelloCorrente->nemiciI[k];
                            for (int j = k; j < gestoreMappa.livelloCorrente->contatoreI - 1; j++) gestoreMappa.livelloCorrente->nemiciI[j] = gestoreMappa.livelloCorrente->nemiciI[j + 1];
                            gestoreMappa.livelloCorrente->contatoreI--; k--;
                        }
                    }
                }
                // Spegne SOLO questa bomba specifica
                gestoreMappa.livelloCorrente->isBombActive[b] = false;
                gestoreMappa.livelloCorrente->bombTimer[b] = 0;
            }
        }
    }

    contatoreFrameX++;
    if (contatoreFrameX >= 7) {
        for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreX; i++) {
            gestoreMappa.livelloCorrente->nemiciX[i]->move(gestoreMappa.livelloCorrente);
        }
        contatoreFrameX = 0;
    }

    contatoreFrameZ++;
    if (contatoreFrameZ >= 3) {
        for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreZ; i++) {
            gestoreMappa.livelloCorrente->nemiciZ[i]->move(gestoreMappa.livelloCorrente);
        }
        contatoreFrameZ = 0;
    }

    // MOVIMENTO NEMICO INTELLIGENTE
    contatoreFrameI++;
    if (contatoreFrameI >= 5) {
        for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreI; i++) {
            // GLI PASSIAMO LE COORDINATE DEL GIOCATORE!
            gestoreMappa.livelloCorrente->nemiciI[i]->move(giocatore.getX(), giocatore.getY());
        }
        contatoreFrameI = 0;
    }

    // --- LOGICA TIMER DEGLI ITEM ---
    for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreItems; i++) {
        // Togliamo un punto vita all'item
        gestoreMappa.livelloCorrente->itemsATerra[i]->timerVita--;

        // Se il tempo è scaduto...
        if (gestoreMappa.livelloCorrente->itemsATerra[i]->timerVita <= 0) {
            int ix = gestoreMappa.livelloCorrente->itemsATerra[i]->getX();
            int iy = gestoreMappa.livelloCorrente->itemsATerra[i]->getY();
            char sym = gestoreMappa.livelloCorrente->itemsATerra[i]->getSymbol();

            // 1. Cancelliamo la lettera dalla mappa (SOLO SE il giocatore non l'ha già raccolta!)
            if (gestoreMappa.livelloCorrente->griglia[iy][ix] == sym) {
                gestoreMappa.livelloCorrente->griglia[iy][ix] = ' ';
            }

            // 2. Eliminiamo l'oggetto dalla memoria e compattiamo l'array
            delete gestoreMappa.livelloCorrente->itemsATerra[i];
            for (int j = i; j < gestoreMappa.livelloCorrente->contatoreItems - 1; j++) {
                gestoreMappa.livelloCorrente->itemsATerra[j] = gestoreMappa.livelloCorrente->itemsATerra[j + 1];
            }
            gestoreMappa.livelloCorrente->contatoreItems--;
            i--; // Riduciamo l'indice perché abbiamo spostato tutto indietro
        }
    }

    bool colpito = false;

    for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreX; i++) {
        if (giocatore.getX() == gestoreMappa.livelloCorrente->nemiciX[i]->getX() && giocatore.getY() == gestoreMappa.livelloCorrente->nemiciX[i]->getY()) {
            colpito = true;
        }
    }
    for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreZ; i++) {
        if (giocatore.getX() == gestoreMappa.livelloCorrente->nemiciZ[i]->getX() && giocatore.getY() == gestoreMappa.livelloCorrente->nemiciZ[i]->getY()) {
            colpito = true;
        }
    }

    for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreI; i++) {
        if (giocatore.getX() == gestoreMappa.livelloCorrente->nemiciI[i]->getX() &&
            giocatore.getY() == gestoreMappa.livelloCorrente->nemiciI[i]->getY()) {
            colpito = true;
            }
    }

    if (colpito == true && !giocatore.getIsInvincible()) {
        giocatore.take_damage();
        if (giocatore.getlife() > 0) {
            giocatore.reset_position();
        } else {
            clear();
            mvprintw(Livello::max_y / 2, Livello::max_x / 2 - 5, "G A M E   O V E R");
            refresh();
            napms(2000);
            inGioco = false;
        }
    }

    if (gestoreMappa.livelloCorrente->contatoreX == 0 && gestoreMappa.livelloCorrente->contatoreZ == 0) {
        gestoreMappa.livelloCorrente->apriPortaUscita();
    }

    erase();
    gestoreMappa.livelloCorrente->disegna();

    mvprintw(gestoreMappa.livelloCorrente->start_y - 1, gestoreMappa.livelloCorrente->start_x + 1, "Vite: %d", giocatore.getlife());


// --- DISEGNO GRAFICO BOMBA "CINEMATOGRAFICA" ---
    for (int b = 0; b < 10; b++) {
        if (gestoreMappa.livelloCorrente->isBombActive[b]) {

            bool disegnaBomba = true;
            int t = gestoreMappa.livelloCorrente->bombTimer[b]; // <--- Usiamo il timer per decidere

            // FASE 2: Palpitante (Timer da 15 a 19)
            // Lampeggia piano: accesa per 4 frame, spenta per 4
            if (t >= 15 && t < 20) {
                if (t % 8 < 4) {
                    disegnaBomba = false;
                }
            }

            // FASE 3: Panico total-lampeggio (Timer da 20 a 24)
            // Lampeggia velocissimo: accesa per 1 frame, spenta per 1
            if (t >= 20 && t < 25) {
                if (t % 2 == 0) {
                    disegnaBomba = false;
                }
            }

            // Disegniamo la bomba 'O' solo se il frame è "acceso"
            if (disegnaBomba) {
                attron(COLOR_PAIR(COLORE_BOMBA) | A_BOLD);
                mvaddch(gestoreMappa.livelloCorrente->start_y + gestoreMappa.livelloCorrente->bombY[b],
                        gestoreMappa.livelloCorrente->start_x + gestoreMappa.livelloCorrente->bombX[b], 'O');
                attroff(COLOR_PAIR(COLORE_BOMBA) | A_BOLD);
            }
        }
    }

    //disegno nemici;
    for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreX; i++) {
        gestoreMappa.livelloCorrente->nemiciX[i]->draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);
    }

    for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreZ; i++) {
        gestoreMappa.livelloCorrente->nemiciZ[i]->draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);
    }

    for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreI; i++) {
        gestoreMappa.livelloCorrente->nemiciI[i]->draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);
    }

    bool sulTeletrasporto = (gestoreMappa.livelloCorrente->griglia[giocatore.getY()][giocatore.getX()] == 'T');
    bool possoLampeggiareTele = sulTeletrasporto && (gestoreMappa.livelloCorrente->tempodiInizio != -1);
    bool disegnaGiocatore = true;

    if (possoLampeggiareTele) {
        if (contatoreFrameX % 4 < 2) {
            disegnaGiocatore = false;
        }
    }

    if (giocatore.getIsInvincible()) {
        if (contatoreFrameX % 2 == 0) {
            disegnaGiocatore = false;
        }
    }

    if (disegnaGiocatore) {
        if (giocatore.isRadiusBoosted()) {
            // Se ha il bonus attivo, lo disegnamo con lo sfondo giallo (coppia 99)
            attron(COLOR_PAIR(99) | A_BOLD);
            mvaddch(gestoreMappa.livelloCorrente->start_y + giocatore.getY(),
                    gestoreMappa.livelloCorrente->start_x + giocatore.getX(), 'P');
            attroff(COLOR_PAIR(99) | A_BOLD);
        } else {
            // Altrimenti lo disegna normalmente con i suoi colori classici
            giocatore.draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x, false);
        }
    }
    refresh();

}

clear();
} while (vuoleRicominciare);
} // <--- Fine if(scelta == 0)
        else if (scelta == 1) {
            clear();
            mvprintw(10, 10, "Schermata Classifica in costruzione! Premi un tasto per tornare indietro...");
            refresh();
            timeout(-1);
            getch();
            clear();
        }
        else if (scelta == 2) {
            chiudiTutto = true;
        }

    } // <--- Fine while(!chiudiTutto)

    endwin();
    return 0;
}