/**
 * @file
 * Implementacja modułu zawierającego funkcje pomocnicze do modułu gamma.h
 *
 * @author Jakub Panasiuk
 */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "supp_functions.h"
#include "gamma.h"
#include "disjoint_union.h"

void error(uint32_t line) {
    fprintf(stderr, "ERROR %d\n", line);
}

uint32_t position(gamma_t *g, uint32_t x, uint32_t y) {
    return (y - 1) * g->width + x;
}

bool is_legal(gamma_t *g, uint32_t x, uint32_t y) {
    return (x <= g->width && x > 0 && y <= g->height && y > 0);
}

bool union_with_nearby(gamma_t *g, uint32_t x, uint32_t y, uint32_t player) {
    // tablica xmove sprawia, że mogę symulować ruchy w pętli, upraszcza kod
    int xmove[] = {1, -1, 0, 0};
    // tablica ymove podobnie, tablice służą symulowaniu ruchów w 4 strony od @p x, @p y
    int ymove[] = {0, 0, -1, 1};
    bool is_connected = 0;
    for (uint32_t i = 0; i < 4; i++) {
        if (is_legal(g, x + xmove[i], y + ymove[i]) &&
            g->board[y + ymove[i]][x + xmove[i]] == player) {
            if (is_connected &&
                Find(g, position(g, x, y)) !=
                Find(g, position(g, x + xmove[i], y + ymove[i])))
                g->gamer[player].fields--;

            Union(g, position(g, x, y), position(g, x + xmove[i], y + ymove[i]));
            is_connected = 1;
        }
    }
    return is_connected;
}

void sub_adjacent_from_prev(gamma_t *g, uint32_t x, uint32_t y, uint32_t prev_occ) {
    // tablica xmove sprawia, że mogę symulować ruchy w pętli, upraszcza kod
    int xmove[] = {1, -1, 0, 0};
    // tablica ymove podobnie, tablice służą symulowaniu ruchów w 4 strony od @p x, @p y
    int ymove[] = {0, 0, -1, 1};
    for (uint32_t i = 0; i < 4; i++) {
        bool duplicate = 0;
        if (is_legal(g, x + xmove[i], y + ymove[i]) &&
            g->board[y + ymove[i]][x + xmove[i]] == 0) {
            for (uint32_t j = 0; j < 4; j++) {
                if ((y + ymove[i] + ymove[j] != y || x + xmove[i] + xmove[j] != x) &&
                    is_legal(g, x + xmove[i] + xmove[j], y + ymove[i] + ymove[j]) &&
                    g->board[y + ymove[i] + ymove[j]][x + xmove[i] + xmove[j]] == prev_occ)

                    duplicate = 1;
            }
            if (!duplicate)
                g->gamer[prev_occ].adjacent--;
        }
    }
}

void manage_adjacent(gamma_t *g, uint32_t x, uint32_t y, uint32_t player) {
    // tablica xmove sprawia, że mogę symulować ruchy w pętli, upraszcza kod
    int xmove[] = {1, -1, 0, 0};
    // tablica ymove podobnie, tablice służą symulowaniu ruchów w 4 strony od @p x, @p y
    int ymove[] = {0, 0, -1, 1};
    for (uint32_t i = 0; i < 4; i++) {
        bool duplicate = 0;

        if (is_legal(g, x + xmove[i], y + ymove[i]) &&
            g->board[y + ymove[i]][x + xmove[i]] == 0) {
            for (uint32_t j = 0; j < 4; j++) {

                if (((y + ymove[i] + ymove[j] != y) ||
                     (x + xmove[i] + xmove[j] != x)) &&
                    is_legal(g, (x + xmove[i] + xmove[j]),
                             (y + ymove[i] + ymove[j])) &&
                        g->board[y + ymove[i] + ymove[j]]
                    [x + xmove[i] + xmove[j]] == player)

                    duplicate = 1;
            }

            if (!duplicate)
                g->gamer[player].adjacent++;
        }
        else if (is_legal(g, x + xmove[i], y + ymove[i]) &&
                g->board[y + ymove[i]][x + xmove[i]] != player &&
                g->board[y + ymove[i]][x + xmove[i]] != 0) {
            bool subtract = 1;
            for (uint32_t j = 0; j < i; j++) {
                if (is_legal(g, x + xmove[j], y + ymove[j]) &&
                    g->board[y + ymove[j]][x + xmove[j]] ==
                    g->board[y + ymove[i]][x + xmove[i]])
                    subtract = 0;
            }
            if (subtract)
                g->gamer[g->board[y + ymove[i]][x + xmove[i]]].adjacent--;
        }
    }
}

void fix_adjacent(gamma_t *g, uint32_t x, uint32_t y) {
    // tablica xmove sprawia, że mogę symulować ruchy w pętli, upraszcza kod
    int xmove[] = {1, -1, 0, 0};
    // tablica ymove podobnie, tablice służą symulowaniu ruchów w 4 strony od @p x, @p y
    int ymove[] = {0, 0, -1, 1};

    for (uint32_t i = 0; i < 4; i++) {
        if (is_legal(g, x + xmove[i], y + ymove[i]) &&
            g->board[y + ymove[i]][x + xmove[i]] != 0) {
            bool subtract = 1;
            for (uint32_t j = 0; j < i; j++) {
                if (is_legal(g, x + xmove[j], y + ymove[j]) &&
                    g->board[y + ymove[j]][x + xmove[j]] ==
                    g->board[y + ymove[i]][x + xmove[i]])
                    subtract = 0;
            }
            if (subtract)
                g->gamer[g->board[y + ymove[i]][x + xmove[i]]].adjacent++;
        }
    }
}

uint32_t to_str(uint32_t x, unsigned char *buffer) {
    int size = 0;
    while (x > 0) {
        size++;
        if (buffer != NULL)
            buffer[size] = x % 10;
        x /= 10;
    }
    return size;
}

void empty_field(gamma_t *g, char *out, uint32_t *idx, uint32_t length) {
    if (g->players >= 10) {
        for (uint32_t k = 1; k <= length - 1; k++) {
            out[(*idx)++] = ' ';
        }
    }
    out[(*idx)++] = '.';
}

void non_empty_field(gamma_t *g, char *out, uint32_t *idx, uint32_t length, uint32_t y, uint32_t x) {
    unsigned char buffer[10];
    uint32_t numbers = to_str(g->board[y][x], buffer);
    if (g->players >= 10) {
        for (uint32_t k = 1; k <= length - numbers; k++) {
            out[(*idx)++] = ' ';
        }
    }
    for (uint32_t k = numbers; k >= 1; k--) {
        out[(*idx)++] = buffer[k] + '0';
    }
}