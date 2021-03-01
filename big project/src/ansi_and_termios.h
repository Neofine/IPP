/**
 * @file
 * Moduł przetrzymujący funkcje używające ansi escape codes lub
 * termios
 *
 * @author Jakub Panasiuk
 */
#ifndef ANSI_AND_TERMIOS_H
#define ANSI_AND_TERMIOS_H

#include <inttypes.h>
#include <termios.h>

/**
 * @brief Chowa kursor
 */
void hide_cursor();

/**
 * @brief Pokazuje kursor
 */
void show_cursor();

/**
 * @brief Usuwa tekst w linii od kursora aż do jej końca
 */
void erase_line();

/**
 * @brief Przesuwa kursor na pozycję (@p x, @p y)
 * @param[in] x          – numer kolumny, liczba dodatnia
 * @param[in] y          – numer wiersza, liczba dodatnia
 */
void move_cursor(uint32_t x, uint32_t y);

/**
 * @brief Zapisuje pozycję kursora
 */
void save_cursor();

/**
 * @brief Przywraca zapisaną pozycję kursora
 */
void restore_cursor();

/**
 * @brief Czyści ekran
 */
void clear();

/**
 * @brief Usuwa wszelkie kolorowanie tekstu i tła wywołane do tej pory
 */
void clear_color();

/**
 * @brief Sprawia, że każdy wypisany teraz test będzie w kolorze złotym/żółtym
 */
void print_gold();

/**
 * @brief Sprawia, że każdy wypisany teraz test będzie w kolorze czerwonym
 */
void print_red();

/**
 * @brief Sprawia, że każdy wypisany teraz test będzie w kolorze cyjanu
 */
void print_cyan();

/**
 * @brief Sprawia, że każdy wypisany teraz test będzie w kolorze zielonym
 */
void print_green();

/**
 * @brief Po wypisaniu wyniku funkcji dalszy tekst będzie w kolorze cyjanu
 * @return kod z ansi escape sequences odpowiadający cyjanowemu kolorowi tekstu
 */
char *cyan_txt_seq();

/**
 * @brief Po wypisaniu wyniku funkcji dalszy tekst będzie w kolorze czarnym
 * @return kod z ansi escape sequences odpowiadający czarnemu kolorowi tekstu
 */
char *black_txt_seq();

/**
 * @brief Po wypisaniu wyniku funkcji dalsze tło tekstu będzie koloru białego
 * @return kod z ansi escape sequences odpowiadający tłu kolorowi białego
 */
char *white_bcg_seq();

/**
 * @brief Wyłącza echo wciśniętych klawiszy w terminalu
 * wywoływany przy wychodzeniu z interactive mode, jeżeli ta komenda się
 * nie uda, (co jest bardzo mało prawdopodobne) to wychodzi z programu
 * zwracając wartość 1
 */
void turn_echo_off();

/**
 * @brief Włącza echo wciśniętych klawiszy w terminalu
 * wywoływany przy wychodzeniu z interactive mode, jeżeli ta komenda się
 * nie uda, (co jest bardzo mało prawdopodobne) to wychodzi z programu
 * zwracając wartość 1
 */
void turn_echo_on();

/**
 * @brief Umożliwia programowi wczytywanie znaków specjalnych
 * wywoływany przy wchodzeniu do interactive mode, jeżeli ta komenda się
 * nie uda, (co jest bardzo mało prawdopodobne) to wychodzi z programu
 * zwracając wartość 1
 */
void beginArrowRead(struct termios newattr);

/**
 * @brief Kończy możliwość wczytywania znaków specjalnych
 * wywoływany przy wychodzeniu z interactive mode, jeżeli ta komenda się
 * nie uda, (co jest bardzo mało prawdopodobne) to wychodzi z programu
 * zwracając wartość 1
 */
void endArrowRead(struct termios oldattr);

/**
 * @brief Wypisuje koordynaty (@p x-1, @p y-1) które są obecnie podświetlane
 * Wypisuje je nad planszą w kolorze cyanu
 * @param[in] x          – numer kolumny, liczba dodatnia mniejsza równa wartości
 *                         @p width z funkcji @ref gamma_new,
 * @param[in] y          – numer wiersza, liczba dodatnia mniejsza równa wartości
 *                         @p width z funkcji @ref gamma_new,
 */
void print_coords(uint32_t x, uint32_t y);

/**
 * @brief Zmienia wypisywane koordynaty na (@p x, @p y)
 * Nad wypisaną planszą w programie wypisywane są koordynaty pola które jest obecnie
 * podświetlane, więc gdy funkcja zmienia podświetlenie to zmienia też te koordynaty,
 * @param[in] x          – numer kolumny, liczba dodatnia mniejsza równa wartości
 *                         @p width z funkcji @ref gamma_new,
 * @param[in] y          – numer wiersza, liczba dodatnia mniejsza równa wartości
 *                         @p width z funkcji @ref gamma_new,
 */
void update_cords(uint32_t x, uint32_t y);

#endif //ANSI_AND_TERMIOS_H
