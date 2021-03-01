#include <stddef.h>
#include <stdlib.h>
#include "batch.h"
#include "gamma.h"

int main() {
    uint32_t line = 1;
    gamma_t *g = NULL;
    uint32_t *nums = calloc(5, sizeof(uint32_t));

    while(read_in(&g, line, nums)) {
        line++;
    }

    if (nums != NULL)
        free(nums);
    gamma_delete(g);
    return 0;
}