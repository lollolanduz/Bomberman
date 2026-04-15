#include <pdcurses.h>
#include <cstdlib>
#include "Mappa.h"
#include "Menu.h"
#include "Player.h"
#include "Costanti.h"
#include "Enemy.h"
#include "Enemy2.h"

int main() {
    initscr();
    start_color();
    PDC_set_blink(TRUE);

    init_pair(MURO, COLOR_WHITE, COLOR_BLACK);
    init_pair(MURO_DISTRUTTIBILE, 28, COLOR_BLACK);

    init_color(LAYER_4, 500, 0, 0);
    init_color(LAYER_2, 1000, 500, 0);

    init_pair(LAYER_1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(LAYER_2, LAYER_2 , COLOR_BLACK);
    init_pair(LAYER_3, COLOR_RED, COLOR_BLACK);
    init_pair(LAYER_4, LAYER_4, COLOR_BLACK);
    init_pair(SCELTA_MENU, COLOR_YELLOW, COLOR_BLACK);
    init_pair(TELETRASPORTO, COLOR_BLUE, COLOR_BLACK);
    init_pair(PORTALE, COLOR_RED, COLOR_BLACK);

    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    bool chiudiTutto = false;

    while (!chiudiTutto) {
        Menu menuPrincipale;
        int scelta = menuPrincipale.gestisciInput();

        if (scelta == 0) {
            clear();
            Mappa gestoreMappa;
            Player giocatore(1, 1, '@', 3);

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
            timeout(100);
            int contatoreFrame = 0;

            while(inGioco) {
                int input = getch();

                if (input != ERR) {
                    switch (input) {
                        case ' ':
                            giocatore.bomb_placement();
                            break;
                        case 'q':
                            inGioco = false;
                            break;

                        case '+':
                            // 1. Salviamo la posizione attuale nel livello che stiamo lasciando
                            gestoreMappa.livelloCorrente->player_save_x = giocatore.getX();
                            gestoreMappa.livelloCorrente->player_save_y = giocatore.getY();

                            // 2. Cambiamo stanza
                            gestoreMappa.vaiAlProssimo();

                            // 3. Carichiamo la posizione salvata del NUOVO livello
                            giocatore.set_position(gestoreMappa.livelloCorrente->player_save_x,
                                                  gestoreMappa.livelloCorrente->player_save_y);
                            break;

                        case '-':
                            // 1. Salviamo la posizione attuale
                            gestoreMappa.livelloCorrente->player_save_x = giocatore.getX();
                            gestoreMappa.livelloCorrente->player_save_y = giocatore.getY();

                            // 2. Torniamo indietro
                            gestoreMappa.tornaAlPrecedente();

                            // 3. Carichiamo la posizione salvata del livello precedente
                            giocatore.set_position(gestoreMappa.livelloCorrente->player_save_x,
                                                  gestoreMappa.livelloCorrente->player_save_y);
                            break;

                        default:
                            giocatore.move(input, gestoreMappa.livelloCorrente);

                            // --- CONTROLLO ENTRATA NEL PORTALE ---
                            if (gestoreMappa.livelloCorrente->griglia[giocatore.getY()][giocatore.getX()] == 'U') {

                                // Distruggiamo il livello attuale e saltiamo al prossimo!
                                gestoreMappa.eliminaLivelloCorrenteEAvanti();

                                // Controlliamo se abbiamo appena finito l'ultimo livello
                                if (gestoreMappa.livelloCorrente == nullptr) {
                                    clear();
                                    mvprintw(10, 20, "HAI VINTO! TUTTI I LIVELLI COMPLETATI!");
                                    refresh();
                                    napms(3000);
                                    inGioco = false; // Torna al menu
                                } else {
                                    // Se ci sono ancora livelli, resettiamo il player per il nuovo livello
                                    giocatore.reset_position();
                                    clear();
                                }
                            }
                            break;
                    }
                }

                giocatore.check_teleport(gestoreMappa.livelloCorrente);
                giocatore.tickBomb();

                //Giocatore.getBombTimer tempo dello scoppio della bomba (ogni 10 = 1 secondo)
                if (giocatore.getIsBombActive() == true && giocatore.getBombTimer() >= 30) {
                    int bX = giocatore.getBombX();
                    int bY = giocatore.getBombY();

                    int esplosioneX[5] = {bX, bX, bX, bX - 1, bX + 1};
                    int esplosioneY[5] = {bY, bY - 1, bY + 1, bY, bY};

                    attron(COLOR_PAIR(LAYER_2) | A_BOLD);
                    for (int dir = 0; dir < 5; dir++) {
                        if (gestoreMappa.livelloCorrente->griglia[esplosioneY[dir]][esplosioneX[dir]] != 'M') {
                            mvaddch(gestoreMappa.livelloCorrente->start_y + esplosioneY[dir],
                                    gestoreMappa.livelloCorrente->start_x + esplosioneX[dir], '#');
                        }
                    }
                    attroff(COLOR_PAIR(LAYER_2) | A_BOLD);
                    refresh();
                    napms(150);

                    for (int dir = 0; dir < 5; dir++) {
                        int eX = esplosioneX[dir];
                        int eY = esplosioneY[dir];

                        if (gestoreMappa.livelloCorrente->griglia[eY][eX] == 'D') {
                            gestoreMappa.livelloCorrente->griglia[eY][eX] = ' ';
                        }

                        if (giocatore.getX() == eX && giocatore.getY() == eY) {
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

                        //Uccisione Nemici X
                        for (int k = 0; k < gestoreMappa.livelloCorrente->contatoreX; k++) {
                            if (gestoreMappa.livelloCorrente->nemiciX[k]->getX() == eX && gestoreMappa.livelloCorrente->nemiciX[k]->getY() == eY) {
                                delete gestoreMappa.livelloCorrente->nemiciX[k];
                                for (int j = k; j < gestoreMappa.livelloCorrente->contatoreX - 1; j++) {
                                    gestoreMappa.livelloCorrente->nemiciX[j] = gestoreMappa.livelloCorrente->nemiciX[j + 1];
                                }
                                gestoreMappa.livelloCorrente->contatoreX--;
                                k--;
                            }
                        }

                        //Uccisione Nemici Z
                        for (int k = 0; k < gestoreMappa.livelloCorrente->contatoreZ; k++) {
                            if (gestoreMappa.livelloCorrente->nemiciZ[k]->getX() == eX && gestoreMappa.livelloCorrente->nemiciZ[k]->getY() == eY) {
                                delete gestoreMappa.livelloCorrente->nemiciZ[k];
                                for (int j = k; j < gestoreMappa.livelloCorrente->contatoreZ - 1; j++) {
                                    gestoreMappa.livelloCorrente->nemiciZ[j] = gestoreMappa.livelloCorrente->nemiciZ[j + 1];
                                }
                                gestoreMappa.livelloCorrente->contatoreZ--;
                                k--;
                            }
                        }
                    }
                    giocatore.resetBomb();
                }

                contatoreFrame++;
                //Serve a gestire ogni quanto si muovono i nemici (porlo >= a 10 indica >= 1 secondo)
                if (contatoreFrame >= 5) {
                    for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreX; i++) {
                        gestoreMappa.livelloCorrente->nemiciX[i]->move(gestoreMappa.livelloCorrente);
                    }
                    for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreZ; i++) {
                        gestoreMappa.livelloCorrente->nemiciZ[i]->move(gestoreMappa.livelloCorrente);
                    }
                    contatoreFrame = 0;
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

                if (colpito == true) {
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

                // --- LOGICA APERTURA PORTA ---
                // Se non ci sono più nemici di nessun tipo, ordiniamo al livello di aprire l'uscita
                if (gestoreMappa.livelloCorrente->contatoreX == 0 && gestoreMappa.livelloCorrente->contatoreZ == 0) {
                    gestoreMappa.livelloCorrente->apriPortaUscita();
                }

                erase();

                gestoreMappa.livelloCorrente->disegna();

                if (giocatore.getIsBombActive() == true) {
                    attron(COLOR_PAIR(LAYER_3) | A_BOLD);
                    mvaddch(gestoreMappa.livelloCorrente->start_y + giocatore.getBombY(),
                            gestoreMappa.livelloCorrente->start_x + giocatore.getBombX(), 'O');
                    attroff(COLOR_PAIR(LAYER_3) | A_BOLD);
                }

                mvprintw(gestoreMappa.livelloCorrente->start_y - 1, gestoreMappa.livelloCorrente->start_x + 1, "Vite: %d", giocatore.getlife());

                for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreX; i++) {
                    gestoreMappa.livelloCorrente->nemiciX[i]->draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);
                }
                for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreZ; i++) {
                    gestoreMappa.livelloCorrente->nemiciZ[i]->draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);
                }

                giocatore.draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);

                refresh();
            }
            clear();
        }
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
    }

    endwin();
    return 0;
}