/**
 * @file
 * Implementacja moduł przetrzymującego funkcje elementarne
 * do przeprowadzenia trybu interaktywnego
 *
 * @author Jakub Panasiuk
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "interactive.h"
#include "structs.h"
#include "gamma.h"
#include "supp_functions.h"
#include "assist_interactive.h"
#include "ansi_and_termios.h"


/**
 * @brief Obsługuje zapytanie o ruch podświetlenia na sąsiednie pole
 * Jeżeli zapytanie o przeniesienie na pole nie spowoduje wyjścia za
 * teren mapy to przechodzi, czyli wyłącza podświetlenie na polu na
 * którym stałem i włącza na polu o które było zapytanie, jeżeli
 * jest niepoprawne to nie zmienia się podświetlane pole i program
 * wypisuje stosowne ostrzeżenie
 * @param[in] g         – wskaźnik na strukturę przechowującą stan gry,
 * @param[in, out] x    – wskaźnik na numer kolumny, liczba dodatnia mniejsza
 *                        równa wartości @p width z funkcji @ref gamma_new,
 * @param[in, out] y    – wskaźnik na numer wiersza, liczba dodatnia mniejsza
 *                        równa wartości @p width z funkcji @ref gamma_new,
 * @param[in] chars     – wskaźnik na tablicę trzech ostatnich
 *                        wczytanych znaków na klawiaturze
 * @param[in] player    – numer gracza który zadaje pytanie o przeniesienie pola
 */
static void move_field(gamma_t *g, uint32_t *x, uint32_t *y, int *chars, uint32_t player) {
    if (g->board[*y][*x] != player) {
        highlight_field(g, *x, *y, "", "");
    }
    else {
        highlight_field(g, *x, *y, cyan_txt_seq(), "");
    }

    *x += move_horizontal(chars[0]);
    *y += move_vertical(chars[0]);
    if (is_legal(g, *x, *y)) {
        move_cursor(1, g->height + 7);
        erase_line();
    }
    else {
        *x -= move_horizontal(chars[0]);
        *y -= move_vertical(chars[0]);
        print_error(g, out_of_bounds);
    }
    highlight_field(g, *x, *y, black_txt_seq(), white_bcg_seq());
}

/**
 * @brief Obsługuje zapytanie o złoty ruch na podświetlane teraz pole
 * Jeżeli zapytanie o złoty ruch będzie legalne, czyli nie spowoduje naruszenia
 * zasad rozgrywki to jest wykonywane, w przeciwnym wypadku program
 * wypisze stosowne ostrzeżenie dlaczego nie pozwolił wykonać tego ruchu
 * @param[in] g         – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x         – numer kolumny, liczba dodatnia mniejsza równa wartości
 *                        @p width z funkcji @ref gamma_new,
 * @param[in] y         – numer wiersza, liczba dodatnia mniejsza równa wartości
 *                        @p width z funkcji @ref gamma_new,
 * @param[in] player    – numer gracza który zadaje pytanie o złoty ruch
 * @return Wartość @p true jeżeli program wykonał polecenie lub
 * @p false jeżeli odmówił
 */
static bool try_golden(gamma_t *g, uint32_t x, uint32_t y, uint32_t player) {
    if (gamma_golden_move(g, player, x - 1, y - 1)) {
        highlight_field(g, x, y, black_txt_seq(), white_bcg_seq());
        return true;
    }
    else {
        golden_error(g, player, x, y);
    }
    return false;
}

/**
 * @brief Obsługuje zapytanie o postawienie pionka na podświetlane teraz pole
 * Jeżeli zapytanie będzie legalne, czyli nie spowoduje naruszenia
 * zasad rozgrywki to jest wykonywane, w przeciwnym wypadku program
 * wypisze stosowne ostrzeżenie dlaczego nie pozwolił wykonać tej czynności
 * @param[in] g         – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x         – numer kolumny, liczba dodatnia mniejsza równa wartości
 *                        @p width z funkcji @ref gamma_new,
 * @param[in] y         – numer wiersza, liczba dodatnia mniejsza równa wartości
 *                        @p width z funkcji @ref gamma_new,
 * @param[in] player    – numer gracza który zadaje pytanie o postawienie piona
 * @return Wartość @p true jeżeli program wykonał polecenie lub
 * @p false jeżeli odmówił
 */
static bool try_regular(gamma_t *g, uint32_t x, uint32_t y, uint32_t player) {
    if (gamma_move(g, player, x - 1, y - 1)) {
        highlight_field(g, x, y, black_txt_seq(), white_bcg_seq());
        return true;
    }
    else {
        move_error(g, player, x, y);
    }
    return false;
}

