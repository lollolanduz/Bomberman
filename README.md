# Bomberman ASCII

Un'implementazione C++ del classico videogioco arcade Bomberman, giocabile interamente da terminale. Sviluppato come progetto per l'esame di Programmazione (A.A. 2025/2026).

## 👥 Membri del Gruppo
* **Simone Iorio** - simone.iorio3@studio.unibo.it | Numero Matricola 0001222337
* **Lorenzo Landuzzi** - lorenzo.landuzzi3@studio.unibo.it | Numero Matricola 0001213118
* **Riccardo Zuffi** - riccardo.zuffi@studio.unibo.it | Numero Matricola 0001223770

---

## 🎮 Come Giocare
L'obiettivo del gioco è superare tutti e 5 i livelli sconfiggendo i nemici presenti nella mappa. Il giocatore può piazzare bombe per distruggere i muri (verdi) ed eliminare i nemici, ma deve fare attenzione a non farsi colpire dalle proprie esplosioni o dai nemici stessi.

### Comandi Principali
* **W, A, S, D** / **Frecce Direzionali**: Muovi il personaggio.
* **Spazio**: Piazza una bomba.
* **T**: Metti in pausa il gioco (Mostra anche la Legenda).
* **+ / -** : Viaggia tra i livelli (Avanti / Indietro).
* **Invio**: Conferma selezione nei menu.
* **Q / ESC**: Torna indietro / Esci dal menu della Classifica.

### Elementi di Gioco e Simboli
* `P` : Il Giocatore (Player).
* `M` : Muro Indistruttibile (Bianco).
* `D` / `H` : Muro Distruttibile / Muro Duro (Verde).
* `O` : Bomba attiva.
* `X` / `Z` : Nemici standard (movimento casuale e velocità diverse).
* `i` / `I` : Nemico Intelligente (dormiente 'i', si sveglia e insegue 'I' se il giocatore è vicino).
* `0` (Blu) : Teletrasporto (si attiva restandoci sopra per 2 secondi).
* `█` (Rosso) : Portale di uscita (appare solo quando tutti i nemici sono sconfitti).

### Power-Ups (Items)
Distruggendo i muri distruttibili o sconfiggendo i nemici, è possibile trovare degli oggetti:
* **C (Comune)**: Aggiunge 50 Punti extra.
* **R (Raro)**: Aumenta il raggio dell'esplosione delle bombe nel livello corrente.
* **E (Epico)**: Aggiunge una vita extra al giocatore.

### Meccanica Speciale: I Mutatori di Livello
Per aumentare la difficoltà e la rigiocabilità, ogni livello (in base all'ID) ha una probabilità crescente di generarsi con un mutatore ambientale:
1. **Normale:** Regole standard.
2. **Zona Blindata:** Tutte le mura distruttibili diventano mura dure (`H`).
3. **Carestia:** Non viene generato alcun Item (Drop) alla distruzione di muri o nemici.
4. **Blackout:** Il livello è completamente avvolto dall'oscurità e il giocatore ha un raggio visivo limitato a 8 caselle attorno a sé.

---

## ⚙️ Requisiti di Sistema
Per compilare ed eseguire il gioco è necessario avere installato:
* Un compilatore C++ (es. GCC / MinGW) con supporto allo standard C++17.
* La libreria grafica **ncurses** (o **pdcurses** su Windows).
* CMake (versione 3.26 o superiore).

## 🚀 Compilazione ed Esecuzione

### Tramite CLion (Consigliato)
1. Aprire la cartella del progetto con CLion.
2. Assicurarsi di aver configurato correttamente la Toolchain (MinGW su Windows).
3. Cliccare sul pulsante **Build** (martelletto verde).
4. Avviare il gioco tramite il pulsante **Run** (triangolo verde).
   *Nota: L'eseguibile verrà generato all'interno della cartella `cmake-build-debug`.*

### Tramite Terminale (Linux / MacOS)
Se si desidera compilare manualmente tramite riga di comando:
```bash
g++ -std=c++17 main.cpp Game.cpp Menu.cpp Livello.cpp Mappa.cpp Player.cpp Entity.cpp Item.cpp Pausa.cpp EnemyRandom.cpp EnemyIntelligente.cpp -lncurses -o Bomberman
./Bomberman