#include <stdlib.h>
#include <stdbool.h>
#include "game_state.h"
#include "ball.h"
#include "sound.h"
#include "player.h"
#include "player_transitions.h"
#include "court_dimensions.h"
#include "messages.h"
#include "score_board.h"
#include "computer_controls.h"
#include "umpire.h"

// ---------------------------------------------------------
// Constants
// ---------------------------------------------------------

#define DEFAULT_FADEOUT 30
#define SHORT_FADEOUT 10
#define END_FADEOUT 50

// ---------------------------------------------------------
// Game logical state variables
// ---------------------------------------------------------

// Initialized in start_match
GameState game_state;
Score score;

// Initialized in menu settings
MatchType match_type = {
    .n_games = 6,
    .n_sets = 2};

// ---------------------------------------------------------
// Internal function prototypes
// ---------------------------------------------------------

void start_set(void);

void start_game(void);

void start_point(void);

void start_service(void);

void ball_is_bouncing_out(void);

void ball_bounced_out(bool own_area);

void ball_bounced_twice(void);

/**
 * Finished the point... point for playerID
 */
void finish_point(PlayerId playerId);

void finish_game(PlayerId playerId);

void finish_set(PlayerId playerId);

void finish_match(PlayerId playerId);

void end_match(void);

void ball_touched_player(void);

// ---------------------------------------------------------
// Helper functions
// ---------------------------------------------------------
bool ball_in_boundary(CourtArea area)
{
    const Boundary *boundary = &court_boundaries[area];
    return ball.x >= boundary->left && ball.x <= boundary->right && ball.y >= boundary->top && ball.y <= boundary->bottom;
}

Player *get_serving_player(void)
{
    return game_state.service_owner == PLAYER_TOP ? &player_top : &player_bottom;
}

const Boundary *service_area(void)
{
    return &court_boundaries[COURT_AREA_UPPER_DEUCE_SERVE + (is_ad_service() ? 2 : 0) + (game_state.service_owner == PLAYER_BOTTOM ? 0 : 1)];
}

const Boundary *valid_area(PlayerId player_id)
{
    return &court_boundaries[COURT_AREA_VALID_UPPER + (player_id == PLAYER_BOTTOM ? 0 : 1)];
}

bool is_ad_service(void)
{
    return (score.points[0] + score.points[1]) & 1;
}

// ---------------------------------------------------------
// Function implemementation
// ---------------------------------------------------------

void fade_out(uint8_t timer, void (*fn)(void))
{
    game_state.fadeout_timer = timer;
    game_state.fadeout_fn = fn;
}

void update_entities(void)
{
    if (game_state.fadeout_timer > 0)
    {
        game_state.fadeout_timer--;
        if (game_state.fadeout_timer == 0)
        {
            game_state.fadeout_fn();
        }
    }

    update_ball();
    update_players();
    update_umpire();
    // No camera in favour of playability
    // update_camera();
}

/**
 * Start of a new match
 */
void start_match(void)
{
    init_controls();
    init_umpire();

#ifdef DEBUG
    game_state.service_owner = PLAYER_TOP;
#else
    // First server is randomly selected
    game_state.service_owner = rand() % 2;
#endif

    umpire_call_service();

    // Initializes sets
    score.sets[0] = 0;
    score.sets[1] = 0;

    // No fadeout
    game_state.fadeout_timer = 0;
    game_state.fadeout_fn = NULL;

    // Score board updated with player names
    init_score_board();
    show_score_board();

    start_set();
}

void start_set(void)
{
    score.games[0] = 0;
    score.games[1] = 0;
    show_score_board();

    start_game();
}

void start_game(void)
{
    game_state.service_owner = (game_state.service_owner + 1) % 2;
    score.points[0] = GAME_ZERO;
    score.points[1] = GAME_ZERO;
    show_score_board();

    start_point();
}

void start_point(void)
{
    game_state.second_service = false;

    start_service();
}

