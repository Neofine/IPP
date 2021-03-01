/**
 * @file
 * Implementacja modułu przetrzymującego funkcje pomocnicze
 * do modułu interactive.h
 *
 * @author Jakub Panasiuk
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include "assist_interactive.h"
#include "supp_functions.h"
#include "ansi_and_termios.h"

int move_horizontal(int c) {
    if (c == 'C') return 1;
    else if (c == 'D') return -1;
    return 0;
}

int move_vertical(int c) {
    if (c == 'A') return 1;
    else if (c == 'B') return -1;
    return 0;
}

void print_error(gamma_t *g, int error) {
    move_cursor(1, g->height + 7);
    erase_line();
    print_red();
    if (error == already_owned)
        printf("Field is already owned by YOU!");
    else if (error == other_players)
        printf("Field is already owned by another player!");
    else if (error == too_much_areas)
        printf("No can do! You would have too much areas.");
    else if (error == golden_himself)
        printf("You can't do golden move on your field!");
    else if (error == free_field)
        printf("You can't do golden move on a free field, go for regular one!");
    else if (error == violate_rules)
        printf("Your move would violate rules of the game, don't try to cheat!");
    else if (error == out_of_bounds)
        printf("You can't go that way, refine your strategy!");
    clear_color();
}

void move_error(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (g->board[y][x] == player) {
        print_error(g, already_owned);
    }
    else if (g->board[y][x] != 0) {
        print_error(g, other_players);
    }
    else{
        print_error(g, too_much_areas);
    }
}

void golden_error(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (g->board[y][x] == player) {
        print_error(g, golden_himself);
    }
    else if (g->board[y][x] == 0) {
        print_error(g, free_field);
    }
    else {
        print_error(g, violate_rules);
    }
}

bool skip_turn(int *chars) {
    return (chars[0] == 'C' || chars[0] == 'c') && (chars[2] != 27 || chars[1] != 91);
}



void highlight_owned(gamma_t *g, uint32_t landlord, bool like_rest) {
    for (uint32_t i = 1; i <= g->height; i++) {
        for (uint32_t j = 1; j <= g->width; j++) {
            if (g->board[i][j] == landlord) {
                if (like_rest) {
                    highlight_field(g, j, i, "", "");
                }
                else {
                    highlight_field(g, j, i, cyan_txt_seq(), "");
                }
            }

        }
    }
}

void highlight_field(gamma_t *g, uint32_t x, uint32_t y, char * color, char * background) {
    uint32_t length = snprintf(NULL, 0, "%" PRIu32, g->players);
    update_cords(x, y);

    uint32_t xtp, ytp;
    ytp = g->height - y + 2;
    if (length == 1) {
        xtp = x;
    }
    else {
        xtp = (x - 1) * (length + 1) + 1;
    }
    save_cursor();
    move_cursor(xtp, ytp);

    unsigned char buffer[10];
    uint32_t numbers = to_str(g->board[y][x], buffer);
    if (g->players >= 10) {
        if (g->board[y][x] == 0) {
            for (uint32_t k = 1; k <= length - 1; k++) {
                printf(" ");
            }
        }
        else {
            for (uint32_t k = 1; k <= length - numbers; k++) {
                printf(" ");
            }
        }
    }

    printf("%s%s", color, background);

    if (numbers == 0) {
        printf(".");
    }
    else {
        for (uint32_t k = numbers; k >= 1; k--) {
            printf("%d", buffer[k]);
        }
    }

    clear_color();
    restore_cursor();
}

void final_show(gamma_t *g) {
    clear();
    char * p = gamma_board(g);
    printf("%s", p);

    uint32_t max_fields = 0;
    for (uint32_t i = 1; i <= g->players; i++) {
        if (g->gamer[i].taken > max_fields)
            max_fields = g->gamer[i].taken;
    }
    free(p);

    int winners = 0, last_winner = 0;
    for (uint32_t i = 1; i <= g->players; i++) {
        if (g->gamer[i].taken == max_fields) {
            winners++;
            last_winner = i;
            print_green();
            printf("PLAYER nr %d ended up with %d field(s) <- WINNER!\n", i, g->gamer[i].taken);
        }
        else {
            print_red();
            printf("PLAYER nr %d ended up with %d field(s)\n", i, g->gamer[i].taken);
        }
    }

    print_gold();
    if (winners == 1) {
        printf("PLAYER nr %d won the entire game, congratz!\n", last_winner);
    }
    else {
        printf("%d players won ex aequo first place, GG!\n", winners);
    }
    print_cyan();
    printf("Thanks for playing, see you next time!\n");
    clear_color();
}

void print_player(gamma_t *g, uint32_t player, uint64_t free_fields, bool golden_possible) {
    uint32_t print_location = g->height + 2;
    move_cursor(1, print_location);

    erase_line();
    print_green();
    printf("PLAYER nr: %u\n", player);

    move_cursor(1, print_location + 1);
    erase_line();
    printf("OWNED FIELDS: %u\n", g->gamer[player].taken);

    move_cursor(1, print_location + 2);
    erase_line();
    printf("POSSIBLE TO ACQUIRE: %" PRIu64 "\n", free_fields);

    move_cursor(1, print_location + 3);
    erase_line();
    printf("OCCUPIED AREAS: %u / %u\n", g->gamer[player].fields, g->areas);

    move_cursor(1, print_location + 4);
    erase_line();
    if (golden_possible) {
        print_gold();
        printf("[Golden Available]");
    }

    clear_color();
}