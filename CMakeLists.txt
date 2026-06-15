cmake_minimum_required(VERSION 4.0)
project(Bomberman)

set(CMAKE_CXX_STANDARD 20)

# 1. Creiamo l'eseguibile con tutti i file del progetto
add_executable(Bomberman
        main.cpp
        Entity.cpp Entity.h
        Menu.cpp Menu.h
        Mappa.cpp Mappa.h
        Livello.cpp Livello.h
        Player.cpp Player.h
        Costanti.h
        Pausa.cpp Pausa.h
        Item.cpp Item.h
        Game.cpp Game.h
        EnemyRandom.cpp EnemyRandom.h
        EnemyIntelligente.cpp EnemyIntelligente.h
        terminal_config.h
)

# 2. Gestione delle librerie in base al Sistema Operativo
if(WIN32)
    # --- CONFIGURAZIONE WINDOWS (MSYS2 / MinGW) ---
    # Usiamo i tuoi percorsi locali per trovare pdcurses
    include_directories(C:/msys64/mingw64/include/pdcurses)
    link_directories(C:/msys64/mingw64/lib)
    target_link_libraries(Bomberman PRIVATE pdcurses)

else()
    # --- CONFIGURAZIONE LINUX / macOS ---
    # find_package cerca ncurses automaticamente nel sistema
    find_package(Curses REQUIRED)

    # Includiamo gli header trovati da CMake
    include_directories(${CURSES_INCLUDE_DIRS})

    # Colleghiamo la libreria trovata da CMake
    target_link_libraries(Bomberman PRIVATE ${CURSES_LIBRARIES})
endif()