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
    //Per attivare l'effetto lampeggiante
    PDC_set_blink(TRUE);

    init_pair(MURO, COLOR_WHITE, COLOR_BLACK);
    init_pair(MURO_DISTRUTTIBILE, 28, COLOR_BLACK);

    //Colori Titolo
    init_color(LAYER_4, 500, 0, 0); //Rosso scuro (un po' puzzolente)
    init_color(LAYER_2, 1000, 500, 0); //Arancione (forse) (non lo distinguo)

    init_pair(LAYER_1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(LAYER_2, LAYER_2 , COLOR_BLACK); //oppure 208
    init_pair(LAYER_3, COLOR_RED, COLOR_BLACK);
    init_pair(LAYER_4, LAYER_4, COLOR_BLACK);

    init_pair(SCELTA_MENU, COLOR_YELLOW, COLOR_BLACK);

    init_pair(TELETRASPORTO, COLOR_BLUE, COLOR_BLACK);

    init_pair(PORTALE, COLOR_CYAN, COLOR_BLACK);

    cbreak();
    noecho();

    curs_set(0);
    keypad(stdscr, TRUE);

    bool chiudiTutto = false;

    //Ciclo principale del programma
    while (!chiudiTutto) {

        Menu menuPrincipale;

        //Funzione che disegna il menu e rimane attiva fino a che non si esegue una scelta
        int scelta = menuPrincipale.gestisciInput();

        //Se si sceglie gioca
        // ... (codice precedente del main) ...

        // Se si sceglie gioca
        if (scelta == 0) {
            clear();
            Mappa gestoreMappa;

            // facciamo nascere il giocatore e i nemici
            Player giocatore(1, 1, '@', 3);

            // CREAZIONE DELLE DUE LISTE SEPARATE
            Enemy* nemiciX[25];
            int contatoreX = 0;

            Enemy2* nemiciZ[25];
            int contatoreZ = 0;

            int numeroNemici = 2 + gestoreMappa.livelloCorrente->idLivello;

            for (int i = 0; i < numeroNemici; i++) {
                int randY, randX;
                do {
                    randY = (rand() % (Livello::max_y - 2)) + 1;
                    randX = (rand() % (Livello::max_x - 2)) + 1;
                } while (gestoreMappa.livelloCorrente->griglia[randY][randX] != ' ');

                if (i % 2 == 0) {
                    nemiciX[contatoreX] = new Enemy(randX, randY, 'X');
                    contatoreX++;
                } else {
                    nemiciZ[contatoreZ] = new Enemy2(randX, randY, 'Z');
                    contatoreZ++;
                }
            }

            //Disegna la mappa (Questo fa calcolare a Simone start_y e start_x)
            gestoreMappa.livelloCorrente->disegna();

            //Disegna il giocatore passandogli l'offset!
            giocatore.draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);

            //Disegna i nemici iniziali
            for (int i = 0; i < contatoreX; i++) {
                nemiciX[i]->draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);
            }
            for (int i = 0; i < contatoreZ; i++) {
                nemiciZ[i]->draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);
            }

            refresh(); // Mostra il fotogramma aggiornato

            bool inGioco = true;
            //Serve a non far bloccare il ciclo while se no si fermerebbe
            //a int input=getch
            timeout(100);

            // variabile per rallentare il nemico
            int contatoreFrame = 0;

            while(inGioco) {
                int input = getch();

                /* entra nello switch solo se l'utente ha premuto
                effettivamente qualcosa
                 */
                if (input != ERR) {
                    switch (input) {
                        case ' ': // TASTO SPAZIO
                            giocatore.bomb_placement();
                            break;
                        case 'q':
                            inGioco = false; // Esce dalla partita e torna al menu
                            break;

                        case '+':
                            // 1. Eliminiamo i vecchi nemici prima di cambiare stanza
                            for (int i = 0; i < contatoreX; i++) delete nemiciX[i];
                            for (int i = 0; i < contatoreZ; i++) delete nemiciZ[i];
                            contatoreX = 0; // Azzeriamo i contatori
                            contatoreZ = 0;

                            // 2. Cambiamo stanza
                            gestoreMappa.vaiAlProssimo();

                            // 3. Generiamo i nuovi nemici in base al nuovo idLivello!
                            {
                                int nuoviNemici = 2 + gestoreMappa.livelloCorrente->idLivello;
                                for (int i = 0; i < nuoviNemici; i++) {
                                    int randY, randX;
                                    do {
                                        randY = (rand() % (Livello::max_y - 2)) + 1;
                                        randX = (rand() % (Livello::max_x - 2)) + 1;
                                    } while (gestoreMappa.livelloCorrente->griglia[randY][randX] != ' ');

                                    if (i % 2 == 0) {
                                        nemiciX[contatoreX] = new Enemy(randX, randY, 'X');
                                        contatoreX++;
                                    } else {
                                        nemiciZ[contatoreZ] = new Enemy2(randX, randY, 'Z');
                                        contatoreZ++;
                                    }
                                }
                            }
                            break;

                        case '-':
                            // Stessa logica: Pulizia -> Cambio Stanza -> Nuova Generazione
                            for (int i = 0; i < contatoreX; i++) delete nemiciX[i];
                            for (int i = 0; i < contatoreZ; i++) delete nemiciZ[i];
                            contatoreX = 0;
                            contatoreZ = 0;

                            gestoreMappa.tornaAlPrecedente();

                            {
                                int nuoviNemici = 2 + gestoreMappa.livelloCorrente->idLivello;
                                for (int i = 0; i < nuoviNemici; i++) {
                                    int randY, randX;
                                    do {
                                        randY = (rand() % (Livello::max_y - 2)) + 1;
                                        randX = (rand() % (Livello::max_x - 2)) + 1;
                                    } while (gestoreMappa.livelloCorrente->griglia[randY][randX] != ' ');

                                    if (i % 2 == 0) {
                                        nemiciX[contatoreX] = new Enemy(randX, randY, 'X');
                                        contatoreX++;
                                    } else {
                                        nemiciZ[contatoreZ] = new Enemy2(randX, randY, 'Z');
                                        contatoreZ++;
                                    }
                                }
                            }
                            break;

                        default:
                            // Se premo le freccette (o altri tasti), muovo il giocatore
                            giocatore.move(input, gestoreMappa.livelloCorrente);
                            break;
                    }
                }

                // Controlla se attivare il teletrasporto
                giocatore.check_teleport(gestoreMappa.livelloCorrente);

                //Gestione tempo bomba
                giocatore.tickBomb(); // Fa scorrere la miccia di 1 frame

                // Se sono passati 30 frame (3 secondi), la bomba esplode
                if (giocatore.getIsBombActive() == true && giocatore.getBombTimer() >= 30) {

                    int bX = giocatore.getBombX();
                    int bY = giocatore.getBombY();

                    //Definiamo le 5 coordinate dell'esplosione a croce (Centro, Su, Giù, Sinistra, Destra)
                    int esplosioneX[5] = {bX, bX, bX, bX - 1, bX + 1};
                    int esplosioneY[5] = {bY, bY - 1, bY + 1, bY, bY};

                    // effetto visivo della fiammata
                    attron(COLOR_PAIR(LAYER_2) | A_BOLD);
                    for (int dir = 0; dir < 5; dir++) {
                        //Disegna il fuoco '#', ma non disegnarlo sopra i muri indistruttibili 'M'
                        if (gestoreMappa.livelloCorrente->griglia[esplosioneY[dir]][esplosioneX[dir]] != 'M') {
                            mvaddch(gestoreMappa.livelloCorrente->start_y + esplosioneY[dir],
                                    gestoreMappa.livelloCorrente->start_x + esplosioneX[dir], '#');
                        }
                    }
                    attroff(COLOR_PAIR(LAYER_2) | A_BOLD);
                    refresh();
                    napms(150); //Mettiamo in pausa il gioco per 0.15 secondi per far vedere l'esplosione



                    //Logica dei danni e distruzione
                    for (int dir = 0; dir < 5; dir++) {
                        int eX = esplosioneX[dir];
                        int eY = esplosioneY[dir];

                        //Distruzione Muri 'D'
                        if (gestoreMappa.livelloCorrente->griglia[eY][eX] == 'D') {

                            gestoreMappa.livelloCorrente->griglia[eY][eX] = ' '; // Il muro sparisce!

                            gestoreMappa.livelloCorrente->controllaEsplosione(eY,eX); //Controllo se è il muro del portale
                        }

                        //Danno al Giocatore (Fuoco amico)
                        if (giocatore.getX() == eX && giocatore.getY() == eY) {
                            giocatore.take_damage();
                            if (giocatore.getlife() > 0) {
                                giocatore.reset_position();
                            } else {
                                // GAME OVER
                                clear();
                                mvprintw(Livello::max_y / 2, Livello::max_x / 2 - 5, "G A M E   O V E R");
                                refresh();
                                napms(2000);
                                inGioco = false;
                            }
                        }

                        //Uccisione Nemici X
                        for (int k = 0; k < contatoreX; k++) {
                            if (nemiciX[k]->getX() == eX && nemiciX[k]->getY() == eY) {
                                delete nemiciX[k]; // Uccido il nemico fisicamente
                                // Sposto tutti gli altri in avanti per chiudere il buco nella lista
                                for (int j = k; j < contatoreX - 1; j++) {
                                    nemiciX[j] = nemiciX[j + 1];
                                }
                                contatoreX--; //Ho un nemico 'X' in meno
                                k--; //Ricontrollo questo indice al prossimo giro
                            }
                        }

                        //Uccisione Nemici Z
                        for (int k = 0; k < contatoreZ; k++) {
                            if (nemiciZ[k]->getX() == eX && nemiciZ[k]->getY() == eY) {
                                delete nemiciZ[k]; // Uccido il nemico fisicamente
                                // Sposto tutti gli altri in avanti
                                for (int j = k; j < contatoreZ - 1; j++) {
                                    nemiciZ[j] = nemiciZ[j + 1];
                                }
                                contatoreZ--; //Ho un nemico 'Z' in meno
                                k--; // Ricontrollo questo indice al prossimo giro
                            }
                        }
                    }

                    // Alla fine di tutto, la bomba viene disinnescata e si può piazzare di nuovo
                    giocatore.resetBomb();
                }
                // gestione nemico
                contatoreFrame++;
                // Se sono passati 5 frame (0.5 secondi), il mostro fa un passo
                if (contatoreFrame >= 5) {
                    // Movimento delle due liste separate
                    for (int i = 0; i < contatoreX; i++) {
                        nemiciX[i]->move(gestoreMappa.livelloCorrente);
                    }
                    for (int i = 0; i < contatoreZ; i++) {
                        nemiciZ[i]->move(gestoreMappa.livelloCorrente);
                    }

                    contatoreFrame = 0; // Azzero il contatore
                }

                //inizio controllo collisioni
                bool colpito = false;

                //Controllo se una 'X' mi ha toccato
                for (int i = 0; i < contatoreX; i++) {
                    if (giocatore.getX() == nemiciX[i]->getX() && giocatore.getY() == nemiciX[i]->getY()) {
                        colpito = true;
                    }
                }
                //Controllo se una 'Z' mi ha toccato
                for (int i = 0; i < contatoreZ; i++) {
                    if (giocatore.getX() == nemiciZ[i]->getX() && giocatore.getY() == nemiciZ[i]->getY()) {
                        colpito = true;
                    }
                }

                //Conseguenze dello scontro
                if (colpito == true) {
                    giocatore.take_damage(); // Tolgo una vita

                    if (giocatore.getlife() > 0) {
                        giocatore.reset_position(); // Torna all'inizio se ha ancora vite
                    } else {
                        // GAME OVER: Vite finite.
                        clear();
                        mvprintw(Livello::max_y / 2, Livello::max_x / 2 - 5, "G A M E   O V E R");
                        refresh();
                        napms(2000); // Aspetta 2 secondi per farti leggere la scritta
                        inGioco = false; // Interrompe la partita e torna al Menu
                    }
                }


                erase(); //ho cambiato clear() con erase() per il problema dello sfarfallio

                //Disegna la mappa (Questo fa calcolare a Simone start_y e start_x)
                gestoreMappa.livelloCorrente->disegna();

                if (giocatore.getIsBombActive() == true) {
                    attron(COLOR_PAIR(LAYER_3) | A_BOLD); // Usiamo il rosso per la bomba
                    mvaddch(gestoreMappa.livelloCorrente->start_y + giocatore.getBombY(),
                            gestoreMappa.livelloCorrente->start_x + giocatore.getBombX(), 'O');
                    attroff(COLOR_PAIR(LAYER_3) | A_BOLD);
                }

                // stampa le vite sullo schermo
                mvprintw(gestoreMappa.livelloCorrente->start_y - 1, gestoreMappa.livelloCorrente->start_x + 1, "Vite: %d", giocatore.getlife());

                // Disegna i nemici aggiornati
                for (int i = 0; i < contatoreX; i++) {
                    nemiciX[i]->draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);
                }
                for (int i = 0; i < contatoreZ; i++) {
                    nemiciZ[i]->draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);
                }

                //Disegna il giocatore passandogli l'offset!
                giocatore.draw(gestoreMappa.livelloCorrente->start_y, gestoreMappa.livelloCorrente->start_x);

                refresh();
            }

            // Pulizia memoria
            for (int i = 0; i < contatoreX; i++) {
                delete nemiciX[i];
            }
            for (int i = 0; i < contatoreZ; i++) {
                delete nemiciZ[i];
            }

            clear();
        }

        //Se si sceglie classifica
        else if (scelta == 1) {
            clear();
            mvprintw(10, 10, "Schermata Classifica in costruzione! Premi un tasto per tornare indietro...");
            refresh();
            timeout(-1); //Per non far uscire dalla schermata della classifica
            getch();
            clear();
        }
        //Se si sceglie esci
        else if (scelta == 2) {
            //Interrompe il ciclo while
            chiudiTutto = true;
        }
    }

    endwin();
    return 0;
}