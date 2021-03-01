/**
 * @file
 * Moduł przetrzymujący funkcje elementarne do przeprowadzenia trybu wsadowego
 *
 * @author Jakub Panasiuk
 */
#ifndef BATCH_H
#define BATCH_H

#include <stdbool.h>
#include "structs.h"

/**
 * @brief Wczytuje pojedynczą linię podaną na wejściu i reaguje na nią
 * @param[in, out] g          – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] line             – numer wczytanego wiersza, liczba dodatnia
 * @param[in, out] nums       – wskaźnik na tablicę intów na którą będę
 *                              zapisywał wczytane liczby na wejściu
 * @return wartość @p true jeżeli w linii nie znalazł się wiersz ze znakiem EOF
 * @p false w przeciwnym wypadku
 */
bool read_in(gamma_t **g, int line, uint32_t *nums);

#endif //BATCH_H
