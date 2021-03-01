/**
 * @file
 * Moduł przetrzymujący struktury używane w programie
 *
 * @author Jakub Panasiuk
 */
#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include <stdint.h>
#include "structs.h"

/**
 * Struktura przechowująca wartości specyficzne dla danego gracza podczas gry.
 */
typedef struct player {
    uint32_t fields; ///< ilość obszarów zajętych przez gracza
    uint32_t adjacent; ///< ilość wolnych pól przyległych do pól gracza
    uint32_t taken;  ///< ilość pól zajętych przez gracza
    bool golden_done; ///< czy gracz wykonał już złoty ruch w tej grze
} player_t;

/**
 * Struktura przechowująca elementy pomocnicze z których funkcje
 * korzystają podczas rozgrywki
 */
typedef struct temporary {
    bool **visited; ///< tablica odwiedzonych wierzchołków
    uint32_t *stack; ///< kolejka wierzchołków w dfs'ie
    uint32_t *parent; ///< tablica reprezentantów wierzchołków (DSU)
    uint32_t *depth; ///< tablica wysokości drzew utworzonych w DSU
} temporary_t;

/**
 * Struktura przechowująca stan gry.
 */
typedef struct gamma {
    uint32_t width; ///< szerokość planszy
    uint32_t height; ///< wysokość planszy
    uint32_t players; ///< ilość graczy
    uint32_t areas; ///< maksymalna ilość zajętych obszarów przez gracza
    uint32_t taken_fields; ///< sumaryczna ilość pól zajętych na planszy
    player_t *gamer; ///< tablica graczy
    temporary_t tools; ///< inicjalizacja struktury pomocniczej
    uint32_t **board; ///< plansza podlegająca modyfikacjom przez graczy
} gamma_t;

#endif //STRUCTS_H
