#include "Game.h"
#include <pdcurses.h>
#include <cstdlib>
#include <ctime>
#include "Mappa.h"
#include "Menu.h"
#include "Player.h"
#include "Costanti.h"
#include "EnemyRandom.h"
#include "EnemyIntelligente.h"
#include "Pausa.h"
#include <fstream>
#include <cmath>

Game::Game() {
    inizializzaGrafica();
}

Game::~Game() {
    endwin();
}

void Game::inizializzaGrafica() {
    initscr();
    start_color();
    PDC_set_blink(TRUE);

    init_color(MURO_DISTRUTTIBILE, 0, 480, 0);
    init_color(LAYER_2, 1000, 500, 0);
    init_color(LAYER_4, 500, 0, 0);
    init_color(MURO_DURO, 0, 320, 0);
    init_color(COLORE_BOMBA, 800, 0, 0);
    init_color(COLORE_ESPLOSIONE, 686, 0, 0);

    init_pair(COLORE_BASE, COLOR_WHITE, COLOR_BLACK);

    init_pair(MURO_INDISTRUTTIBILE, COLOR_WHITE, COLOR_BLACK);
    init_pair(MURO_DURO, MURO_DURO, COLOR_BLACK);
    init_pair(MURO_DISTRUTTIBILE, MURO_DISTRUTTIBILE, COLOR_BLACK);

    init_pair(COLORE_X, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLORE_Z, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLORE_I, COLOR_RED, COLOR_BLACK);

    init_pair(LAYER_1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(LAYER_2, LAYER_2, COLOR_BLACK);
    init_pair(LAYER_3, COLOR_RED, COLOR_BLACK);
    init_pair(LAYER_4, LAYER_4, COLOR_BLACK);

    init_pair(SCELTA_MENU, COLOR_YELLOW, COLOR_BLACK);

    init_pair(TELETRASPORTO, COLOR_BLUE, COLOR_BLACK);

    init_pair(PORTALE, COLOR_RED, COLOR_BLACK);

    init_pair(ITEM_RARO_ATTIVO, COLOR_BLACK, COLOR_YELLOW);

    init_pair(COLORE_BOMBA, COLORE_BOMBA, COLOR_BLACK);
    init_pair(COLORE_ESPLOSIONE, COLORE_ESPLOSIONE, COLOR_BLACK);

    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
}

void Game::run() {
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
                Player giocatore(1, 1, 'P', 5);

                gestoreMappa.livelloCorrente->disegna(giocatore.getX(), giocatore.getY());
                giocatore.draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);

                for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreNemici; i++) {
                    int nX = gestoreMappa.livelloCorrente->nemici[i]->getX();
                    int nY = gestoreMappa.livelloCorrente->nemici[i]->getY();
                    // Mutatore 4 = Blackout
                    if (gestoreMappa.livelloCorrente->mutatore == 4 && !gestoreMappa.livelloCorrente->Portale) {
                        if (std::abs(nX - giocatore.getX()) > DIAMETRO_VISIVO || std::abs(nY - giocatore.getY()) > DIAMETRO_VISIVO) continue;
                    }
                    gestoreMappa.livelloCorrente->nemici[i]->draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);
                }

                refresh();

                bool inGioco = true;
                timeout(mps);
                int contatoreFrameX = 0;

                gestoreMappa.livelloCorrente->tempoLivello = std::time(nullptr);

                while (inGioco) {
                    int input = getch();

                    if (input != ERR) {
                        switch (input) {
                            case ' ': {
                                int bombeAttive = 0;
                                for (int i = 0; i < 10; i++) {
                                    if (gestoreMappa.livelloCorrente->isBombActive[i]) bombeAttive++;
                                }

                                if (bombeAttive < giocatore.getMaxBombe()) {
                                    for (int i = 0; i < 10; i++) {
                                        if (!gestoreMappa.livelloCorrente->isBombActive[i]) {
                                            gestoreMappa.livelloCorrente->isBombActive[i] = true;
                                            gestoreMappa.livelloCorrente->bombX[i] = giocatore.getX();
                                            gestoreMappa.livelloCorrente->bombY[i] = giocatore.getY();
                                            gestoreMappa.livelloCorrente->bombTimer[i] = 0;
                                            gestoreMappa.livelloCorrente->bombRadius[i] = giocatore.getRaggioBomba(gestoreMappa.livelloCorrente);
                                            break;
                                        }
                                    }
                                }
                                break;
                            }
                            case '+': {
                                time_t ora = std::time(nullptr);
                                if (gestoreMappa.livelloCorrente->tempoLivello > 1000000) gestoreMappa.livelloCorrente->tempoLivello = ora - gestoreMappa.livelloCorrente->tempoLivello;
                                gestoreMappa.livelloCorrente->player_save_x = giocatore.getX();
                                gestoreMappa.livelloCorrente->player_save_y = giocatore.getY();
                                gestoreMappa.vaiAlProssimo();
                                if (gestoreMappa.livelloCorrente->tempoLivello == -1) gestoreMappa.livelloCorrente->tempoLivello = ora;
                                else if (gestoreMappa.livelloCorrente->tempoLivello < 1000000) gestoreMappa.livelloCorrente->tempoLivello = ora - gestoreMappa.livelloCorrente->tempoLivello;
                                giocatore.set_position(gestoreMappa.livelloCorrente->player_save_x, gestoreMappa.livelloCorrente->player_save_y);
                                break;
                            }

                            case '-': {
                                time_t ora = std::time(nullptr);
                                if (gestoreMappa.livelloCorrente->tempoLivello > 1000000) gestoreMappa.livelloCorrente->tempoLivello = ora - gestoreMappa.livelloCorrente->tempoLivello;
                                gestoreMappa.livelloCorrente->player_save_x = giocatore.getX();
                                gestoreMappa.livelloCorrente->player_save_y = giocatore.getY();
                                gestoreMappa.tornaAlPrecedente();
                                if (gestoreMappa.livelloCorrente->tempoLivello == -1) gestoreMappa.livelloCorrente->tempoLivello = ora;
                                else if (gestoreMappa.livelloCorrente->tempoLivello < 1000000) gestoreMappa.livelloCorrente->tempoLivello = ora - gestoreMappa.livelloCorrente->tempoLivello;
                                giocatore.set_position(gestoreMappa.livelloCorrente->player_save_x, gestoreMappa.livelloCorrente->player_save_y);
                                break;
                            }

                            case 't':
                            case 'T': {
                                time_t tempo_inizio_pausa = std::time(nullptr);
                                Pausa menuPausa;
                                int sceltaPausa = menuPausa.gestisciPause();
                                time_t tempo_fine_pausa = std::time(nullptr);
                                int durata_pausa = tempo_fine_pausa - tempo_inizio_pausa;
                                if (sceltaPausa == 0) {
                                    if (gestoreMappa.livelloCorrente->tempoLivello > 1000000) gestoreMappa.livelloCorrente->tempoLivello += durata_pausa;
                                    if (gestoreMappa.livelloCorrente->tempodiInizio != -1) gestoreMappa.livelloCorrente->tempodiInizio += durata_pausa;
                                    timeout(mps);
                                    clear();
                                }
                                else if (sceltaPausa == 1) { vuoleRicominciare = true; inGioco = false; }
                                else if (sceltaPausa == 2) { inGioco = false; }
                                break;
                            }
                            default:
                                giocatore.move(input, gestoreMappa.livelloCorrente);

                                if (gestoreMappa.livelloCorrente->griglia[giocatore.getY()][giocatore.getX()] == 'U') {
                                    int tMassimo = gestoreMappa.livelloCorrente->getTempoMaxLivello();
                                    int tPassato = std::time(nullptr) - gestoreMappa.livelloCorrente->tempoLivello;
                                    int tRimanente = tMassimo - tPassato;

                                    if (tRimanente > 0) {
                                        clear();
                                        int centerY = getmaxy(stdscr) / 2;
                                        int centerX = getmaxx(stdscr) / 2;

                                        attron(COLOR_PAIR(SCELTA_MENU) | A_BOLD);
                                        mvprintw(centerY - 4, centerX - 10, "====================");
                                        mvprintw(centerY - 3, centerX - 9,  " LIVELLO SUPERATO ");
                                        mvprintw(centerY - 2, centerX - 10, "====================");
                                        attroff(COLOR_PAIR(SCELTA_MENU) | A_BOLD);

                                        while (tRimanente > 0) {
                                            tRimanente--;
                                            giocatore.addPunteggio(10);
                                            int visualizzaMinuti = tRimanente / 60;
                                            int visualizzaSecondi = tRimanente % 60;

                                            attron(COLOR_PAIR(COLORE_Z) | A_BOLD);
                                            mvprintw(centerY, centerX - 11, "Tempo Rimasto : %02d:%02d", visualizzaMinuti, visualizzaSecondi);
                                            attroff(COLOR_PAIR(COLORE_Z) | A_BOLD);

                                            attron(COLOR_PAIR(SCELTA_MENU) | A_BOLD);
                                            mvprintw(centerY + 2, centerX - 11, "Punteggio Totale: %05d", giocatore.getPunteggio());
                                            attroff(COLOR_PAIR(SCELTA_MENU) | A_BOLD);

                                            refresh();
                                            napms(30);
                                        }

                                        attron(COLOR_PAIR(PORTALE) | A_BLINK | A_BOLD);
                                        mvprintw(centerY + 5, centerX - 5, " PERFETTO! ");
                                        attroff(COLOR_PAIR(PORTALE) | A_BLINK | A_BOLD);

                                        refresh();
                                        napms(1500);
                                    }

                                    gestoreMappa.eliminaLivelloCorrenteEAvanti();

                                    if (gestoreMappa.livelloCorrente == nullptr) {
                                        gestisciFinePartita(true, giocatore.getPunteggio());
                                        inGioco = false;
                                    } else {
                                        gestoreMappa.livelloCorrente->tempoLivello = std::time(nullptr);
                                        giocatore.set_position(gestoreMappa.livelloCorrente->player_save_x, gestoreMappa.livelloCorrente->player_save_y);
                                        clear();
                                        gestoreMappa.livelloCorrente->disegna(giocatore.getX(), giocatore.getY());
                                        refresh();
                                    }
                                }
                                break;
                        }
                    }

                    time_t oraAttuale = std::time(nullptr);
                    if (gestoreMappa.livelloCorrente->tempoLivello == -1) gestoreMappa.livelloCorrente->tempoLivello = oraAttuale;
                    int tempoMassimo = gestoreMappa.livelloCorrente->getTempoMaxLivello();
                    int tempoPassato = oraAttuale - gestoreMappa.livelloCorrente->tempoLivello;
                    int tempoRimanente = tempoMassimo - tempoPassato;

                    if (tempoRimanente <= 0) {
                        gestisciFinePartita(false, giocatore.getPunteggio());
                        inGioco = false;
                        break;
                    }

                    giocatore.tickInvincibility();
                    if (gestoreMappa.livelloCorrente->playerRadiusTimer > 0) {
                        gestoreMappa.livelloCorrente->playerRadiusTimer--;
                    }
                    giocatore.check_teleport(gestoreMappa.livelloCorrente);

                    for (int b = 0; b < 10; b++) {
                        if (gestoreMappa.livelloCorrente->isBombActive[b]) {
                            gestoreMappa.livelloCorrente->bombTimer[b]++;

                            if (gestoreMappa.livelloCorrente->bombTimer[b] >= TEMPO_ESPLOSIONE_BOMBA) {
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

                                attron(COLOR_PAIR(COLORE_ESPLOSIONE) | A_BOLD | A_REVERSE);
                                for (int i = 0; i < numCelle; i++) {
                                    mvaddch(gestoreMappa.livelloCorrente->start_y + esplosioneY[i], gestoreMappa.livelloCorrente->start_x + esplosioneX[i], '#');
                                }
                                attroff(A_REVERSE | COLOR_PAIR(COLORE_ESPLOSIONE) | A_BOLD);
                                refresh();
                                napms(150);

                                for (int i = 0; i < numCelle; i++) {
                                    int eX = esplosioneX[i];
                                    int eY = esplosioneY[i];

                                    if (gestoreMappa.livelloCorrente->griglia[eY][eX] == 'H') {
                                        gestoreMappa.livelloCorrente->griglia[eY][eX] = 'D';
                                    } else if (gestoreMappa.livelloCorrente->griglia[eY][eX] == 'D') {
                                        gestoreMappa.livelloCorrente->generaDrop(eY, eX);
                                    }

                                    if (giocatore.getX() == eX && giocatore.getY() == eY && !giocatore.getIsInvincible()) {
                                        giocatore.take_damage();
                                        if (giocatore.getlife() > 0) giocatore.reset_position();
                                        else { gestisciFinePartita(false, giocatore.getPunteggio());
                                            inGioco = false; }
                                    }

                                    for (int k = 0; k < gestoreMappa.livelloCorrente->contatoreNemici; k++) {
                                        if (gestoreMappa.livelloCorrente->nemici[k]->getX() == eX && gestoreMappa.livelloCorrente->nemici[k]->getY() == eY) {
                                            giocatore.addPunteggio(gestoreMappa.livelloCorrente->nemici[k]->getPunti());
                                            gestoreMappa.livelloCorrente->generaDrop(eY, eX);
                                            delete gestoreMappa.livelloCorrente->nemici[k];
                                            for (int j = k; j < gestoreMappa.livelloCorrente->contatoreNemici - 1; j++) {
                                                gestoreMappa.livelloCorrente->nemici[j] = gestoreMappa.livelloCorrente->nemici[j + 1];
                                            }
                                            gestoreMappa.livelloCorrente->contatoreNemici--;
                                            k--;
                                        }
                                    }
                                }
                                gestoreMappa.livelloCorrente->isBombActive[b] = false;
                                gestoreMappa.livelloCorrente->bombTimer[b] = 0;
                            }
                        }
                    }

                    contatoreFrameX++;

                    for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreNemici; i++) {
                        gestoreMappa.livelloCorrente->nemici[i]->move(gestoreMappa.livelloCorrente, giocatore.getX(), giocatore.getY());
                    }

                    for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreItems; i++) {
                        gestoreMappa.livelloCorrente->itemsATerra[i]->timerVita--;
                        if (gestoreMappa.livelloCorrente->itemsATerra[i]->timerVita <= 0) {
                            int ix = gestoreMappa.livelloCorrente->itemsATerra[i]->getX();
                            int iy = gestoreMappa.livelloCorrente->itemsATerra[i]->getY();
                            char sym = gestoreMappa.livelloCorrente->itemsATerra[i]->getSymbol();
                            if (gestoreMappa.livelloCorrente->griglia[iy][ix] == sym) gestoreMappa.livelloCorrente->griglia[iy][ix] = ' ';
                            delete gestoreMappa.livelloCorrente->itemsATerra[i];
                            for (int j = i; j < gestoreMappa.livelloCorrente->contatoreItems - 1; j++) gestoreMappa.livelloCorrente->itemsATerra[j] = gestoreMappa.livelloCorrente->itemsATerra[j + 1];
                            gestoreMappa.livelloCorrente->contatoreItems--; i--;
                        }
                    }

                    bool colpito = false;
                    for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreNemici; i++) {
                        if (giocatore.getX() == gestoreMappa.livelloCorrente->nemici[i]->getX() && giocatore.getY() == gestoreMappa.livelloCorrente->nemici[i]->getY()) {
                            colpito = true;
                            break;
                        }
                    }

                    if (colpito && !giocatore.getIsInvincible()) {
                        giocatore.take_damage();
                        if (giocatore.getlife() > 0) giocatore.reset_position();
                        else { gestisciFinePartita(false, giocatore.getPunteggio());
                            inGioco = false;}
                    }

                    if (gestoreMappa.livelloCorrente->contatoreNemici == 0) {
                        gestoreMappa.livelloCorrente->apriPortaUscita();
                    }

                    erase();
                    // Passiamo le coordinate al disegno per applicare il Blackout
                    gestoreMappa.livelloCorrente->disegna(giocatore.getX(), giocatore.getY());

                    int visualizzaMinuti = (tempoRimanente > 0) ? tempoRimanente / 60 : 0;
                    int visualizzaSecondi = (tempoRimanente > 0) ? tempoRimanente % 60 : 0;
                    mvprintw(gestoreMappa.livelloCorrente->start_y - 1, gestoreMappa.livelloCorrente->start_x + 1, "Vite: %d  |  Tempo: %02d:%02d  |  Punti: %d", giocatore.getlife(), visualizzaMinuti, visualizzaSecondi, giocatore.getPunteggio());

                    mvprintw(gestoreMappa.livelloCorrente->start_y + Livello::max_y, gestoreMappa.livelloCorrente->start_x, "Premi 'T' per Pausa");

                    for (int b = 0; b < 10; b++) {
                        if (gestoreMappa.livelloCorrente->isBombActive[b]) {
                            bool disegnaBomba = true;
                            int t = gestoreMappa.livelloCorrente->bombTimer[b];

                            // Se siamo nel Blackout (Mutatore 4), nascondiamo la bomba se è troppo lontana
                            if (gestoreMappa.livelloCorrente->mutatore == 4 && !gestoreMappa.livelloCorrente->Portale) {
                                int distX = std::abs(gestoreMappa.livelloCorrente->bombX[b] - giocatore.getX());
                                int distY = std::abs(gestoreMappa.livelloCorrente->bombY[b] - giocatore.getY());
                                if (distX > DIAMETRO_VISIVO || distY > DIAMETRO_VISIVO) disegnaBomba = false;
                            }

                            if (t >= INIZIO_BOMBA_PALPITANTE && t < INIZIO_BOMBA_PANICO) if (t % 8 < 4) disegnaBomba = false;
                            if (t >= INIZIO_BOMBA_PANICO && t < TEMPO_ESPLOSIONE_BOMBA) if (t % 2 == 0) disegnaBomba = false;

                            if (disegnaBomba) {
                                attron(COLOR_PAIR(COLORE_BOMBA) | A_BOLD);
                                mvaddch(gestoreMappa.livelloCorrente->start_y + gestoreMappa.livelloCorrente->bombY[b], gestoreMappa.livelloCorrente->start_x + gestoreMappa.livelloCorrente->bombX[b], 'O');
                                attroff(COLOR_PAIR(COLORE_BOMBA) | A_BOLD);
                            }
                        }
                    }

                    for (int i = 0; i < gestoreMappa.livelloCorrente->contatoreNemici; i++) {
                        // Se siamo nel Blackout (Mutatore 4), nascondiamo i nemici distanti
                        if (gestoreMappa.livelloCorrente->mutatore == 4 && !gestoreMappa.livelloCorrente->Portale) {
                            int nX = gestoreMappa.livelloCorrente->nemici[i]->getX();
                            int nY = gestoreMappa.livelloCorrente->nemici[i]->getY();
                            if (std::abs(nX - giocatore.getX()) > DIAMETRO_VISIVO || std::abs(nY - giocatore.getY()) > DIAMETRO_VISIVO) continue;
                        }
                        gestoreMappa.livelloCorrente->nemici[i]->draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);
                    }

                    bool sulTeletrasporto = (gestoreMappa.livelloCorrente->griglia[giocatore.getY()][giocatore.getX()] == 'T');
                    bool possoLampeggiareTele = sulTeletrasporto && (gestoreMappa.livelloCorrente->tempodiInizio != -1);
                    bool disegnaGiocatore = true;

                    if (possoLampeggiareTele && contatoreFrameX % 4 < 2) disegnaGiocatore = false;
                    if (giocatore.getIsInvincible() && contatoreFrameX % 2 == 0) disegnaGiocatore = false;

                    if (disegnaGiocatore) {
                        if (giocatore.isRadiusBoosted(gestoreMappa.livelloCorrente)) {
                            attron(COLOR_PAIR(ITEM_RARO_ATTIVO) | A_BOLD);
                            mvaddch(gestoreMappa.livelloCorrente->start_y + giocatore.getY(), gestoreMappa.livelloCorrente->start_x + giocatore.getX(), 'P');
                            attroff(COLOR_PAIR(ITEM_RARO_ATTIVO) | A_BOLD);
                        } else {
                            giocatore.draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x, false);
                        }
                    }
                    refresh();
                }

                clear();
            } while (vuoleRicominciare);
        }
        else if (scelta == 1) {
            menuPrincipale.mostraClassifica();
        }
        else if (scelta == 2) {
            chiudiTutto = true;
        }
    }
}