void start_service(void)
{
    game_state.touched_net = false;
    game_state.point_status = POS_SERVICE_START;
    restart_computer_controls();
    Player *serving_player;
    Player *receiving_player;
    if (game_state.service_owner == PLAYER_TOP)
    {
        serving_player = &player_top;
        receiving_player = &player_bottom;
    }
    else
    {
        serving_player = &player_bottom;
        receiving_player = &player_top;
    }
    transition_serve_ready(serving_player);
    transition_receive_serve(receiving_player);
    // send_message(game_state.second_service ? "Second service" : "First service");
    // TODO: transition the other player to receive service
}

// --------------------------------------------------------------------
// Events implementation
// --------------------------------------------------------------------

/**
 * Event triggered when the ball bounces
 */
void event_ball_bounced(void)
{
    if (game_state.fadeout_timer > 0)
        return;

    if (game_state.point_status == POS_SERVICE_START)
    {
        // It's not a fault. The player did not try to hit the ball
        fade_out(DEFAULT_FADEOUT, start_service);
    }
    else if (game_state.point_status == POS_SERVICE_HIT || game_state.point_status == POS_SERVICE_HIT_ATTEMPT)
    {
        CourtArea valid_area = (is_ad_service() ? COURT_AREA_UPPER_AD_SERVE : COURT_AREA_UPPER_DEUCE_SERVE) | (game_state.service_owner ^ 1);
        if (ball_in_boundary(valid_area))
        {
            // Ball bounced IN of valid area
            if (game_state.touched_net)
            {
                // Let... Repeat service
                umpire_call_let();
                if (game_state.second_service)
                    umpire_call_second_service();
                else
                    umpire_call_first_service();
                fade_out(DEFAULT_FADEOUT, start_service);
            }
            else
            {
                game_state.point_status = POS_BOUNCED_IN;
            }
        }
        else
        {
            ball_is_bouncing_out();
            // fade_out(SHORT_FADEOUT, ball_bounced_out);
        }
    }
    else if (game_state.point_status == POS_HIT)
    {
        if (ball_in_boundary(COURT_AREA_VALID_UPPER | (game_state.ball_owner ^ 1)))
            game_state.point_status = POS_BOUNCED_IN;
        else
            ball_is_bouncing_out();
    }
    else if (game_state.point_status == POS_BOUNCED_IN)
    {
        fade_out(SHORT_FADEOUT, ball_bounced_twice);
    }
    else
    {
        // Second bounce of the ball... game for the ball_owner
        finish_point(game_state.service_owner);
    }
}

// TODO: rename to avoid confusion with player events

/**
 * Event triggered when a player hits the ball
 */
void event_player_hit(PlayerId player)
{
    if (game_state.fadeout_timer > 0)
        return;

    // We are in the service phase
    if (game_state.point_status == POS_SERVICE_START)
    {
        game_state.point_status = POS_SERVICE_HIT;
        game_state.ball_owner = player;
    }
    else if (game_state.point_status == POS_SERVICE_HIT)
    {
        // The player touches the ball during service before it hits the ground
        // Point for the opponent
        finish_point(player ^ 1);
    }
    else if (game_state.point_status == POS_BOUNCED_IN || game_state.point_status == POS_HIT)
    {
        if (game_state.ball_owner == player)
        {
            // Second touch of the ball by the same player
            finish_point(player ^ 1);
        }
        else
        {
            // Correct hit of the ball
            game_state.point_status = POS_HIT;
            game_state.ball_owner = player;
        }
    }
    else
    {
        // Point is over... but we need this for the calculation of the speed of the ball
        game_state.ball_owner = player;
    }
}

/**
 * Event triggered when the ball touches the net
 */
void event_net_touched(void)
{
    game_state.touched_net = true;
}

void event_player_touch(Player *player)
{
    if (game_state.fadeout_timer > 0 || game_state.point_status == POS_END_MATCH)
        return;

    game_state.touched_ball = player->id;
    transition_touch(player);

    fade_out(DEFAULT_FADEOUT, ball_touched_player);
}

