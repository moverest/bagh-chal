#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "game.h"
#include "ai_rand.h"
#include "tools.h"

static bool board_equals(board_t *b1, board_t *b2) {
    position_t pos;

    for (pos.r = 0; pos.r < 5; pos.r++) {
        for (pos.c = 0; pos.c < 5; pos.c++) {
            if (board_get_cell(b1, pos) != board_get_cell(b2, pos)) {
                return false;
            }
        }
    }

    return true;
}


static bool game_equals(game_t *g1, game_t *g2) {
    return g1->turn == g2->turn &&
           g1->num_goats_to_put == g2->num_goats_to_put &&
           g1->num_eaten_goats == g2->num_eaten_goats &&
           board_equals(&g1->board, &g2->board);
}


static bool possible_positions_equals(possible_positions_t *p1,
                                      possible_positions_t *p2) {
    position_t pos;

    for (pos.r = 0; pos.r < 5; pos.r++) {
        for (pos.c = 0; pos.c < 5; pos.c++) {
            if (is_position_possible(p1, pos) != is_position_possible(p2, pos)) {
                return false;
            }
        }
    }

    return true;
}


static void game_print_board(board_t *board) {
    position_t pos;

    for (pos.r = 0; pos.r < 5; pos.r++) {
        for (pos.c = 0; pos.c < 5; pos.c++) {
            switch (board_get_cell(board, pos)) {
            case TIGER_CELL:
                printf("T ");

                break;

            case GOAT_CELL:
                printf("G ");
                break;

            default:
                printf(". ");
            }
        }

        puts("");
    }
}


static void game_print(game_t *game) {
    printf("Turn: %s\n", game->turn == GOAT_TURN ? "GOAT" : "TIGER");
    printf("Goat to put: %d\n", game->num_goats_to_put);
    printf("Eaten goats: %d\n", game->num_eaten_goats);
    game_print_board(&game->board);
}


static void possible_positions_print(possible_positions_t *p) {
    position_t pos;

    for (pos.r = 0; pos.r < 5; pos.r++) {
        for (pos.c = 0; pos.c < 5; pos.c++) {
            bool possible = is_position_possible(p, pos);
            if (possible) {
                printf("%d ", possible);
            } else {
                printf(". ");
            }
        }

        puts("");
    }
}


#define CHECK_GAME_STATE(game_got, game_expect, line) \
    if (!game_equals(game_got, game_expect)) {        \
        printf("## Line: %d\n", line);                \
        puts("## Expected ##");                       \
        game_print(game_expect);                      \
        puts("\n## Got ##");                          \
        game_print(game_got);                         \
        test_fail(t);                                 \
    }                                                 \

#define CHECK_POSSIBLE_POS(pos_got, pos_expected, game_got, line) \
    if (!possible_positions_equals(pos_got, pos_expected)) {      \
        printf("## Line: %d\n", line);                            \
        puts("## Expected ##");                                   \
        possible_positions_print(pos_expected);                   \
        puts("\n## Got ##");                                      \
        possible_positions_print(pos_got);                        \
        game_print(game_got);                                     \
        test_fail(t);                                             \
    }                                                             \

#define CHECK_IMPOSSIBLE_MVT(mvt, game_got, line)                       \
    if (game_do_mvt(game_got, mvt)) {                                   \
        printf("## Line: %d\n", line);                                  \
        game_print(game_got);                                           \
        printf("Movement %c%d -> %c%d should not be valid.\n",          \
               'a' + mvt.from.c, mvt.from.r, 'a' + mvt.to.c, mvt.to.r); \
        test_fail(t);                                                   \
    }                                                                   \

#define CHECK_IMPOSSIBLE_MVTS(impossible_mvts, game_got, line)  \
    for (int impossible_mvt_i = 0;                              \
         impossible_mvt_i < sizeof(impossible_mvts) /           \
         sizeof(impossible_mvts[0]);                            \
         impossible_mvt_i++) {                                  \
        CHECK_IMPOSSIBLE_MVT(impossible_mvts[impossible_mvt_i], \
                             game_got, __LINE__);               \
    }                                                           \

