#ifndef _OPPONENT_H_
#define _OPPONENT_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    OPPONENT_ROGER_FEDEXPRESS,
    OPPONENT_SARAH_WOWVA,
    OPPONENT_JOHN_MCENRAGE,
    OPPONENT_IVAN_TERRIBALL,
    OPPONENT_KINKY_BORIS,
    OPPONENT_GABI_SPLATINI,
    OPPONENT_STEFFINATOR,
    OPPONENT_SHRED_HEAD_AGASSI,
    OPPONENT_SNEAKY_PETE,
    OPPONENT_ICEBORG,
    OPPONENT_NOVAX_DJOKOBRICK,
    OPPONENT_RAFA_NODEAL,
    N_OPPONENTS
} Opponent;

typedef enum
{
    SKILL_STRENGTH,
    SKILL_SPEED,
    SKILL_ACCURACY,
    SKILL_DEFENSE,
    SKILL_ATTACK,
    N_SKILLS
} ProfileSkill;

typedef struct
{
    const char *name;
    const char *nationality;
    bool left_handed;
    const char *style;
    const char *description;
    uint8_t skills[N_SKILLS];
    bool coming_soon;
} OpponentProfile;

extern OpponentProfile ppponent_profile[N_OPPONENTS];

void diplay_profile(Opponent opponent);

#endif