/**
 * @brief Wczytuje akcje gracza oraz na nie reaguje
 * Rozpoczynając w polu (@p x, @p y) wykonuje całą interakcję z
 * graczem nr @p player aż do postawienia pionka przez gracza, spasowania, lub
 * wczytania kodu klawisza Ctrl + D który kończy całą rozgrywkę
 * @param[in] chars     – wskaźnik na tablicę trzech ostatnich
 *                        wczytanych znaków na klawiaturze
 * @param[in] g         – wskaźnik na strukturę przechowującą stan gry,
 * @param[in, out] x    – wskaźnik na numer kolumny, liczba dodatnia mniejsza
 *                        równa wartości @p width z funkcji @ref gamma_new,
 * @param[in, out] y    – wskaźnik na numer wiersza, liczba dodatnia mniejsza
 *                        równa wartości @p width z funkcji @ref gamma_new,
 * @param[in] player    – numer gracza który wykonuje akcje
 * @return Wartość @p true jeżeli program wczytał kod klawisza Ctrl + D,
 * @p false w przeciwnym wypadku
 */
static bool read_moves(int *chars, gamma_t *g, __uint32_t *x, uint32_t *y, __uint32_t player) {
    chars[0] = chars[1] = chars[2] = 0;
    while(1) {
        chars[0] = getchar();
        if (chars[0] == 4) {
            free(chars);
            return 1;
        }
        else if (skip_turn(chars)) {
            move_cursor(1, g->height + 7);
            erase_line();
            break;
        }
        else if (chars[0] == ' ') {
            if (try_regular(g, *x, *y, player))
                break;
        }
        else if (chars[0] == 'g' || chars[0] == 'G') {
            if (try_golden(g, *x, *y, player))
                break;
        }
        else if (chars[2] == 27 && chars[1] == 91) {
            move_field(g, x, y, chars, player);
        }
        chars[2] = chars[1];
        chars[1] = chars[0];
    }
    highlight_owned(g, player, 1);
    return 0;
}

/**
 * @brief Przeprowadza całą rozgrywkę, aż do jej zatrzymania
 * Dopóki jakikolwiek gracz może postawić w pewnym punkcie swój
 * pionek lub do czasu naciśnięcia Ctrl + D przeprowadza rozgrywkę,
 * czyli iteruje się po graczach i po znalezieniu takiego który
 * może postawić gdzieś pionek, wypisuje jego numer i statystyki
 * pod planszą, podświetla wszystkie jego pionki oraz wczytuje jego
 * ruchy
 * @param[in] g         – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x         – numer kolumny, liczba dodatnia mniejsza równa wartości
 *                        @p width z funkcji @ref gamma_new,
 * @param[in] y         – numer wiersza, liczba dodatnia mniejsza równa wartości
 *                        @p width z funkcji @ref gamma_new,
 * @return Wartość @p true jeżeli program wczytał kod klawisza Ctrl + D,
 * @p false w przeciwnym wypadku
 */
static void play_game(gamma_t *g, uint32_t x, uint32_t y) {
    int *chars = calloc(3, sizeof(int));
    while(1) {
        bool players_move = false;
        for (uint32_t i = 1; i <= g->players; i++) {
            bool golden_possible = gamma_golden_possible(g, i);
            uint64_t free_fields = gamma_free_fields(g, i);

            if (free_fields || golden_possible) {
                players_move = true;

                highlight_owned(g, i, 0);
                highlight_field(g, x, y, black_txt_seq(), white_bcg_seq());
                print_player(g, i, free_fields, golden_possible);

                if (read_moves(chars, g, &x, &y, i))
                    return;
            }
        }
        if (!players_move) {
            free(chars);
            return;
        }
    }
}

void interactive(gamma_t *g, char *board) {
    struct termios oldattr, newattr;
    if (tcgetattr(STDIN_FILENO, &oldattr))
        exit(1);
    newattr = oldattr;
    beginArrowRead(newattr);

    uint32_t x, y;
    x = (g->width + 1) / 2;
    y = (g->height + 1) / 2;

    hide_cursor();
    turn_echo_off();
    clear();
    print_coords(x, y);
    printf("%s", board);
    highlight_field(g, x, y, black_txt_seq(), white_bcg_seq());

    play_game(g, x, y);

    final_show(g);
    show_cursor();
    turn_echo_on();
    endArrowRead(oldattr);
}
