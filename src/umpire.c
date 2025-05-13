#include <string.h>
#include "umpire.h"
#include "umpire_queue.h"
#include "umpire_balloon.h"
#include "ball.h"
#include "court_dimensions.h"
#include "game_state.h"
#include "tilemap.h"
#include "sound.h"

#pragma codeseg BANK_8

// Height of the area of the screen (centered at the net), where the umpirse looks at the center
#define UMPIRE_CENTRAL_AREA_HEIGHT 80

#define NO_SOUND_SAMPLE 0xFF

// Timeout for the balloon to be removed from the screen with no voice calls
#define NO_VOICE_CALL_BALLOON_TIMEOUT 50

// Extra time balloons are left after voice is over
#define EXTENDED_BALLOON_TIMEOUT 10

// -----------------------------------------------
// Umpire calls text constants
// -----------------------------------------------

static const char *game_score_texts[5] = {
    "0",
    "15",
    "30",
    "40",
    "AD"};

static const char *umpire_call_texts[10] = {
    [VOICE_OUT] = "OUT!",
    [VOICE_FAULT] = "FAULT!",
    [VOICE_DOUBLE_FAULT] = "DOUBLE FAULT!",
    [VOICE_FIRST_SERVICE] = "FIRST SERVICE",
    [VOICE_SECOND_SERVICE] = "SECOND SERVICE",
    [VOICE_LET] = "LET!",
    [VOICE_TOUCH] = "TOUCH!",
    [GAME_POINT] = "GAME POINT",
    [SET_POINT] = "SET POINT",
    [MATCH_POINT] = "MATCH POINT",
};

#define TEXT_VOICE_SERVICE "SERVICE TO "
#define TEXT_VOICE_GAME "GAME "
#define TEXT_VOICE_SET "GAME, SET "
#define TEXT_VOICE_MATCH "GAME, SET, MATCH " // The & is not displaying nicely and we are using it for central 0

// ----------------------------------------------
// Umpire variables
// ----------------------------------------------

/**
 * Frame of the umpire sprite
 */
UmpireFrame umpire_frame = UMPIRE_FRAME_LOOK_CENTER;

/**
 * Used as a timer to remove the ballon from the screen
 */
uint8_t umpire_balloon_countdown = 0;

/**
 * A balloon is on the screen with corresponding voice playing
 */
bool voice_synced_balloon = false;

static char balloon_text_buffer[BALLOON_TEXT_MAX_LENGTH] = {0};

// ----------------------------------------------
// Internal prototypes
// ----------------------------------------------
void make_next_umpire_call(void);

void umpire_call_no_voice(const char *text);

void umpire_call(SoundSampleEnum voice, const char *text);

void umpire_call_named_player(PlayerId player_id, const char *text);

void umpire_call_named_player_voice(PlayerId player_id, SoundSampleEnum voice, const char *text);

void check_game_point(uint8_t player);

// ----------------------------------------------
// Public functions implementation
// ----------------------------------------------

void init_umpire(void)
{
    // The umpire is always looking at the center of the court
    umpire_frame = UMPIRE_FRAME_LOOK_CENTER;

    // The balloon is not shown
    hide_balloon();
    umpire_balloon_countdown = 0;
    voice_synced_balloon = false;

    clear_queue();
}

void update_umpire(void)
{
    // Do not follow the ball if the point is over or if a balloon is being displayed
    if (game_state.fadeout_timer == 0 && !voice_synced_balloon && umpire_balloon_countdown == 0)
    {
        int ball_distance_net = ball.y - NET_REFERENCE_Y;
        int abs_ball_distance_net = ball_distance_net < 0 ? -ball_distance_net : ball_distance_net;
        if (abs_ball_distance_net < UMPIRE_CENTRAL_AREA_HEIGHT / 2)
            umpire_frame = UMPIRE_FRAME_LOOK_CENTER;
        else if (ball_distance_net < 0)
            umpire_frame = UMPIRE_FRAME_LOOK_UP; // Ball is below the net
        else
            umpire_frame = UMPIRE_FRAME_LOOK_DOWN; // Ball is above the net
    }

    // Sets the head depending on the position of the ball

    if (umpire_balloon_countdown > 0)
    {
        // The balloon is still on the screen
        umpire_balloon_countdown--;
        if (umpire_balloon_countdown == 0)
        {
            hide_balloon();
            make_next_umpire_call();
        }
    }
    else if (voice_synced_balloon)
    {
        bool mouth_open = channel_b_remaining_length() & 0x0800;
        umpire_frame = mouth_open ? UMPIRE_FRAME_SPEAK : UMPIRE_FRAME_LOOK_CENTER;
        if (!is_sound_playing_channel_b())
        {
            // The voice has ended... we let the balloon some extra time
            umpire_balloon_countdown = EXTENDED_BALLOON_TIMEOUT;
            voice_synced_balloon = false;
        }
    }
    else
    {
        make_next_umpire_call();
    }
}

// ------------------------------------
// Umpire calls helper functions
// ------------------------------------

