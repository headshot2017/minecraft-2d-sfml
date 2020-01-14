#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include "bass.h"
#include "settings_manager.h"
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
    SOUND_GLASS_STEP=25,

    SOUND_GRASS_DIG=29,
    SOUND_DIRT_DIG=33,
    SOUND_STONE_DIG=37,
    SOUND_SAND_DIG=41,
    SOUND_WOOD_DIG=45,
    SOUND_WOOL_DIG=49,
    SOUND_GLASS_DIG=53,

    SOUND_TOTAL=56
};

enum
{
    SAMPLE_TNT_FUSE,
    SAMPLE_EXPLODE,
    SAMPLE_TOTAL=5
};

enum
{
    MUSIC_CALM1,
    MUSIC_CALM2,
    MUSIC_CALM3,
    MUSIC_HAL1,
    MUSIC_HAL2,
    MUSIC_HAL3,
    MUSIC_HAL4,
    MUSIC_PIANO1,
    MUSIC_PIANO2,
    MUSIC_PIANO3,
    MUSIC_CREATIVE1,
    MUSIC_CREATIVE2,
    MUSIC_CREATIVE3,
    MUSIC_CREATIVE4,
    MUSIC_CREATIVE5,
    MUSIC_CREATIVE6,
    MUSIC_MENU1,
    MUSIC_MENU2,
    MUSIC_MENU3,
    MUSIC_MENU4,
    MUSIC_TOTAL=20
};

class SoundEngine
{
public:
    void init(SettingsManager *settings);
    void cleanup();
    bool loadTheme(const char *theme);
    bool unloadTheme();
    void updateVolume();

    HSTREAM getCurrentMusic() {return (m_currmusic > -1) ? m_music[m_currmusic] : HSTREAM(0);}

    void playMusic(int music);
    void stopMusic();

    void playGlassBreakSound(float player_x, float player_y, float x, float y);
    void playDigSound(float player_x, float player_y, float x, float y, int type);
    void playFootstepSound(float player_x, float player_y, float x, float y, int type);
    void playGameSound(float player_x, float player_y, float x, float y, int type, int multi=4);
    void playSample(float player_x, float player_y, float x, float y, int type, int multi=4);
    void playClickSound();

private:
    std::vector<HSTREAM> m_sounds;
    std::vector<HSTREAM> m_music;
    std::vector<HSAMPLE> m_samples;
    SettingsManager* m_settings;
    bool theme_loaded = false;
    int m_currmusic;
};

#endif // SOUND_H_INCLUDED
