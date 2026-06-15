//
// Created by Utente on 15/06/2026.
//

#ifndef BOMBERMAN_TERMINAL_CONFIG_H
#define BOMBERMAN_TERMINAL_CONFIG_H

#ifdef _WIN32
    #include <pdcurses.h>
#else
    #include <ncurses.h> // oppure <curses.h>
#endif

#endif //BOMBERMAN_TERMINAL_CONFIG_H