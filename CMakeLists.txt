cmake_minimum_required(VERSION 4.0)
project(Bomberman)

set(CMAKE_CXX_STANDARD 20)

include_directories(C:/msys64/mingw64/include/pdcurses)

link_directories(C:/msys64/mingw64/lib)

add_executable(Bomberman main.cpp
        Entita.cpp
        Entita.h
)

target_link_libraries(Bomberman PRIVATE pdcurses)