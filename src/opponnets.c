#include <stdarg.h>
#include "opponents.h"
#include "tilemap.h"
#include <string.h>

#define PROFILE_X 46
#define PROFILE_Y 9
#define PROFILE_WIDTH 26
#define PROFILE_HEIGHT 20
#define SKILLS_ROW 15
#define SKILLS_COLUMN 5

#pragma codeseg PAGE_97
#pragma constseg PAGE_97

#define DESCRIPTION_ROGER_FEDEXPRESS "Face a tennis king\n"       \
                                     "and maybe live to tell.\n"  \
                                     "This is the Swiss artist\n" \
                                     "who floats on the court,\n" \
                                     "never seeming to sweat,\n"  \
                                     "making tough shots look\n"  \
                                     "as easy as sipping hot\n"   \
                                     "coffee at the mountains."

#define DESCRIPTION_SARAH_WOWVA "Cover your ears but keep\n"  \
                                "eyes on court. Prepare to\n" \
                                "face the Siberian machine\n" \
                                "whose screams register on\n" \
                                "seismic scales while her\n"  \
                                "groundstrokes deliver\n"     \
                                "with top-model precision\n"  \
                                "and cold-blooded intensity"

#define DESCRIPTION_JOHN_MCENRAGE "You can't be serious!\n"    \
                                  "Crowds will be thrilled\n"  \
                                  "as the volcanic American\n" \
                                  "argues with molecules\n"    \
                                  "while weaving net magic\n"  \
                                  "that defies physics and\n"  \
                                  "tests umpire patience\n"    \
                                  "with every loud burst."

#define DESCRIPTION_IVAN_TERRIBALL "The Czech machine is a\n"     \
                                   "tennis cyborg with a\n"       \
                                   "mechanical serve and a\n"     \
                                   "robotic backhand. He will\n"  \
                                   "unleash a barrage of shots\n" \
                                   "that will make you wonder\n"  \
                                   "your own sanity."

#define DESCRIPTION_KINKY_BORIS "Diving is your new skill\n"   \
                                "both on and off the court.\n" \
                                "Crowds roar as you face\n"    \
                                "Kinky Boris, whose serve\n"   \
                                "dodges racquets like he\n"    \
                                "dodges tax collectors.\n"     \
                                "His net charges were pure,\n" \
                                "his finance... not so much"

#define DESCRIPTION_GABI_SPLATINI "Beauty with a backhand\n"     \
                                  "so deadly, cameras turn.\n"   \
                                  "You trade shots with the\n"   \
                                  "Argentine rose whose grace\n" \
                                  "hypnotizes opponents as\n"    \
                                  "her perfume lingers much\n"   \
                                  "longer than your returns\n"   \
                                  "stay alive in her rallies."

#define DESCRIPTION_STEFFINATOR "Her forehand needs its own\n" \
                                "zip code and insurance.\n"    \
                                "The world stops to watch\n"   \
                                "you challenge Fraulein\n"     \
                                "Forehand, whose slice cuts\n" \
                                "diamonds and whose speed\n"   \
                                "covers more court than\n"     \
                                "airport runway lights."

#define DESCRIPTION_SHRED_HEAD_AGASSI "Image is everything when\n"   \
                                      "you walk onto the court.\n"   \
                                      "Cameras will flash bright\n"  \
                                      "as you rally with the bald\n" \
                                      "prophet of baseline play,\n"  \
                                      "returning serves before\n"    \
                                      "they even cross the net.\n"   \
                                      "He reads you like a sign."

#define DESCRIPTION_SNEAKY_PETE "Your return game? No hope.\n" \
                                "History will remember you\n"  \
                                "standing before Sneaky\n"     \
                                "Pete, whose deadly serve\n"   \
                                "should need a permit to\n"    \
                                "exist. His fast volleys\n"    \
                                "ended points in a blink,\n"   \
                                "before you could react."

#define DESCRIPTION_ICEBORG "Ice in his veins, band on\n" \
                            "his flowing Swedish hair.\n" \
                            "Future fans will worship\n"  \
                            "how you faced the Zen man\n" \
                            "whose baseline mastery\n"    \
                            "crushed foes while his\n"    \
                            "pulse stayed lower than\n"   \
                            "a polar bear in winter."

#define DESCRIPTION_NOVAX_DJOKOBRICK "No vaccine can stop his\n"   \
                                     "elastic, endless defense.\n" \
                                     "History will recall how\n"   \
                                     "you survived the Serb\n"     \
                                     "who returned missiles\n"     \
                                     "from neighboring lands,\n"   \
                                     "doing splits that made\n"    \
                                     "gymnasts just retire."

#define DESCRIPTION_RAFA_NODEAL "No hope vs him, but you'll\n" \
                                "have a legendary story to\n"  \
                                "share with your grandkids:\n" \
                                "'I faced the GOAT himself,\n" \
                                "saw that fierce backhand,\n"  \
                                "felt those brutal serves,\n"  \
                                "witnessed that footwork,\n"   \
                                "pure tennis perfection.'"