void Game::gestisciFinePartita(bool vittoria, int punteggio) {
    clear();
    attron(COLOR_PAIR(SCELTA_MENU) | A_BOLD);

    if (vittoria) {
        mvprintw(10, 20, "HAI VINTO! TUTTI I LIVELLI COMPLETATI!");
    } else {
        mvprintw(10, 28, "G A M E   O V E R");
    }

    mvprintw(12, 25, "PUNTEGGIO FINALE: %05d", punteggio);
    mvprintw(15, 10, "Inserisci il tuo nome (max 10 lettere, premi INVIO per finire): ");
    attroff(COLOR_PAIR(SCELTA_MENU) | A_BOLD);
    refresh();

    char nome[11];
    for(int i = 0; i < 11; i++) nome[i] = '\0';

    int cursore = 0;

    flushinp();
    timeout(-1);

    while (true) {
        int ch = getch();

        // 1. Invio per confermare (funziona solo se è stato inserito almeno un carattere)
        if ((ch == '\n' || ch == '\r' || ch == KEY_ENTER) && cursore > 0) {
            break;
        }

        // 2. --- GESTIONE TASTO CANCELLA (BACKSPACE) ---
        if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            if (cursore > 0) {
                cursore--;          // Sposta il cursore indietro nell'array
                nome[cursore] = '\0'; // Cancella il carattere in memoria
                mvaddch(16, 25 + cursore, ' '); // Cancella il carattere a schermo stampando uno spazio
                refresh();
            }
            continue; // Salta il resto del ciclo e aspetta il prossimo tasto
        }

        // 3. Accetta solo lettere e previene l'overflow oltre i 10 caratteri
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            if (cursore < 10) {
                if (ch >= 'a' && ch <= 'z') ch -= 32;
                nome[cursore] = ch;

                attron(COLOR_PAIR(SCELTA_MENU) | A_BOLD);
                mvaddch(16, 25 + cursore, ch);
                attroff(COLOR_PAIR(SCELTA_MENU) | A_BOLD);
                refresh();

                cursore++;
            }
        }
    }

    napms(500);

    // 1. CALCOLO DELLA DATA ATTUALE
    time_t t = std::time(nullptr);
    tm* now = std::localtime(&t);
    char dataOdierna[6];
    // Formattiamo giorno e mese (es: "12/06")
    sprintf(dataOdierna, "%02d/%02d", now->tm_mday, now->tm_mon + 1);

    // Struttura locale per l'ordinamento
    struct RecordTemp {
        char n[11];
        int p;
        char d[6];
    };

    RecordTemp arrayScore[100];
    int tot_record = 0;

    // 2. LETTURA DEL VECCHIO FILE
    std::ifstream fileIn("classifica.txt");
    if (fileIn.is_open()) {
        char nTemp[11], dTemp[6];
        int pTemp;
        // Leggiamo massimo 99 record per far spazio al nuovo
        while (fileIn >> nTemp >> pTemp >> dTemp && tot_record < 99) {
            strcpy(arrayScore[tot_record].n, nTemp);
            arrayScore[tot_record].p = pTemp;
            strcpy(arrayScore[tot_record].d, dTemp);
            tot_record++;
        }
        fileIn.close();
    }

    // 3. INSERIMENTO DEL NUOVO RECORD
    strcpy(arrayScore[tot_record].n, nome);
    arrayScore[tot_record].p = punteggio;
    strcpy(arrayScore[tot_record].d, dataOdierna);
    tot_record++;

    // 4. ORDINAMENTO DAL PIÙ ALTO AL PIÙ BASSO
    for (int i = 0; i < tot_record - 1; i++) {
        for (int j = 0; j < tot_record - i - 1; j++) {
            if (arrayScore[j].p < arrayScore[j+1].p) {
                RecordTemp temp = arrayScore[j];
                arrayScore[j] = arrayScore[j+1];
                arrayScore[j+1] = temp;
            }
        }
    }

    // 5. SOVRASCRITTURA DEL FILE (Mantiene solo la Top 50)
    std::ofstream fileOut("classifica.txt"); // Niente ios::app! Questo resetta il file.
    if (fileOut.is_open()) {
        int limite_salvataggi = (tot_record < 50) ? tot_record : 50;

        for (int i = 0; i < limite_salvataggi; i++) {
            fileOut << arrayScore[i].n << " " << arrayScore[i].p << " " << arrayScore[i].d << "\n";
        }
        fileOut.close();
    }

    clear();
    mvprintw(15, 25, "Salvataggio completato!");
    refresh();
    napms(1500);
}