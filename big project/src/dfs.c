/**
 * @file
 * Implementacja modułu zawierającego implementację algorytmu DFS
 *
 * @author Jakub Panasiuk
 */
#include <stdint.h>
#include "dfs.h"

void dfs_adjacent(gamma_t *g, uint32_t x, uint32_t y, uint32_t prev_occ) {
    // tablica xmove sprawia, że mogę symulować ruchy w pętli, upraszcza kod
    int xmove[] = {1, -1, 0, 0};
    // tablica ymove podobnie, tablice służą symulowaniu ruchów w 4 strony od @p x, @p y
    int ymove[] = {0, 0, -1, 1};
    bool dfs_done = 0;
    for (uint32_t i = 0; i < 4; i++) {
        if (is_legal(g, x + xmove[i], y + ymove[i]) &&
            g->board[y + ymove[i]][x + xmove[i]] == prev_occ &&
            !g->tools.visited[y + ymove[i]][x + xmove[i]]) {
            if (dfs_done)
                g->gamer[prev_occ].fields++;
            dfs(g, x + xmove[i], y + ymove[i],
                position(g, x + xmove[i], y + ymove[i]), prev_occ);
            dfs_done = 1;
        }
    }
    if (!dfs_done) g->gamer[prev_occ].fields--;

    // ustawiam @p g->tools.visited pól na 0, żeby móc użyć tego w następnych wywołaniach
    dedfs(g, x, y);
}
void dfs (gamma_t *g, uint32_t xt, uint32_t yt, uint32_t parent, uint32_t player) {
    int idx = 0;

    // tablica xmove sprawia, że mogę symulować ruchy w pętli, upraszcza kod
    int xmove[] = {1, -1, 0, 0};
    // tablica ymove podobnie, tablice służą symulowaniu ruchów w 4 strony od @p x, @p y
    int ymove[] = {0, 0, -1, 1};

    g->tools.visited[yt][xt] = 1;
    g->tools.stack[idx] = position(g, xt, yt);
    uint32_t x, y;

    while(idx >= 0) {
        g->tools.parent[g->tools.stack[idx]] = parent;
        g->tools.depth[g->tools.stack[idx]] = 0;

        // z numeru pozycji tablicy zmieniam na x i y
        x = ((g->tools.stack[idx] - 1) % g->width) + 1;
        y = g->tools.stack[idx--] / g->width;
        if (x != g->width)
            y++;

        for (int i = 0; i < 4; i++) {
            if (is_legal(g, x + xmove[i], y + ymove[i]) &&
                !g->tools.visited[y + ymove[i]][x + xmove[i]] &&
                g->board[y + ymove[i]][x + xmove[i]] == player) {

                g->tools.visited[y + ymove[i]][x + xmove[i]] = 1,
                g->tools.stack[++idx] = position(g, x + xmove[i], y + ymove[i]);
            }
        }
    }
}

void dedfs (gamma_t *g, uint32_t xt, uint32_t yt) {
    int idx = 0;

    // tablica xmove sprawia, że mogę symulować ruchy w pętli, upraszcza kod
    int xmove[] = {1, -1, 0, 0};
    // tablica ymove podobnie, tablice służą symulowaniu ruchów w 4 strony od @p x, @p y
    int ymove[] = {0, 0, -1, 1};

    g->tools.visited[yt][xt] = 0;
    g->tools.stack[idx] = position(g, xt, yt);
    uint32_t x, y;

    while(idx >= 0) {
        // z numeru pozycji tablicy zmieniam na x i y
        x = ((g->tools.stack[idx] - 1) % g->width) + 1;
        y = g->tools.stack[idx--] / g->width;
        if (x != g->width)
            y++;

        for (int i = 0; i < 4; i++) {
            if (is_legal(g, x + xmove[i], y + ymove[i]) &&
                g->tools.visited[y + ymove[i]][x + xmove[i]]) {

                g->tools.visited[y + ymove[i]][x + xmove[i]] = 0,
                g->tools.stack[++idx] = position(g, x + xmove[i], y + ymove[i]);
            }
        }
    }
}