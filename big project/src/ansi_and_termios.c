/**
 * @file
 * Impelemtacja modułu przetrzymującego funkcje używające
 * ansi escape codes lub termiosa
 *
 * @author Jakub Panasiuk
 */
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include "ansi_and_termios.h"

void hide_cursor() {
    printf("\e[?25l");
}

void show_cursor() {
    printf("\e[?25h");
}

void erase_line() {
    printf("\x1b[K");
}

void move_cursor(uint32_t x, uint32_t y) {
    printf("\x1b[%d;%dH", y, x);
}

void save_cursor() {
    printf("\x1b[s");
}

void restore_cursor() {
    printf("\x1b[u");
}

void clear() {
    printf("\e[1;1H\e[2J");
}

void clear_color() {
    printf("\x1b[0m");
}

void print_gold() {
    printf("\x1b[93;1m");
}

void print_red() {
    printf("\x1b[31;1m");
}

void print_cyan() {
    printf("\x1b[96;1m");
}

void print_green() {
    printf("\x1b[32;1m");
}

char *cyan_txt_seq() {
    return "\x1b[96;1m";
}

char *black_txt_seq() {
    return "\x1b[30m";
}

char *white_bcg_seq() {
    return "\x1b[47m";
}


void turn_echo_off() {
    if (system("stty -echo"))
        exit(1);
}

void turn_echo_on() {
    if (system("stty echo"))
        exit(1);
}

void beginArrowRead(struct termios newattr) {
    newattr.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newattr))
        exit(1);
}

void endArrowRead(struct termios oldattr) {
    if (tcsetattr(STDIN_FILENO, TCSANOW, &oldattr))
        exit(1);
}

void print_coords(uint32_t x, uint32_t y) {
    print_cyan();
    printf("Coordinates: (%u, %u)\n", x - 1, y - 1);
    clear_color();
}

void update_cords(uint32_t x, uint32_t y) {
    save_cursor();
    move_cursor(1, 1);
    erase_line();
    print_coords(x, y);
    restore_cursor();
}