static void test_game_begin(test_t *t) {
    game_t *game = game_new();

    game_t expect_game_1 = {
        .board            = { {
                                  TIGER_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, TIGER_CELL,
                                  EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                                  EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                                  EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                                  TIGER_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, TIGER_CELL
                              } },
        .turn             = GOAT_TURN,
        .num_goats_to_put = 20,
        .num_eaten_goats  = 0
    };

    CHECK_GAME_STATE(game, &expect_game_1, __LINE__);

    possible_positions_t expect_possible_from_1 = { {
                                                        false, true, true, true, false,
                                                        true, true, true, true, true,
                                                        true, true, true, true, true,
                                                        true, true, true, true, true,
                                                        false, true, true, true, false,
                                                    } };


    possible_positions_t got_possible;
    game_get_possible_from_positions(game, &got_possible);

    CHECK_POSSIBLE_POS(&got_possible, &expect_possible_from_1,
                       game, __LINE__);

    mvt_t mvt_1 = { { 1, 0 }, { POSITION_NOT_SET, POSITION_NOT_SET } };
    game_do_mvt(game, mvt_1);

    game_t expect_game_2 = {
        .board            = { {
                                  TIGER_CELL, GOAT_CELL, EMPTY_CELL, EMPTY_CELL, TIGER_CELL,
                                  EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                                  EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                                  EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                                  TIGER_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, TIGER_CELL
                              } },
        .turn             = TIGER_TURN,
        .num_goats_to_put = 19,
        .num_eaten_goats  = 0
    };

    CHECK_GAME_STATE(game, &expect_game_2, __LINE__);

    possible_positions_t expect_possible_from_2 = { {
                                                        true, false, false, false, true,
                                                        false, false, false, false, false,
                                                        false, false, false, false, false,
                                                        false, false, false, false, false,
                                                        true, false, false, false, true,
                                                    } };

    game_get_possible_from_positions(game, &got_possible);
    CHECK_POSSIBLE_POS(&got_possible, &expect_possible_from_2,
                       game, __LINE__);

    possible_positions_t expect_possible_to_2_1 = { {
                                                        false, false, true, false, false,
                                                        true, true, false, false, false,
                                                        false, false, false, false, false,
                                                        false, false, false, false, false,
                                                        false, false, false, false, false,
                                                    } };

    game_get_possible_to_positions(game, (position_t){0, 0 }, &got_possible);
    CHECK_POSSIBLE_POS(&got_possible, &expect_possible_to_2_1,
                       game, __LINE__);


    possible_positions_t expect_possible_to_2_2 = { {
                                                        false, false, false, false, false,
                                                        false, false, false, false, false,
                                                        false, false, false, false, false,
                                                        false, false, false, true, true,
                                                        false, false, false, true, false,
                                                    } };

    game_get_possible_to_positions(game, (position_t){4, 4 }, &got_possible);
    CHECK_POSSIBLE_POS(&got_possible, &expect_possible_to_2_2,
                       game, __LINE__);

    mvt_t mvt_2_impossibles[] = {
        { { 0, 0 }, {  0, 0 } },
        { { 0, 0 }, {  2, 2 } },
        { { 0, 0 }, {  0, 2 } },
        { { 1, 0 }, {  2, 0 } },
        { { 0, 0 }, { -1, 0 } },
        { { 0, 0 }, {  3, 0 } }
    };
    CHECK_IMPOSSIBLE_MVTS(mvt_2_impossibles, game, __LINE__)


    mvt_t mvt_2 = { { 0, 0 }, { 2, 0 } };
    game_do_mvt(game, mvt_2);


    game_t expect_game_3 = {
        .board            = { {
                                  EMPTY_CELL, EMPTY_CELL, TIGER_CELL, EMPTY_CELL, TIGER_CELL,
                                  EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                                  EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                                  EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                                  TIGER_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, TIGER_CELL
                              } },
        .turn             = GOAT_TURN,
        .num_goats_to_put = 19,
        .num_eaten_goats  = 1
    };
    CHECK_GAME_STATE(game, &expect_game_3, __LINE__);

    possible_positions_t expect_possible_from_3 = { {
                                                        true, true, false, true, false,
                                                        true, true, true, true, true,
                                                        true, true, true, true, true,
                                                        true, true, true, true, true,
                                                        false, true, true, true, false,
                                                    } };

    game_get_possible_from_positions(game, &got_possible);
    CHECK_POSSIBLE_POS(&got_possible, &expect_possible_from_3,
                       game, __LINE__);

    mvt_t mvt_3_impossibles[] = {
        { { 2, 0 }, { POSITION_NOT_SET, POSITION_NOT_SET } },
        { { 4, 0 }, { POSITION_NOT_SET, POSITION_NOT_SET } },
        { { 0, 4 }, { POSITION_NOT_SET, POSITION_NOT_SET } },
        { { 4, 4 }, { POSITION_NOT_SET, POSITION_NOT_SET } }
    };

    CHECK_IMPOSSIBLE_MVTS(mvt_3_impossibles, game, __LINE__);


    mvt_t mvt_3 = { { 2, 3 }, { POSITION_NOT_SET, POSITION_NOT_SET } };
    game_do_mvt(game, mvt_3);

    game_t expect_game_4 = {
        .board            = { {
                                  EMPTY_CELL, EMPTY_CELL, TIGER_CELL, EMPTY_CELL, TIGER_CELL,
                                  EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                                  EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                                  EMPTY_CELL, EMPTY_CELL, GOAT_CELL, EMPTY_CELL, EMPTY_CELL,
                                  TIGER_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, TIGER_CELL
                              } },
        .turn             = TIGER_TURN,
        .num_goats_to_put = 18,
        .num_eaten_goats  = 1
    };
    CHECK_GAME_STATE(game, &expect_game_4, __LINE__);

    mvt_t mvt_4 = { { 4, 0 }, { 4, 1 } };
    game_do_mvt(game, mvt_4);

    game_t expect_game_5 = {
        .board            = { {
                                  EMPTY_CELL, EMPTY_CELL, TIGER_CELL, EMPTY_CELL, EMPTY_CELL,
                                  EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, TIGER_CELL,
                                  EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL,
                                  EMPTY_CELL, EMPTY_CELL, GOAT_CELL, EMPTY_CELL, EMPTY_CELL,
                                  TIGER_CELL, EMPTY_CELL, EMPTY_CELL, EMPTY_CELL, TIGER_CELL
                              } },
        .turn             = GOAT_TURN,
        .num_goats_to_put = 18,
        .num_eaten_goats  = 1
    };
    CHECK_GAME_STATE(game, &expect_game_5, __LINE__);

    game_free(game);
}


