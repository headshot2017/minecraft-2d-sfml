#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include "bass.h"
#include "game/block.h"
#include <vector>

enum
{
    SOUND_CLICK,

    SOUND_GRASS_STEP,
    SOUND_DIRT_STEP=5,
    SOUND_STONE_STEP=9,
    SOUND_SAND_STEP=13,
    SOUND_WOOD_STEP=17,

    SOUND_GRASS_DIG=21,
    SOUND_DIRT_DIG=25,
    SOUND_STONE_DIG=29,
    SOUND_SAND_DIG=33,
    SOUND_WOOD_DIG=37,

    SOUND_TOTAL=41
};

class SoundEngine
{
public:
    void init();
    void cleanup();
    bool loadTheme(const char *theme);
    bool unloadTheme();

    void playDigSound(float player_x, float player_y, float x, float y, int type);
    void playFootstepSound(float player_x, float player_y, float x, float y, int type);
    void playGameSound(float player_x, float player_y, float x, float y, int type);
    void playClickSound();

private:
    std::vector<HSTREAM> m_sounds;
    //GameEngine* m_engine;
    bool theme_loaded = false;
};

#endif // SOUND_H_INCLUDED
