/**
 * @file
 * Implementacja modułu przetrzymującego funkcje elementarne do przeprowadzenia
 * trybu wsadowego
 *
 * @author Jakub Panasiuk
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/ioctl.h>
#include "supp_functions.h"
#include "batch.h"
#include "gamma.h"
#include "interactive.h"

/**
 * @brief Zwraca maksymalną z dwóch liczb
 * @param[in] a, b       – liczby nieujemne
 * @return zwraca większą liczbę
 */
static uint32_t max(uint32_t a, uint32_t b) {
    if (a > b)
        return a;
    return b;
}

/**
 * @brief Wykonuje dobrze wywołaną komendę B lub I
 * Jeżeli @p g jest NULL'em to próbuje wywołać funkcję @ref gamma_new
 * jeżeli się to powiedzie to plansza będąca wynikiem @ref gamma_new będzie
 * planszą na której będzie się rozgrywała gra, jeżeli wczytana została
 * komenda I to jeżeli wielkość terminala będzie odpowiednia to wchodzi do trybu
 * interaktywnego
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] line    – numer linii którą z wejścia obsługuję, liczba dodatnia,
 * @param[in] nums    – tablica 4 liczb które były wczytane w tej linii,
 * @param[in] command – wczytana komenda, równa 'I' lub 'B'
 * @return 1 jeżeli @p g nie jest NULL'em, 0 jeżeli z sukcesem udało się stworzyć
 * i rozegrać tryb interaktywny, 2 jeżeli funkcja @ref gamma_new zwróciła NULL'a, lub
 * jeżeli udało się stworzyć poprawnie grę w trybie batchowym, dodatkowo jeżeli
 * gracz próbował wywołać tryb interaktywny w zbyt małym terminalu to wypisuje
 * odopwiedni komunikat i robi exit(1)
 */
static int new_game(gamma_t **g, uint32_t line, uint32_t *nums, int command) {
    if (*g != NULL) {
        error(line);
        return 1;
    }

    (*g) = gamma_new(nums[1], nums[2], nums[3], nums[4]);
    if (*g == NULL) {
        error(line);
    }
    else if (command == 'B') {
        printf("OK %d\n", line);
    }
    else {
        char *p = gamma_board(*g);
        if (p == NULL) {
            error(line);
            free(p);
        }
        else {
            struct winsize w;
            ioctl(0, TIOCGWINSZ, &w);
            int length = snprintf(NULL, 0, "%" PRIu32, (*g)->players);
            uint32_t term_height, term_width;

            // dodaję 7 bo tyle linii dodatkowych program wypisuje podczas gry
            term_height = (*g)->height + 7;

            // tutaj maksuję z 62, gdyż tak długi jest największy warning
            if ((*g)->players < 10)
                term_width = max((*g)->width, 62);
            else
                term_width = max((*g)->width * (length + 1), 62);

            if (term_width <= w.ws_col && term_height <= w.ws_row) {
                interactive(*g, p);
                free(p);
                return 0;
            }
            else {
                printf("Terminal is too small, try changing it to the size: %d x %d\n",
                        term_width, term_height);
                exit(1);
            }
        }
    }
    return 2;
}

/**
 * @brief Wykonuje dobrze wywołaną komendę m lub g
 * Jeżeli @p g nie jest nullem to próbuje wykonać podaną komendę wstawienia piona,
 * wypisuje na wyjście 0 gdy podana komenda nie może zostać zaaplikowana do rozgrywki
 * lub 1 gdy udało się poprawnie komendę wykonać
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] line    – numer linii którą z wejścia obsługuję, liczba dodatnia,
 * @param[in] nums    – tablica 4 liczb które były wczytane w tej linii,
 * @param[in] command – wczytana komenda, równa 'I' lub 'B'
 */
static void try_move(gamma_t **g, uint32_t line, uint32_t *nums, int command) {
    if (*g == NULL)
        error(line);
    else if (command == 'm')
        printf("%d\n", gamma_move(*g, nums[1], nums[2], nums[3]));
    else
        printf("%d\n", gamma_golden_move(*g, nums[1], nums[2], nums[3]));
}

/**
 * @brief Wykonuje dobrze wywołaną komendę b, f lub q
 * Jeżeli @p g nie jest nullem to próbuje wykonać podaną komendę, wypisuje
 * odpowiedzi na zapytania zadane na wyjściu
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] line    – numer linii którą z wejścia obsługuję, liczba dodatnia,
 * @param[in] nums    – tablica 4 liczb które były wczytane w tej linii,
 * @param[in] command – wczytana komenda, równa 'I' lub 'B'
 */
static void ask_game(gamma_t **g, uint32_t line, uint32_t *nums, int command) {
    if (*g == NULL)
        error(line);
    else if (command == 'b')
        printf("%" PRId64 "\n", gamma_busy_fields(*g, nums[1]));
    else if (command == 'f')
        printf("%" PRId64 "\n", gamma_free_fields(*g, nums[1]));
    else
        printf("%d\n", gamma_golden_possible(*g, nums[1]));
}

/**
 * @brief Wykonuje dobrze wywołaną komendę p
 * Jeżeli @p g nie jest nullem to próbuje wykonać podaną komendę, wypisuje
 * planszę rozgrywki, jeżeli natomiast jest to wypisuje "ERROR" i aktualnie
 * wczytywaną linię
 * @param[in] g       – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] line    – numer linii którą z wejścia obsługuję, liczba dodatnia
 */
static void try_print(gamma_t **g, uint32_t line) {
    char *p = gamma_board(*g);
    if (p == NULL)
        error(line);
    else {
        printf("%s", p);
        free(p);
    }
}

bool read_in(gamma_t **g, int line, uint32_t *nums) {
    if (nums == NULL)
        {error(line); return 0;}

    int c;
    uint32_t numCnt = 0;

    bool prevBlank = false;
    bool printError = false;
    for (uint32_t i = 1; i <= 4; i++)
        nums[i] = 0;

    int command = getchar();
    if (command == '\n')
        return 1;
    if (command < 0)
        printError = true;

    for (int i = 0; (c = getchar()) != '\n' && c != EOF; i++) {
        if ((i == 0 && (!isspace(c))) || (!isspace(c) && isdigit(c) == 0) || c < 0) {printError = true;}
        else if (isspace(c)) {prevBlank = true; continue;}
        else if (!isspace(c) && prevBlank) {prevBlank = false, numCnt++;}
        if (numCnt <= 4 && (uint64_t)((uint32_t)(nums[numCnt] * 10 + c - '0')) == (uint64_t)((uint64_t)nums[numCnt] * 10 + c - '0'))
            nums[numCnt] = nums[numCnt] * 10 + c - '0';
        else printError = true;
    }

    if (c == EOF && command != EOF) {
        if (command != '#')
            error(line);
        return 0;
    }
    else if (command == EOF) {
        return 0;
    }
    else if (command == '#') {
        return 1;
    }
    else if (printError) {
        error(line);
    }
    else if (numCnt == 4 && (command == 'B' || command == 'I')) {
        int outcome = new_game(g, line, nums, command);
        if (outcome != 2)
            return outcome;
    }
    else if (numCnt == 3 && (command == 'm' || command == 'g')) {
        try_move(g, line, nums, command);
    }
    else if (numCnt == 1 && (command == 'b' || command == 'f' || command == 'q')) {
        ask_game(g, line, nums, command);
    }
    else if (numCnt == 0 && command == 'p') {
        try_print(g, line);
    }
    else {
        error(line);
    }

    return 1;
}