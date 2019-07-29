#include "sound.h"
#include <stdio.h>
#include "math.h"

void SoundEngine::init()
{
    m_sounds.resize(SOUND_TOTAL);
    m_samples.resize(SAMPLE_TOTAL);
    BASS_Init(-1, 44100, 0, 0, 0);
    theme_loaded = false;
}

void SoundEngine::cleanup()
{
    unloadTheme();
    BASS_Free();
    theme_loaded = false;
}

bool SoundEngine::loadTheme(const char *theme)
{
    if (theme_loaded)
        unloadTheme();

    char aFile[128];
    sprintf(aFile, "data/sounds/%s/click.wav", theme);
    m_sounds[SOUND_CLICK] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
    sprintf(aFile, "data/sounds/%s/fuse.wav", theme);
    m_samples[SAMPLE_TNT_FUSE] = BASS_SampleLoad(false, aFile, 0, 0, 65536/4, BASS_SAMPLE_OVER_POS);
    for(int i=0; i<4; i++)
    {
        sprintf(aFile, "data/sounds/%s/step/grass%d.wav", theme, i+1);
        m_sounds[SOUND_GRASS_STEP+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        sprintf(aFile, "data/sounds/%s/step/dirt%d.wav", theme, i+1);
        m_sounds[SOUND_DIRT_STEP+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        sprintf(aFile, "data/sounds/%s/step/stone%d.wav", theme, i+1);
        m_sounds[SOUND_STONE_STEP+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        sprintf(aFile, "data/sounds/%s/step/sand%d.wav", theme, i+1);
        m_sounds[SOUND_SAND_STEP+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        sprintf(aFile, "data/sounds/%s/step/wood%d.wav", theme, i+1);
        m_sounds[SOUND_WOOD_STEP+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        sprintf(aFile, "data/sounds/%s/step/wool%d.wav", theme, i+1);
        m_sounds[SOUND_WOOL_STEP+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);

        sprintf(aFile, "data/sounds/%s/dig/grass%d.wav", theme, i+1);
        m_sounds[SOUND_GRASS_DIG+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        sprintf(aFile, "data/sounds/%s/dig/dirt%d.wav", theme, i+1);
        m_sounds[SOUND_DIRT_DIG+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        sprintf(aFile, "data/sounds/%s/dig/stone%d.wav", theme, i+1);
        m_sounds[SOUND_STONE_DIG+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        sprintf(aFile, "data/sounds/%s/dig/sand%d.wav", theme, i+1);
        m_sounds[SOUND_SAND_DIG+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        sprintf(aFile, "data/sounds/%s/dig/wood%d.wav", theme, i+1);
        m_sounds[SOUND_WOOD_DIG+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        sprintf(aFile, "data/sounds/%s/step/wool%d.wav", theme, i+1);
        m_sounds[SOUND_WOOL_DIG+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);

        sprintf(aFile, "data/sounds/%s/explode%d.wav", theme, i+1);
        m_samples[SAMPLE_EXPLODE+i] = BASS_SampleLoad(false, aFile, 0, 0, 65536/4, BASS_SAMPLE_OVER_POS);
    }

    theme_loaded = true;
    return true;
}

bool SoundEngine::unloadTheme()
{
    if (not theme_loaded) return false;

    for(int i=0; i<SOUND_TOTAL; i++)
        BASS_StreamFree(m_sounds[i]);
    for(int i=0; i<SAMPLE_TOTAL; i++)
        BASS_SampleFree(m_samples[i]);

    theme_loaded = false;
    return true;
}

void SoundEngine::playClickSound()
{
    BASS_ChannelSetAttribute(m_sounds[SOUND_CLICK], BASS_ATTRIB_VOL, 1.0f);
    BASS_ChannelSetAttribute(m_sounds[SOUND_CLICK], BASS_ATTRIB_PAN, 0.f);
    BASS_ChannelPlay(m_sounds[SOUND_CLICK], true);
}

void SoundEngine::playDigSound(float player_x, float player_y, float x, float y, int type)
{
    int blocktype = SOUND_STONE_DIG;

    if (type == BLOCK_GRASS or type == BLOCK_OAK_LEAVES or type == BLOCK_TNT)
        blocktype = SOUND_GRASS_DIG;
    else if (type == BLOCK_DIRT or type == BLOCK_GRAVEL)
        blocktype = SOUND_DIRT_DIG;
    else if (type == BLOCK_OAK_WOOD or type == BLOCK_OAK_PLANKS)
        blocktype = SOUND_WOOD_DIG;
    else if (type == BLOCK_SAND)
        blocktype = SOUND_SAND_DIG;

    playGameSound(player_x, player_y, x, y, blocktype);
}

void SoundEngine::playFootstepSound(float player_x, float player_y, float x, float y, int type)
{
    int blocktype = SOUND_STONE_STEP;

    if (type == BLOCK_GRASS or type == BLOCK_OAK_LEAVES or type == BLOCK_TNT)
        blocktype = SOUND_GRASS_STEP;
    else if (type == BLOCK_DIRT or type == BLOCK_GRAVEL)
        blocktype = SOUND_DIRT_STEP;
    else if (type == BLOCK_OAK_WOOD or type == BLOCK_OAK_PLANKS)
        blocktype = SOUND_WOOD_STEP;
    else if (type == BLOCK_SAND)
        blocktype = SOUND_SAND_STEP;

    playGameSound(player_x, player_y, x, y, blocktype);
}

void SoundEngine::playGameSound(float player_x, float player_y, float x, float y, int type, bool multi)
{
    float vol, pan, dist;
    dist = sqrt(pow(x - player_x, 2) + pow(y - player_y, 2));

    if (dist < 500)
        vol = (500.0f - dist) / 500.0f;
    else
        vol = 0;

    pan = (-x + player_x) / 500.0f;

    int ind = (multi) ? rand() % 4 : 0;
    BASS_ChannelSetAttribute(m_sounds[type+ind], BASS_ATTRIB_VOL, vol);
    BASS_ChannelSetAttribute(m_sounds[type+ind], BASS_ATTRIB_PAN, pan);
    BASS_ChannelPlay(m_sounds[type+ind], true);
}

void SoundEngine::playSample(float player_x, float player_y, float x, float y, int type, bool multi)
{
    float vol, pan, dist;
    dist = sqrt(pow(x - player_x, 2) + pow(y - player_y, 2));

    if (dist < 500)
        vol = (500.0f - dist) / 500.0f;
    else
        vol = 0;

    pan = (-x + player_x) / 500.0f;

    int ind = (multi) ? rand() % 4 : 0;

    HCHANNEL channel = BASS_SampleGetChannel(m_samples[type+ind], true);
    BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, vol);
    BASS_ChannelSetAttribute(channel, BASS_ATTRIB_PAN, pan);
    BASS_ChannelPlay(channel, false);
}
