#include "score_board.h"
#include "player.h"
#include "game_state.h"
#include "tilemap.h"

#pragma codeseg BANK_8

#define SCORE_COORD_X 2
#define SCORE_COORD_Y 29
#define SCORE_NAME_LENGTH 10

#define SCORE_NAME_POSITION 2
#define SCORE_SERVICE_POSITION (SCORE_NAME_POSITION + SCORE_NAME_LENGTH + 2)
#define SCORE_SETS_POSITION (SCORE_SERVICE_POSITION + 2)
#define SCORE_GAMES_POSITION (SCORE_SETS_POSITION + 2)
#define SCORE_POINTS_POSITION (SCORE_GAMES_POSITION + 2)
#define SCORE_TAIL_POSITION (SCORE_POINTS_POSITION + 2)

#define SCORE_LENGTH (SCORE_TAIL_POSITION + 2)

char score_board_1[SCORE_LENGTH];
char score_board_2[SCORE_LENGTH];

static const char score_points_digits[][3] = {
    "%&", // This is a cero centered in two tiles
    "15",
    "30",
    "40",
    "AD",
};

/**
 * Initializes the score board with the name of the players and the trail and ending characters with the shape
 */
void init_score_board(void)
{
    for (uint8_t i = 0; i < SCORE_LENGTH; i++)
    {
        score_board_1[i] = ' ';
        score_board_2[i] = ' ';
    }

    score_board_1[0] = 0x01;
    score_board_1[SCORE_LENGTH - 1] = 0x04;
    score_board_2[0] = 0x02;
    score_board_2[SCORE_LENGTH - 1] = 0x0;
    score_board_2[SCORE_LENGTH - 2] = 0x03;

    for (uint8_t i = 0; (player_top.name)[i] != 0; i++)
        score_board_1[i + SCORE_NAME_POSITION] = (player_top.name)[i];

    for (uint8_t i = 0; (player_bottom.name)[i] != 0; i++)
        score_board_2[i + SCORE_NAME_POSITION] = (player_bottom.name)[i];
}

char db_score = 0xFF;
char db_digit = 0xFF;

/**
 * Updates the score and shows the score board
 */
void show_score_board(void)
{
    score_board_1[SCORE_SERVICE_POSITION] = game_state.service_owner == PLAYER_TOP ? CHAR_SERVICE_BALL : ' ';
    score_board_2[SCORE_SERVICE_POSITION] = game_state.service_owner == PLAYER_BOTTOM ? CHAR_SERVICE_BALL : ' ';
    score_board_1[SCORE_SETS_POSITION] = '0' + score.sets[0];
    score_board_2[SCORE_SETS_POSITION] = '0' + score.sets[1];
    score_board_1[SCORE_GAMES_POSITION] = '0' + score.games[0];
    score_board_2[SCORE_GAMES_POSITION] = '0' + score.games[1];

    tilemap[SCORE_COORD_Y][SCORE_COORD_X + SCORE_POINTS_POSITION].palette = TILEMAP_GAMES_PALETTE;
    tilemap[SCORE_COORD_Y][SCORE_COORD_X + SCORE_POINTS_POSITION + 1].palette = TILEMAP_GAMES_PALETTE;
    tilemap[SCORE_COORD_Y + 1][SCORE_COORD_X + SCORE_POINTS_POSITION].palette = TILEMAP_GAMES_PALETTE;
    tilemap[SCORE_COORD_Y + 1][SCORE_COORD_X + SCORE_POINTS_POSITION + 1].palette = TILEMAP_GAMES_PALETTE;

    // TODO: consider tie break
    const char *score1 = score_points_digits[score.points[0]];
    db_score = score.points[0];
    db_digit = score1[0];
    score_board_1[SCORE_POINTS_POSITION] = score1[0];
    score_board_1[SCORE_POINTS_POSITION + 1] = score1[1];
    const char *score2 = score_points_digits[score.points[1]];
    score_board_2[SCORE_POINTS_POSITION] = score2[0];
    score_board_2[SCORE_POINTS_POSITION + 1] = score2[1];

    for (uint8_t i = 0; i < SCORE_LENGTH; i++)
    {
        tilemap[SCORE_COORD_Y][i + SCORE_COORD_X].character = score_board_1[i];
        tilemap[SCORE_COORD_Y + 1][i + SCORE_COORD_X].character = score_board_2[i];
    }
}

/**
 * Hides the score board
 */
void hide_score_board(void)
{
    for (uint8_t i = 0; i < SCORE_LENGTH; i++)
    {
        tilemap[SCORE_COORD_Y][i + SCORE_COORD_X].character = 0;
        tilemap[SCORE_COORD_Y + 1][i + SCORE_COORD_X].character = 0;
    }
}
