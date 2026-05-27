#ifndef BOMBERMAN_GAME_H
#define BOMBERMAN_GAME_H


class Game {
private:
    void inizializzaGrafica(); // Nasconderemo qui tutta la configurazione dei colori e di pdcurses

public:
    Game();  // Il costruttore avvierà la grafica
    ~Game(); // Il distruttore chiuderà lo schermo alla fine

    void run(); // Questo sarà il "Motore", conterrà il ciclo principale
};


#endif //BOMBERMAN_GAME_H