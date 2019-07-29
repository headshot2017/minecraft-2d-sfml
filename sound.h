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
    SOUND_WOOL_STEP=21,

    SOUND_GRASS_DIG=25,
    SOUND_DIRT_DIG=29,
    SOUND_STONE_DIG=33,
    SOUND_SAND_DIG=37,
    SOUND_WOOD_DIG=41,
    SOUND_WOOL_DIG=45,

    SOUND_TOTAL=54
};

enum
{
    SAMPLE_TNT_FUSE,
    SAMPLE_EXPLODE,
    SAMPLE_TOTAL=5
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
    void playGameSound(float player_x, float player_y, float x, float y, int type, bool multi=true);
    void playSample(float player_x, float player_y, float x, float y, int type, bool multi=true);
    void playClickSound();

private:
    std::vector<HSTREAM> m_sounds;
    std::vector<HSAMPLE> m_samples;
    //GameEngine* m_engine;
    bool theme_loaded = false;
};

#endif // SOUND_H_INCLUDED
