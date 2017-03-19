#include <stdio.h>
#include <stdlib.h>

#include "game.h"

void display_pos(possible_positions_t *p);
void display_board(board_t *b);

void test_create_delete();
void test_in_board();
void test_diag();
void test_adj();
void test_top_left();
void test_bot_right();
void test_movable_pawns();
void test_possible_pos();

int main(int argc, char **argv) {
    puts("hello, world");
    //test_create_delete(); OK
    //test_in_board(); OK
    //test_diag(); OK
    //test_adj(); OK
    //test_top_left(); OK
    //test_bot_right(); OK
    test_possible_pos();

    //test_movable_pawns();
    printf("\n");
    return 0;
}


void test_create_delete() {
    game_t *g = game_new();

    game_free(g);
}


void display_board(board_t *b) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d ", get_cell(b, (position_t){i, j }));
        }
        puts("");
    }
}


void display_pos(possible_positions_t *p) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d ", is_position_possible(p, (position_t){i, j }));
        }
        puts("");
    }
}


void test_in_board() {
    position_t p;

    for (p.r = -1; p.r < 6; p.r++) {
        for (p.c = -1; p.c < 6; p.c++) {
            printf("\np est pas dans la grille (r=%d , c=%d) :%d", p.r, p.c, in_board(p));
        }
    }
}


void test_diag() {
    position_t p;

    for (p.r = 0; p.r < 5; p.r++) {
        for (p.c = 0; p.c < 5; p.c++) {
            printf("\np a une diagonale (r=%d , c=%d) :%d", p.r, p.c, has_diagonal(p));
        }
    }
}


void test_adj() {
    position_t mp;

    mp.r = 3;
    mp.c = 2;
    position_t p;
    printf("\nmp (r=%d , c=%d)", mp.r, mp.c);
    for (p.r = mp.r - 1; p.r <= mp.r + 1; p.r++) {
        for (p.c = mp.c - 1; p.c <= mp.c + 1; p.c++) {
            printf("\np et mp sont adjacentes (r=%d , c=%d) :%d", p.r, p.c, on_same_rowcol(p, mp));
        }
    }
}


void test_top_left() {
    position_t p;

    for (p.r = 0; p.r < 5; p.r++) {
        for (p.c = 0; p.c < 5; p.c++) {
            position_t d = top_left_cell(p);
            printf("\ncellule en haut a gauche de p(r=%d , c=%d) : (%d , %d)", p.r, p.c, d.r, d.c);
        }
    }
}


void test_bot_right() {
    position_t p;

    for (p.r = 0; p.r < 5; p.r++) {
        for (p.c = 0; p.c < 5; p.c++) {
            position_t d = bot_right_cell(p);
            printf("\ncellule en bas a droite de p(r=%d , c=%d) : (%d , %d)", p.r, p.c, d.r, d.c);
        }
    }
}


void test_possible_pos() {
    game_t               *g = game_new();
    possible_positions_t p;
    position_t           rd;

    puts("1 goat");
    g->turn = TIGER_TURN;

    //1st turn goat in cell(0,1)
    set_cell(&(g->board), (position_t){0, 1 }, GOAT_CELL);

    rd.r = 0;
    rd.c = 0;
    printf("top left tiger can move : %d\n", game_possible_movement(g, rd, &p, true));
    display_pos(&p);


    rd.c = 4;
    printf("top right tiger can move : %d\n", game_possible_movement(g, rd, &p, false));

    rd.r = 4;
    printf("bot right tiger can move : %d\n", game_possible_movement(g, rd, &p, false));

    rd.c = 0;
    printf("bot left tiger can move : %d\n", game_possible_movement(g, rd, &p, false));

    puts("possible movements :");
    display_pos(&p);



    puts("tiger cornered, board looks like :");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i + j) {
                set_cell(&(g->board), (position_t){i, j }, GOAT_CELL);
            }
        }
    }
    display_board(&(g->board));

    rd.r = 0;
    rd.c = 0;
    printf("top left tiger can move : %d\n", game_possible_movement(g, rd, &p, true));
    display_pos(&p);


    rd.c = 4;
    printf("top right tiger can move : %d\n", game_possible_movement(g, rd, &p, false));

    rd.r = 4;
    printf("bot right tiger can move : %d\n", game_possible_movement(g, rd, &p, false));

    rd.c = 0;
    printf("bot left tiger can move : %d\n", game_possible_movement(g, rd, &p, false));

    puts("possible movements :");
    display_pos(&p);

    puts("top left tiger can escape by eating a goat");
    set_cell(&(g->board), (position_t){0, 2 }, EMPTY_CELL);
    display_board((board_t *)g);
    rd.r = 0;
    rd.c = 0;
    printf("top left tiger can move : %d\n", game_possible_movement(g, rd, &p, false));
    display_pos(&p);
}


void test_movable_pawns() {
    game_t               *g = game_new();
    possible_positions_t p;

    printf("\nnombre de pions que l'on peut bouger : %d", game_find_movable_pawns(g, &p));
    g->turn = TIGER_TURN;
    printf("\nnombre de pions que l'on peut bouger : %d", game_find_movable_pawns(g, &p));
    game_free(g);
}
