/**
 * @file
 * Moduł zawierający implementację algorytmu Find & Union
 *
 * @author Jakub Panasiuk
 */
#ifndef DISJOINT_UNION_H
#define DISJOINT_UNION_H

#include <stdint.h>
#include "structs.h"

/**
 * @brief znajduje ojca wierzchołka
 * Dla danego pola @p x, @p y, zamienionego funkcją @ref position
 * znajduje jej ojca, czyli wierzchołka którego reprezentantem jest on sam
 * i zmienia wszystkie wierzchołki na drodze do ojca, żeby ich
 * reprezentantem był również ojciec wierzchołka początkowego,
 * jest ona stworzona w celu optymalizacji kodu, żeby móc szybko stwierdzić
 * czy dwa pola są w tym samym obszarze
 * @param[in, out] g  – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] field   – numer pola wyznaczony dla danych @p x, @p y,
 *                      przez funkcję @ref position, liczba dodatnia
 *                      nie większa od liczby pól na planszy
 * @return wartość ojca wierzchołka ponumerowanego przez funkcję
 * @ref position
 */
uint32_t Find (gamma_t *g, uint32_t field);

/**
 * @brief Łączy ojców dwóch wierzchołków w jednego
 * Dla danych pól ponumerowanych przez funkcję @ref position
 * łączy ich ojców ze sobą, podłącza ojca który ma mniejszą wartość
 * @p depth (maksymalna wysokość drzewa reprezentantów) do drugiego,
 * jeżeli są sobie równe to podłącza pierwszego do drugiego i zwiększa
 * wysokość drzewa reprezentantów o 1
 * @param[in, out] g        – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] first_field   – numer pierwszego pola, wywołany przez inne funkcje
 *                            przez odwołanie się na początek do funkcji @ref position
 *                            liczba dodatnia nie większa od liczby pól na planszy
 * @param[in] second_field  – numer drugiego pola, wywołany przez inne funkcje
 *                            przez odwołanie się na początek do funkcji @ref position
 *                            liczba dodatnia nie większa od liczby pól na planszy
 */
void Union (gamma_t *g, uint32_t first_field, uint32_t second_field);

#endif //DISJOINT_UNION_H
