#include "sound.h"
#include <stdio.h>
#include "math.h"

void SoundEngine::init(SettingsManager *settings)
{
    m_settings = settings;

    m_currmusic = -1;
    m_music.resize(MUSIC_TOTAL);
    m_sounds.resize(SOUND_TOTAL);
    m_samples.resize(SAMPLE_TOTAL);
    BASS_Init(settings->m_sounddevice, 44100, 0, 0, 0);

    char aFile[128];
    for (unsigned i=0; i<m_music.size(); i++)
    {
        if (i < 3) // calm.ogg
        {
            sprintf(aFile, "data/music/calm%d.ogg", i+1);
            m_music[i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        }
        else if (i < 7) // hal.ogg
        {
            sprintf(aFile, "data/music/hal%d.ogg", i-3+1);
            m_music[i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        }
        else if (i < 10) // piano.ogg
        {
            sprintf(aFile, "data/music/piano%d.ogg", i-7+1);
            m_music[i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        }
        else if (i < 16) // menu.ogg
        {
            sprintf(aFile, "data/music/creative/creative%d.ogg", i-10+1);
            m_music[i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        }
        else if (i < 20) // creative.ogg
        {
            sprintf(aFile, "data/music/menu/menu%d.ogg", i-16+1);
            m_music[i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
        }
    }
    theme_loaded = false;
}

void SoundEngine::cleanup()
{
    for (HSTREAM snd : m_music) BASS_StreamFree(snd);
    m_currmusic = -1;
    unloadTheme();
    BASS_Free();
    theme_loaded = false;
}

bool SoundEngine::loadTheme(const char *theme)
{
    if (theme_loaded)
        unloadTheme();
    curr_theme = theme;

    char aFile[128];
    sprintf(aFile, "data/sounds/%s/click.wav", theme);
    m_sounds[SOUND_CLICK] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
    sprintf(aFile, "data/sounds/%s/splash_weak.wav", theme);
    m_sounds[SOUND_SPLASH_WEAK] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
    sprintf(aFile, "data/sounds/%s/splash_strong.wav", theme);
    m_sounds[SOUND_SPLASH_STRONG] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
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
        sprintf(aFile, "data/sounds/%s/step/glass%d.wav", theme, i+1);
        m_sounds[SOUND_GLASS_STEP+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);

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
        sprintf(aFile, "data/sounds/%s/dig/wool%d.wav", theme, i+1);
        m_sounds[SOUND_WOOL_DIG+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);

        sprintf(aFile, "data/sounds/%s/explode%d.wav", theme, i+1);
        m_samples[SAMPLE_EXPLODE+i] = BASS_SampleLoad(false, aFile, 0, 0, 16384, BASS_SAMPLE_OVER_POS);
    }

    for(int i=0; i<3; i++)
    {
        sprintf(aFile, "data/sounds/%s/glass%d.wav", theme, i+1);
        m_sounds[SOUND_GLASS_DIG+i] = BASS_StreamCreateFile(false, aFile, 0, 0, 0);
    }

    theme_loaded = true;
    return true;
}

bool SoundEngine::unloadTheme()
{
    if (not theme_loaded) return false;
    curr_theme = nullptr;

    for (HSTREAM snd : m_sounds) BASS_StreamFree(snd);
    for (HSTREAM snd : m_samples) BASS_StreamFree(snd);

    theme_loaded = false;
    return true;
}

void SoundEngine::playMusic(int music)
{
    float m_mastervol = m_settings->m_mastervol / 100.f;
    float m_musicvol = m_settings->m_musicvol / 100.f;

    stopMusic();
    BASS_ChannelSetAttribute(m_music[music], BASS_ATTRIB_VOL, m_mastervol * m_musicvol);
    BASS_ChannelPlay(m_music[music], true);
    m_currmusic = music;
}

void SoundEngine::stopMusic()
{
    if (m_currmusic < 0) return;
    BASS_ChannelStop(m_music[m_currmusic]);
    m_currmusic = -1;
}

void SoundEngine::playClickSound()
{
    float m_mastervol = m_settings->m_mastervol / 100.f;
    float m_soundvol = m_settings->m_soundvol / 100.f;

    BASS_ChannelSetAttribute(m_sounds[SOUND_CLICK], BASS_ATTRIB_VOL, m_mastervol * m_soundvol);
    BASS_ChannelSetAttribute(m_sounds[SOUND_CLICK], BASS_ATTRIB_PAN, 0.f);
    BASS_ChannelPlay(m_sounds[SOUND_CLICK], true);
}

void SoundEngine::playGlassBreakSound(float player_x, float player_y, float x, float y)
{
    playGameSound(player_x, player_y, x, y, SOUND_GLASS_DIG, 3);
}

void SoundEngine::playDigSound(float player_x, float player_y, float x, float y, int type)
{
    int blocktype = SOUND_STONE_DIG;
    int multi = 4;

    if (type == BLOCK_GRASS or type == BLOCK_OAK_LEAVES or type == BLOCK_TNT)
        blocktype = SOUND_GRASS_DIG;
    else if (type == BLOCK_DIRT or type == BLOCK_GRAVEL)
        blocktype = SOUND_DIRT_DIG;
    else if (type == BLOCK_OAK_WOOD or type == BLOCK_OAK_PLANKS or type == BLOCK_BOOKSHELF or type == BLOCK_CRAFTINGTABLE)
        blocktype = SOUND_WOOD_DIG;
    else if (type == BLOCK_SAND)
        blocktype = SOUND_SAND_DIG;
    else if (type == BLOCK_WOOL or type == BLOCK_CACTUS)
        blocktype = SOUND_WOOL_DIG;
    else if (type == BLOCK_GLOWSTONE or type == BLOCK_ICE or type == BLOCK_GLASS)
        blocktype = SOUND_GLASS_STEP;

    playGameSound(player_x, player_y, x, y, blocktype, multi);
}

void SoundEngine::playFootstepSound(float player_x, float player_y, float x, float y, int type)
{
    int blocktype = SOUND_STONE_STEP;

    if (type == BLOCK_GRASS or type == BLOCK_OAK_LEAVES or type == BLOCK_TNT)
        blocktype = SOUND_GRASS_STEP;
    else if (type == BLOCK_DIRT or type == BLOCK_GRAVEL)
        blocktype = SOUND_DIRT_STEP;
    else if (type == BLOCK_OAK_WOOD or type == BLOCK_OAK_PLANKS or type == BLOCK_BOOKSHELF or type == BLOCK_CRAFTINGTABLE)
        blocktype = SOUND_WOOD_STEP;
    else if (type == BLOCK_SAND)
        blocktype = SOUND_SAND_STEP;
    else if (type == BLOCK_WOOL or type == BLOCK_CACTUS)
        blocktype = SOUND_WOOL_STEP;
    else if (type == BLOCK_GLOWSTONE or type == BLOCK_ICE or type == BLOCK_GLASS)
        blocktype = SOUND_GLASS_STEP;

    playGameSound(player_x, player_y, x, y, blocktype);
}

void SoundEngine::playWaterSplashSound(float player_x, float player_y, float x, float y, float vspeed)
{
    float m_mastervol = m_settings->m_mastervol / 100.f;
    float m_soundvol = m_settings->m_soundvol / 100.f;

    float vol, pan, dist;
    dist = sqrt(pow(x - player_x, 2) + pow(y - player_y, 2));

    if (dist < 500)
        vol = (500.0f - dist) / 500.0f;
    else
        vol = 0;

    pan = (-x + player_x) / 500.0f;
    vol *= vspeed/15.0f;
    vol = (vol < 0) ? 0 : (vol > 1) ? 1 : vol;
    int type = (vspeed >= 8) ? SOUND_SPLASH_STRONG : SOUND_SPLASH_WEAK;

    BASS_ChannelSetAttribute(m_sounds[type], BASS_ATTRIB_VOL, vol * m_mastervol * m_soundvol);
    BASS_ChannelSetAttribute(m_sounds[type], BASS_ATTRIB_PAN, pan);
    BASS_ChannelPlay(m_sounds[type], true);
}

void SoundEngine::playGameSound(float player_x, float player_y, float x, float y, int type, int multi)
{
    float m_mastervol = m_settings->m_mastervol / 100.f;
    float m_soundvol = m_settings->m_soundvol / 100.f;

    float vol, pan, dist;
    dist = sqrt(pow(x - player_x, 2) + pow(y - player_y, 2));

    if (dist < 500)
        vol = (500.0f - dist) / 500.0f;
    else
        vol = 0;

    pan = (-x + player_x) / 500.0f;

    int ind = (multi) ? rand() % multi : 0;
    BASS_ChannelSetAttribute(m_sounds[type+ind], BASS_ATTRIB_VOL, vol * m_mastervol * m_soundvol);
    BASS_ChannelSetAttribute(m_sounds[type+ind], BASS_ATTRIB_PAN, pan);
    BASS_ChannelPlay(m_sounds[type+ind], true);
}

void SoundEngine::playSample(float player_x, float player_y, float x, float y, int type, int multi)
{
    float m_mastervol = m_settings->m_mastervol / 100.f;
    float m_soundvol = m_settings->m_soundvol / 100.f;

    float vol, pan, dist;
    dist = sqrt(pow(x - player_x, 2) + pow(y - player_y, 2));

    if (dist < 500)
        vol = (500.0f - dist) / 500.0f;
    else
        vol = 0;

    pan = (-x + player_x) / 500.0f;

    int ind = (multi) ? rand() % multi : 0;

    HCHANNEL channel = BASS_SampleGetChannel(m_samples[type+ind], false);
    BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, vol * m_mastervol * m_soundvol);
    BASS_ChannelSetAttribute(channel, BASS_ATTRIB_PAN, pan);
    BASS_ChannelPlay(channel, false);
}

void SoundEngine::updateVolume()
{
    float m_mastervol = m_settings->m_mastervol / 100.f;
    float m_musicvol = m_settings->m_musicvol / 100.f;
    BASS_ChannelSetAttribute(getCurrentMusic(), BASS_ATTRIB_VOL, m_mastervol * m_musicvol);
}