static void print_mvt(mvt_t mvt) {
    printf("{{%d, %d}, {%d, %d}}", mvt.from.c, mvt.from.r, mvt.to.c, mvt.to.r);
}


static void test_ai_rand(test_t *t) {
    game_t    *game = game_new();
    ai_rand_t *ai   = ai_rand_callbacks.new();
    mvt_t     mvt;

    mvt = ai_rand_callbacks.get_goat_mvt(ai, game);
    print_mvt(mvt);
    puts("");

    game_do_mvt(game, mvt);
    mvt = ai_rand_callbacks.get_tiger_mvt(ai, game);
    print_mvt(mvt);
    puts("");


    ai_rand_callbacks.free(ai);
    free(game);
}


static void test_undo(test_t *t) {
    game_t *game = game_new();
    mvt_t  mvt;

    int num_mvt = 0;

    while (!game_is_done(game)) {
        mvt = ai_rand_get_goat_mvt(NULL, game);
        game_do_mvt(game, mvt);
        num_mvt++;
    }

    while (num_mvt > 0) {
        int err = game_undo(game);
        if (err) {
            printf("%s:%d: Error while undoing movements %d\n",
                   __FILE__, __LINE__, num_mvt);
            test_fail(t);
        }

        num_mvt--;
    }

    if (!game_undo(game)) {
        printf("%s:%d: There should not be any more movements to undo\n",
               __FILE__, __LINE__);
        test_fail(t);
    }

    free(game);
}


int main(int argc, char **argv) {
    test_function_t tests[] = {
        TEST_FUNCTION(test_game_begin),
        TEST_FUNCTION(test_ai_rand),
        TEST_FUNCTION(test_undo)
    };

    return test_run(tests, ARRAY_LEN(tests));
}
