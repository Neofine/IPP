/**
 * @file
 * Moduł zawierający funkcje pomocnicze do modułu gamma.h
 *
 * @author Jakub Panasiuk
 */
#ifndef SUPP_FUNCTIONS_H
#define SUPP_FUNCTIONS_H

#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include "structs.h"

/**
 * @brief zwraca pozycję @p x, @p y na planszy
 * Zamienia zmienne @p x, @p y na pozycję jaką miałoby to pole
 * gdybyśmy każde pole ponumerowali kolejnymi liczbami naturalnymi
 * idąc w sposób: (1, 1), ..., (1, @p width)
 *                (2, 1), ..., (2, @p width)
 *                        ...
 *                (@p height, 1), ..., (@p height, @p width)
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartość równą pozycji pola w podanym numerowaniu
 */
uint32_t position(gamma_t *g, uint32_t x, uint32_t y);

/**
 * @brief Sprawdza czy dane pole jest z obszaru planszy
 * Dla danego pola @p x, @p y, sprawdza czy jest to pole
 * z planszy, czyli czy:
 * - @p x zawiera się w przedziale [1, @p g->width]
 * - @p y zawiera się w przedziale [1, @p g->height]
 * @p x i @p y są zwiększone o 1 w funkcji z której są
 * wywoływane dla uproszczenia kodu
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartość @p true, jeżeli pole jest legalne, w przeciwnym
 * przypadku zwraca @p false
 */
bool is_legal(gamma_t *g, uint32_t x, uint32_t y);

/**
 * @brief Przepisuje odwróconą liczbę na string (jeżeli nie jest nullem) znak po znaku
 * Dla danej liczby @p x, przepisuje ją odwróconą znak po znaku
 * na tablicę @p buffer, @p x może być wielkości do 10^8, jeżeli tablica jest nullem
 * to nie przepisuje, zwraca jedynie liczbę znaków liczby
 * @param[in] x           – liczba dodatnia mniejsza lub równa wartości
 *                          @p players z funkcji @ref gamma_new
 * @param[in, out] buffer – temporalna tablica charów stworzona tylko
 *                          do tej funkcji
 * @return Wartość @p size, równą ilości znaków liczby @p x
 */
uint32_t to_str(uint32_t x, unsigned char *buffer);

/**
 * @brief Wypisuje "ERROR @p line" w miejscu kursora
 * @param[in] line    – numer wczytanej linii w inie, liczba dodatnia
 */
void error(uint32_t line);

/**
 * @brief Podczas wstawiania wierzchołka koryguje liczbę obszarów
 * Jeżeli łączy 2 obszary które miały dwóch różnych reprezentatnów to
 * zmniejsza ilość obszarów i ustawia reprezentanta na ten sam wierzchołek
 * @param[in, out] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Wartość @p is_connected, znaczącą czy pole (@p x, @p y) jest połączone
 * z jakimkolwiek innym polem zajmowanym przez tego samego gracza
 */
bool union_with_nearby(gamma_t *g, uint32_t x, uint32_t y, uint32_t player);

/**
 * @brief Koryguje liczbę pól przystających do pól poprzedniego właściciela
 * Usuwając poprzedniego właściciela z pola musimy usunąć ilośc obszary
 * do których pole (@p x, @p y) ewentualnie przystawało
 * @param[in, out] g         – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x         – numer kolumny, liczba nieujemna mniejsza od wartości
 *                        @p width z funkcji @ref gamma_new,
 * @param[in] y         – numer wiersza, liczba nieujemna mniejsza od wartości
 *                        @p height z funkcji @ref gamma_new.
 * @param[in] prev_occ  – numer gracza, liczba dodatnia niewiększa od wartości
 *                        @p players z funkcji @ref gamma_new.
 * @return Wartość @p is_connected, znaczącą czy pole (@p x, @p y) jest połączone
 * z jakimkolwiek innym polem zajmowanym przez tego samego gracza
 */
void sub_adjacent_from_prev(gamma_t *g, uint32_t x, uint32_t y, uint32_t prev_occ);

/**
 * @brief Po dodaniu pola zmienia liczbę pól przystających do właścicieli sąsiadów pola
 * Dodając na puste pole wierzchołek trzeba zmienić liczbę wierzchołków przystających
 * do @p player jak i do innych które przystawały właśnie do tego pola, ta funkcja to
 * wykonuje
 * @param[in, out] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @param[in] player  – numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 */
void manage_adjacent(gamma_t *g, uint32_t x, uint32_t y, uint32_t player);

/**
 * @brief Naprawia ilość pól przystających do @p x, @p y
 * W wyniku funkcji gamma_move program odejmował @p gamer->adjacent,
 * gdyż myślał, że to pole poprzednio nie było zajmowane przez żadnego
 * gracza, ta funkcja koryguje @p gamer->adjacent do poprawnej ilości
 * @param[in, out] g  – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartość @p size, równą ilości znaków liczby @p x
 */
void fix_adjacent(gamma_t *g, uint32_t x, uint32_t y);

/**
 * @brief Wypisuje na tablicę @p out puste pole
 * Czyli pole zawierające @p length - 1 spacji oraz jedną kropkę po nich
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] out     – wskaźnik na tablicę na którą wpisuję to pole
 * @param[in] idx     – wskaźnik na numer komórki w tablicy od której zaczynam
 *                      wpisywać to puste pole
 * @param[in] length  – wielkość pola, liczba niezerowa, równa
 *                      ilości cyfr liczby @p players z funkcji @ref gamma_new
 */
void empty_field(gamma_t *g, char *out, uint32_t *idx, uint32_t length);

/**
 * @brief Wypisuje na tablicę @p out numer gracza który posiada je
 * Czyli na początek wpisuję pewną (może też zerową) ilość spacji, a po
 * nich numer gracza który posiada to pole
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] out     – wskaźnik na tablicę na którą wpisuję to pole
 * @param[in] idx     – wskaźnik na numer komórki w tablicy od której zaczynam
 *                      wpisywać to puste pole
 * @param[in] length  – wielkość pola, liczba niezerowa, równa
 *                      ilości cyfr liczby @p players z funkcji @ref gamma_new
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 */
void non_empty_field(gamma_t *g, char *out, uint32_t *idx, uint32_t length, uint32_t y, uint32_t x);

#endif //SUPP_FUNCTIONS_H