void make_next_umpire_call(void)
{
    UmpireMessage *message = get_message_from_queue();
    if (message != NULL)
    {
        // // The message is not empty
        if (message->sound != NO_SOUND_SAMPLE)
        {
            // Play the sound
            play_sound_channel_b(message->sound);
            voice_synced_balloon = true;
            if (message->text != NULL)
            {
                // Show the balloon with the text
                draw_balloon_with_text(message->text);
            }
        }
        else
        {
            // No sound, but show the balloon with the text
            draw_balloon_with_text(message->text);
            umpire_balloon_countdown = NO_VOICE_CALL_BALLOON_TIMEOUT;
        }
    }
}

void compose_named_player(PlayerId player_id, const char *text)
{
    Player *player = get_player(player_id);
    uint8_t action_length = strlen(text);
    uint8_t player_name_length = strlen(player->name);
    uint8_t total_length = action_length + player_name_length;
    if (total_length >= BALLOON_TEXT_MAX_LENGTH)
    {
        // The text is too long for the balloon
        total_length = BALLOON_TEXT_MAX_LENGTH - 1;
    }

    memcpy(balloon_text_buffer, text, action_length);
    memcpy(balloon_text_buffer + action_length, player->name, total_length - action_length);
    balloon_text_buffer[total_length] = '\0';
}

void umpire_call_named_player(PlayerId player_id, const char *text)
{
    compose_named_player(player_id, text);
    umpire_call_no_voice(balloon_text_buffer);
}

void umpire_call_named_player_voice(PlayerId player_id, SoundSampleEnum voice, const char *text)
{
    compose_named_player(player_id, text);
    umpire_call(voice, balloon_text_buffer);
}

void umpire_call_no_voice(const char *text)
{
    add_message_to_queue(NO_SOUND_SAMPLE, text);
}

void umpire_simple_call(SoundSampleEnum voice)
{
    add_message_to_queue(voice, umpire_call_texts[voice]);
}

void umpire_call(SoundSampleEnum voice, const char *text)
{
    add_message_to_queue(voice, text);
}

// ---------------------------------------------
// Umpire calls implementation
// ---------------------------------------------

void umpire_call_double_fault(void)
{
    umpire_simple_call(VOICE_DOUBLE_FAULT);
}

void umpire_call_fault(void)
{
    umpire_simple_call(VOICE_FAULT);
}

void umpire_call_let(void)
{
    umpire_simple_call(VOICE_LET);
}

void umpire_call_out(void)
{
    umpire_simple_call(VOICE_OUT);
}

void umpire_call_touch(void)
{
    umpire_simple_call(VOICE_TOUCH);
}

void umpire_call_service(void)
{
    // At the moment the service is announced, the player is not yet serving, so, service_owner points to the opposite
    umpire_call_named_player(game_state.service_owner ^ 1, TEXT_VOICE_SERVICE);
}

void umpire_call_first_service(void)
{
    umpire_simple_call(VOICE_FIRST_SERVICE);
}

void umpire_call_second_service(void)
{
    umpire_simple_call(VOICE_SECOND_SERVICE);
}

void umpire_call_game_for_player(uint8_t player)
{
    umpire_call_named_player_voice(player, game_state.service_owner == player ? GAME_SERVER : GAME_RECEIVER, TEXT_VOICE_GAME);
}

void umpire_call_set_for_player(uint8_t player)
{
    umpire_call_named_player_voice(player, game_state.service_owner == player ? SET_SERVER : SET_RECEIVER, TEXT_VOICE_SET);
}

void umpire_call_match_for_player(uint8_t player)
{
    umpire_call_named_player_voice(player, game_state.service_owner == player ? MATCH_SERVER : MATCH_RECEIVER, TEXT_VOICE_MATCH);
}

void umpire_call_score(void)
{
    uint8_t score_index = 0;

    const GamePoint server_score = score.points[game_state.service_owner];
    const GamePoint receiver_score = score.points[game_state.service_owner ^ 1];

    for (uint8_t i = 0; game_score_texts[server_score][i] != 0; i++)
    {
        balloon_text_buffer[score_index++] = game_score_texts[server_score][i];
    }
    balloon_text_buffer[score_index++] = ' ';
    balloon_text_buffer[score_index++] = '-';
    balloon_text_buffer[score_index++] = ' ';

    for (uint8_t i = 0; game_score_texts[receiver_score][i] != 0; i++)
    {
        balloon_text_buffer[score_index++] = game_score_texts[receiver_score][i];
    }

    balloon_text_buffer[score_index] = '\0';

    SoundSampleEnum sound = SCORE_0_15 + server_score * 4 + receiver_score - 1;
    // There is an exception in the case of ad-40
    if (server_score == GAME_ADVANTAGE)
        sound = SCORE_AD_SERVER;

    umpire_call(sound, balloon_text_buffer);
    check_game_point(0);
    check_game_point(1);
}

void check_game_point(uint8_t player)
{
    // TODO: tie break
    if ((score.points[player] == GAME_FORTY && score.points[player ^ 1] < GAME_FORTY) ||
        (score.points[player] == GAME_ADVANTAGE))
    {
        // Game point... maybe set point?
        if (score.games[player] >= match_type.n_games - 1 && (score.games[player] - score.games[player ^ 1]) >= 2)
        {
            // Set point
            if (score.sets[player] == match_type.n_sets - 1)
            {
                // Match point
                umpire_simple_call(MATCH_POINT);
            }
            else
            {
                umpire_simple_call(SET_POINT);
            }
        }
        else
        {
            umpire_simple_call(GAME_POINT);
        }
    }
}
