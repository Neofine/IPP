/**
 * @file
 * Implementacja modułu zawierająca wszystkie komendy które mogą zostać wywołane w main'ie
 *
 * @author Jakub Panasiuk
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "gamma.h"
#include "supp_functions.h"
#include "dfs.h"
#include "structs.h"

void gamma_delete(gamma_t *g) {
    if (g == NULL)
        return;

    if (g->tools.parent != NULL) free(g->tools.parent);
    if (g->gamer != NULL) free(g->gamer);
    if (g->tools.depth != NULL) free(g->tools.depth);
    if (g->tools.stack != NULL) free(g->tools.stack);
    for (uint32_t i = 1; i <= g->height; i++) {
        bool freeing = false;
        if (g->board != NULL && g->board[i] != NULL) {
            free(g->board[i]);
            freeing = true;
        }
        if (g->tools.visited != NULL && g->tools.visited[i] != NULL) {
            free(g->tools.visited[i]);
            freeing = true;
        }
        if (!freeing)
            break;
    }
    if (g->board != NULL) free(g->board);
    if (g->tools.visited != NULL) free(g->tools.visited);
    free(g);
}

gamma_t* gamma_new(uint32_t width, uint32_t height, uint32_t players, uint32_t areas) {
    if (width == 0 || height == 0 || players == 0 || areas == 0 ||
        width + 1 == 0 || height + 1 == 0 || players + 1 == 0)
        return NULL;

    gamma_t try = {
            .width = width,
            .height = height,
            .players = players,
            .areas = areas,
            .taken_fields = 0,
            .tools.visited = NULL,
            .tools.stack = NULL,
            .tools.parent = NULL,
            .tools.depth = NULL,
            .gamer = NULL,
            .board = NULL,
    };
    gamma_t *new_gamma = (gamma_t *)malloc(sizeof(struct gamma));
    if (new_gamma == NULL) {
        return NULL;
    }
    *new_gamma = try;

    new_gamma->tools.parent = (uint32_t *)calloc((height+1) * (width+1),
                                                 sizeof(uint32_t));
    if (new_gamma->tools.parent == NULL)
        {gamma_delete(new_gamma); return NULL;}

    new_gamma->gamer = (player_t *)malloc((players+1) * sizeof(struct player));
    if (new_gamma->gamer == NULL)
        {gamma_delete(new_gamma); return NULL;}

    new_gamma->tools.depth = (uint32_t *)calloc((height+1) * (width+1),
                                                sizeof(uint32_t));
    if (new_gamma->tools.depth == NULL)
        {gamma_delete(new_gamma); return NULL;}

    new_gamma->tools.stack = (uint32_t *)calloc((uint64_t)(height+1) * (uint64_t)(width+1),
                                                sizeof(uint32_t));
    if (new_gamma->tools.stack == NULL)
        {gamma_delete(new_gamma); return NULL;}

    new_gamma->board = (uint32_t **)malloc((height+1) * sizeof(uint32_t *));
    if (new_gamma->board == NULL)
        {gamma_delete(new_gamma); return NULL;}

    new_gamma->tools.visited = (bool **)malloc((height+1) * sizeof(bool *));
    if (new_gamma->tools.visited == NULL)
        {gamma_delete(new_gamma); return NULL;}

    for (uint32_t i = 1; i <= height; i++) {
        new_gamma->board[i] = (uint32_t *)calloc((width+1), sizeof(uint32_t));
        if (new_gamma->board[i] == NULL)
            {gamma_delete(new_gamma); return NULL;}

        new_gamma->tools.visited[i] = (bool *)calloc((width+1), sizeof(bool));
        if (new_gamma->tools.visited[i] == NULL)
            {gamma_delete(new_gamma); return NULL;}

        for (uint32_t x = 1; x <= width; x++)
            new_gamma->tools.parent[position(new_gamma, x, i)] =
                    position(new_gamma, x, i);
    }

    for (uint32_t i = 1; i <= players; i++) {
        new_gamma->gamer[i].fields = new_gamma->gamer[i].adjacent = 0;
        new_gamma->gamer[i].taken = 0;
        new_gamma->gamer[i].golden_done = false;
    }

    return new_gamma;
}

bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    x++, y++;        // zwiększam o 1 @p x i @p y dla uproszczenia kodu
    if (g == NULL || player == 0 || player > g->players || !is_legal(g, x, y) ||
        g->board[y][x] != 0 || g->board[y][x] == player ||
        g->tools.parent[position(g, x, y)] != position(g, x, y)) {
        return false;
    }

    bool is_connected = union_with_nearby(g, x, y, player);
    if (is_connected)
        g->gamer[player].adjacent--;

    if (is_connected || g->gamer[player].fields < g->areas) {
        manage_adjacent(g, x, y, player);

        g->board[y][x] = player;
        if (!is_connected) g->gamer[player].fields++;
        g->gamer[player].taken++;
        g->taken_fields++;
        return true;
    }
    return false;
}

bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    x++, y++; // zwiększam o 1 @p x i @p y dla uproszczenia kodu

    if (g == NULL || player == 0 || player > g->players || !is_legal(g, x, y) || g->board[y][x] == 0 ||
        g->board[y][x] == player || g->gamer[player].golden_done)
        return false;

    uint32_t prev_occ = g->board[y][x];

    // ustawiam parametry jakby tego pola nigdy nie było w celu dalszego uproszczenia kodu
    g->board[y][x] = 0;
    g->gamer[prev_occ].taken--;
    g->taken_fields--;
    g->tools.parent[position(g, x, y)] = position(g, x, y);
    g->tools.depth[position(g, x, y)] = 0;

    dfs_adjacent(g, x, y, prev_occ);

    sub_adjacent_from_prev(g, x, y, prev_occ);

    // jeżeli mogę wstawić @p player na pole zgodnie z zasadami
    if (g->gamer[prev_occ].fields <= g->areas && gamma_move(g, player, x-1, y-1)){
        g->gamer[player].golden_done = 1;
        fix_adjacent(g, x, y);
        return 1;
    }
    // jeżeli nie mogę wstawić @p player, to cofam ruch
    else {
        gamma_move(g, prev_occ, x-1, y-1);
        fix_adjacent(g, x, y);
        return 0;
    }
}

uint64_t gamma_busy_fields(gamma_t *g, uint32_t player) {
    if (g == NULL || player > g->players || player == 0) return 0;
    return g->gamer[player].taken;
}

uint64_t gamma_free_fields(gamma_t *g, uint32_t player) {
    if (g == NULL || player > g->players || player == 0) return 0;

    if (g->gamer[player].fields < g->areas)
        return (uint64_t)((g->width) * (g->height) - g->taken_fields);
    return g->gamer[player].adjacent;
}

bool gamma_golden_possible(gamma_t *g, uint32_t player) {
    if (g == NULL || player > g->players || player == 0 || g->gamer[player].golden_done)
        return 0;

    for (uint32_t i = 1; i <= g->height; i++) {
        for (uint32_t j = 1; j <= g->width; j++) {
            int prev_owner = g->board[i][j];
            if (gamma_golden_move(g, player, j - 1, i - 1)) {
                bool can_prev_golden = g->gamer[prev_owner].golden_done;
                g->gamer[prev_owner].golden_done = false;

                gamma_golden_move(g, prev_owner, j - 1, i - 1);

                g->gamer[player].golden_done = false;
                g->gamer[prev_owner].golden_done = can_prev_golden;
                return true;
            }
        }
    }

    return false;
}

char* gamma_board(gamma_t *g) {
    if (g == NULL)
        return NULL;

    uint32_t length = snprintf(NULL, 0, "%" PRIu32, g->players);

    char *out;
    out = (char *) calloc((g->height + 1) * (g->width + 1) * (length + 2), (sizeof(char)));
    if (out == NULL)
        exit(1);

    uint32_t idx = 0;
    for (uint32_t i = g->height; i >= 1 ; i--) {
        for (uint32_t j = 1; j <= g->width; j++) {
            if (g->board[i][j] == 0) {
                empty_field(g, out, &idx, length);
            }
            else {
                non_empty_field(g, out, &idx, length, i, j);
            }
            if (g->players >= 10)
                out[idx++] = ' ';
        }
        out[idx++] = '\n';
    }
    return out;
}