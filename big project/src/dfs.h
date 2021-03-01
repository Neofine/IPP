/**
 * @file
 * Moduł zawierający implementację algorytmu DFS
 *
 * @author Jakub Panasiuk
 */
#ifndef DFS_H
#define DFS_H

#include <stdint.h>
#include "structs.h"
#include "supp_functions.h"


/**
 * @brief Zmienia reprezentantów obszarów których usunięcie spowoduje ich rozłączenie
 * Dla każdego z nieodwiedzonych algorytmem @ref dfs sąsiadów zajmowanego przez
 * poprzedniego  właściciela wykonuje @ref dfs oraz zmienia ich reprezentantów
 * i ewentualnie modyfikujue ilość obszarów poprzedniego właściciela
 * @param[in, out] g         – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x         – numer kolumny, liczba nieujemna mniejsza od wartości
 *                        @p width z funkcji @ref gamma_new,
 * @param[in] y         – numer wiersza, liczba nieujemna mniejsza od wartości
 *                        @p height z funkcji @ref gamma_new.
 * @param[in] prev_occ  – numer gracza, liczba dodatnia niewiększa od wartości
 *                        @p players z funkcji @ref gamma_new.
 */
void dfs_adjacent(gamma_t *g, uint32_t x, uint32_t y, uint32_t prev_occ);

/**
 * @brief Robi algorytm DFS dla wierzchołka @p x, @p y
 * Dla danego pola @p x, @p y, oraz gracza @p player, przechodzi
 * po wszystkich polach które nie były odwiedzone i należą do tego gracza,
 * nadaje im nowego ojca, zmienia głębokość i oznacza jako odwiedzone
 * @param[in, out] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x            – numer kolumny, liczba nieujemna mniejsza od wartości
 *                           @p width z funkcji @ref gamma_new,
 * @param[in] y            – numer wiersza, liczba nieujemna mniejsza od wartości
 *                           @p height z funkcji @ref gamma_new.
 * @param[in] parent       – numer wierzchołka nadany przez funkcję
 *                           @ref position, dla @p x, @p y, liczba dodatnia
 *                          nie większa od ilości pól na planszy
 * @param[in] player       – numer gracza, liczba dodatnia mniejsza lub równa wartości
 *                           @p players z funkcji @ref gamma_new
 */
void dfs (gamma_t *g, uint32_t x, uint32_t y, uint32_t parent, uint32_t player);

/**
 * @brief Robi algortym DFS przechodząc po wszystkich odwiedzonych
 * Dla danego pola @p x, @p y, przechodzi po wszystkich polach
 * które były odwiedzone i oznacza jako nieodwiedzone
 * by ponownie móc funkcję użyć w następnych jej wywołaniach
 * @param[in, out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x        – numer kolumny, liczba nieujemna mniejsza od wartości
 *                       @p width z funkcji @ref gamma_new,
 * @param[in] y        – numer wiersza, liczba nieujemna mniejsza od wartości
 *                       @p height z funkcji @ref gamma_new.
 */
void dedfs (gamma_t *g, uint32_t x, uint32_t y);

#endif //DFS_H