// Invoked after a timeout if there's been touch. This leaves some time of the animation and player sound
void ball_touched_player(void)
{
    umpire_call_touch();
    finish_point(game_state.touched_ball ^ 1);
}

void ball_bounced_out_own_area(void)
{
    ball_bounced_out(true);
}

void ball_bounced_out_opponent_area(void)
{
    ball_bounced_out(false);
}

void ball_is_bouncing_out(void)
{
    bool bouncing_top = ball.y < NET_REFERENCE_Y;
    fade_out(SHORT_FADEOUT, bouncing_top ^ game_state.ball_owner == PLAYER_TOP ? ball_bounced_out_opponent_area : ball_bounced_out_own_area);
}

/**
 * The ball bounced out of the valid area... triggered after a certain delay of the actual ball hitting the
 * ground (called by ball_bounced_out_own_area / ball_bounced_out_opponent_area)
 */
void ball_bounced_out(bool own_area)
{
    if (game_state.point_status == POS_SERVICE_HIT || game_state.point_status == POS_SERVICE_HIT_ATTEMPT)
    {
        if (game_state.second_service)
        {
            umpire_call_double_fault();
            finish_point(game_state.service_owner ^ 1);
        }
        else
        {
            umpire_call_fault();
            game_state.second_service = true;
            fade_out(DEFAULT_FADEOUT, start_service);
        }
    }
    else
    {
        // Avoid call out if evident
        if (!own_area)
            umpire_call_out();
        finish_point(game_state.ball_owner ^ 1);
    }
}

void ball_bounced_twice(void)
{
    finish_point(game_state.ball_owner);
}

/**
 * Finishes the point...
 * Updates score for playerID and moves to the next step with a fade_out
 */
void finish_point(PlayerId playerId)
{
    bool tie_break = score.games[0] == score.games[1] && score.games[0] == match_type.n_games;
    GamePoint pointsPlayer = score.points[playerId];
    GamePoint pointsOpponent = score.points[playerId ^ 1];

    if (tie_break)
    {
        // score.points[playerId]++;
        // TODO: implement tie break
    }
    else
    {
        if (pointsPlayer == GAME_FORTY)
        {
            if (pointsOpponent < GAME_FORTY)
            {
                finish_game(playerId);
                show_score_board();
                return;
            }
            else if (pointsOpponent == GAME_FORTY)
            {
                // Advantage for player
                score.points[playerId] = GAME_ADVANTAGE;
            }
            else
            {
                // Deuce
                score.points[playerId ^ 1] = GAME_FORTY;
            }
        }
        else if (pointsPlayer == GAME_ADVANTAGE)
        {
            finish_game(playerId);
            show_score_board();
            return;
        }
        else
        {
            score.points[playerId]++;
        }
    }

    show_score_board();
    umpire_call_score();
    fade_out(DEFAULT_FADEOUT, start_point);
}

void finish_game(PlayerId playerId)
{
    score.games[playerId]++;

    int8_t gamesDiff = score.games[playerId] - score.games[playerId ^ 1];
    if ((score.games[playerId] == match_type.n_games && gamesDiff >= 2) || score.games[playerId] > match_type.n_games)
        finish_set(playerId);
    else
    {
        umpire_call_game_for_player(playerId);
        fade_out(DEFAULT_FADEOUT, start_game);
    }
}

void finish_set(PlayerId playerId)
{
    score.sets[playerId]++;
    if (score.sets[playerId] >= match_type.n_sets)
    {
        finish_match(playerId);
    }
    else
    {
        umpire_call_game_for_player(playerId);
        fade_out(DEFAULT_FADEOUT, start_set);
    }
}

void finish_match(PlayerId playerId)
{
    umpire_call_match_for_player(playerId);
    fade_out(END_FADEOUT, end_match);
}

// The game loop will check for this condition to end
void end_match(void)
{
    game_state.point_status = POS_END_MATCH;
}

// /**
//  * Updates the score with a point for player Id
//  * Returns true if the player wins the match
//  */
// bool update_score(PlayerId playerId)