static const OpponentProfile opponent_profile[N_OPPONENTS] = {
    [OPPONENT_ROGER_FEDEXPRESS] = {
        .name = "ROGER FEDEXPRESS",
        .nationality = "Swiss",
        .style = "All-Court,Attacking",
        .left_handed = false,
        .description = DESCRIPTION_ROGER_FEDEXPRESS,
        .coming_soon = false,
        .skills = {4, 4, 5, 4, 5}},
    [OPPONENT_SARAH_WOWVA] = {.name = "SARAH WOWVA", .coming_soon = true, .nationality = "Russian", .style = "Baseline, Power", .left_handed = false, .description = DESCRIPTION_SARAH_WOWVA, .skills = {4, 3, 4, 3, 5}},
    [OPPONENT_JOHN_MCENRAGE] = {.name = "JOHN MCENRAGE", .coming_soon = true, .nationality = "American", .style = "Serve and Volley", .left_handed = true, .description = DESCRIPTION_JOHN_MCENRAGE, .skills = {3, 4, 5, 3, 5}},
    [OPPONENT_IVAN_TERRIBALL] = {.name = "IVAN TERRIBALL", .coming_soon = true, .nationality = "Czech", .left_handed = false, .style = "Baseline, Power", .description = DESCRIPTION_IVAN_TERRIBALL, .skills = {5, 3, 4, 4, 4}},
    [OPPONENT_KINKY_BORIS] = {.name = "KINKY BORIS", .coming_soon = true, .nationality = "German", .style = "Serve and Volley", .left_handed = false, .description = DESCRIPTION_KINKY_BORIS, .skills = {5, 4, 3, 3, 5}},
    [OPPONENT_GABI_SPLATINI] = {.name = "GABI SPLATINI", .coming_soon = true, .nationality = "Argentinian", .style = "All-Court", .left_handed = false, .description = DESCRIPTION_GABI_SPLATINI, .skills = {3, 4, 4, 3, 4}},
    [OPPONENT_STEFFINATOR] = {.name = "STEFFINATOR", .coming_soon = true, .nationality = "German", .style = "All-Court,Attacking", .left_handed = false, .description = DESCRIPTION_STEFFINATOR, .skills = {4, 5, 4, 4, 5}},
    [OPPONENT_SHRED_HEAD_AGASSI] = {.name = "SHRED HEAD AGASSI", .coming_soon = true, .nationality = "American", .style = "Baseline/Aggressive", .left_handed = false, .description = DESCRIPTION_SHRED_HEAD_AGASSI, .skills = {5, 4, 5, 3, 5}},
    [OPPONENT_SNEAKY_PETE] = {.name = "SNEAKY PETE", .coming_soon = true, .nationality = "American", .style = "Serve and Volley", .left_handed = false, .description = DESCRIPTION_SNEAKY_PETE, .skills = {4, 4, 4, 3, 5}},
    [OPPONENT_ICEBORG] = {.name = "BJORN ICEBORG", .coming_soon = true, .nationality = "Swedish", .style = "All-Court,Attacking", .left_handed = false, .description = DESCRIPTION_ICEBORG, .skills = {4, 5, 4, 5, 4}},
    [OPPONENT_NOVAX_DJOKOBRICK] = {.name = "NOVAX DJOKOBRICK", .coming_soon = true, .nationality = "Serbian", .style = "All-Court,Defensive", .left_handed = false, .description = DESCRIPTION_NOVAX_DJOKOBRICK, .skills = {4, 5, 5, 5, 4}},
    [OPPONENT_RAFA_NODEAL] = {.name = "RAFA NODEAL", .nationality = "Spanish", .style = "Baseline/Aggressive", .left_handed = true, .coming_soon = true, .description = DESCRIPTION_RAFA_NODEAL, .skills = {5, 5, 4, 5, 5}},
};

static void clear_profile(void)
{
    for (uint8_t x = 0; x < PROFILE_WIDTH; x++)
        for (uint8_t y = 0; y < PROFILE_HEIGHT; y++)
        {
            TileMapEntry *tentry = &tilemap[y + PROFILE_Y][x + PROFILE_X];
            tentry->character = ' ';
            tentry->palette = 3;
        }
}

static uint8_t write_text(uint8_t x, uint8_t y, uint8_t palette, const char *text)
{
    for (uint8_t i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '\n')
        {
            y++;
            x = 0;
        }
        else
        {
            TileMapEntry *tentry = &tilemap[y + PROFILE_Y][PROFILE_X + x++];
            tentry->character = text[i];
            tentry->palette = palette;
        }
    }

    return x;
}

static void center_text(uint8_t y, const char *text)
{
    uint8_t x = (PROFILE_WIDTH - strlen(text)) / 2;
    write_text(x, y, 4, text);
}

static void write_key_value(uint8_t x, uint8_t y, const char *key, const char *value)
{
    x = write_text(x, y, 4, key);
    write_text(x, y, 3, value);
}

static void write_skill(uint8_t y, const char *skill, uint8_t value)
{
    uint8_t x = write_text(SKILLS_COLUMN, y, 4, skill);
    for (uint8_t i = 0; i < value; i++)
    {
        TileMapEntry *tentry = &tilemap[y + PROFILE_Y][x + PROFILE_X + 5 - i];
        tentry->character = '*';
        tentry->palette = 3;
    }
}

void _diplay_profile(Opponent opponent)
{
    clear_profile();

    OpponentProfile *profile = &opponent_profile[opponent];
    center_text(0, profile->name);
    if (profile->coming_soon)
        write_text(6, 1, 5, "Coming soon!!!");
    write_key_value(0, 2, "Nationality: ", profile->nationality);
    write_key_value(0, 3, "Handedness:  ", profile->left_handed ? "Left" : "Right");
    write_key_value(0, 4, "Style: ", profile->style);
    write_text(0, 6, 3, profile->description);

    write_skill(SKILLS_ROW, "Strength: ", profile->skills[SKILL_STRENGTH]);
    write_skill(SKILLS_ROW + 1, "Speed:    ", profile->skills[SKILL_SPEED]);
    write_skill(SKILLS_ROW + 2, "Accuracy: ", profile->skills[SKILL_ACCURACY]);
    write_skill(SKILLS_ROW + 3, "Defense:  ", profile->skills[SKILL_DEFENSE]);
    write_skill(SKILLS_ROW + 4, "Attack:   ", profile->skills[SKILL_ATTACK]);
}