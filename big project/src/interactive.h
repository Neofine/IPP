/**
 * @file
 * Moduł przetrzymujący funkcje elementarne do przeprowadzenia trybu interaktywnego
 *
 * @author Jakub Panasiuk
 */
#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include "structs.h"
#include "interactive.h"

/**
 * @brief Wywołuje funkcje niezbędne do przeprowadzenia rozgrywki
 * Wypisuje planszę, chowa echo klawiszy, chowa kursor i wywołuje funkcję
 * wczytującą akcje, po jej zakończeniu wywołuje funkcję @ref final_show i
 * włącza ponownie echo oraz kursor
 * @param[in, out] g          – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] board      – wskaźnik na planszę stworzoną przez funkcję
 *                         @ref gamma_board
 */
void interactive(gamma_t *g, char *board);

#endif //INTERACTIVE_H
