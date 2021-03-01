/**
 * @file
 * Implementacja algorytmu Find & Union
 *
 * @author Jakub Panasiuk
 */
#include <stdint.h>
#include "disjoint_union.h"

uint32_t Find (gamma_t *g, uint32_t field) {
    if (field == g->tools.parent[field])
        return field;

    uint32_t tmp = Find(g, g->tools.parent[field]);
    g->tools.parent[field] = tmp;

    return tmp;
}


void Union (gamma_t *g, uint32_t first_field, uint32_t second_field) {
    uint32_t first_parent = Find(g, first_field);
    uint32_t second_parent = Find(g, second_field);

    if (first_parent == second_parent)
        return;

    uint32_t first_depth = g->tools.depth[first_parent];
    uint32_t second_depth = g->tools.depth[second_parent];

    if (first_depth < second_depth) {
        g->tools.parent[first_parent] = second_parent;
    }
    else if (first_depth > second_depth) {
        g->tools.parent[second_parent] = first_parent;
    }
    else {
        g->tools.parent[second_parent] = first_parent;
        g->tools.depth[first_parent] = g->tools.depth[first_parent] + 1;
    }
}