#include "Game.h"

int main() {
    Game bomberman;

    bomberman.run();

    // Il distruttore ~Game() chiamerà in automatico endwin() per chiudere la finestra in modo pulito.
    return 0;
}