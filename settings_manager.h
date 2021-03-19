#ifndef SETTINGS_MANAGER_H_INCLUDED
#define SETTINGS_MANAGER_H_INCLUDED

#include "controls.h"

class SettingsManager
{
public:
    SettingsManager();

    void generateSettings();
    void loadSettings();
    void saveSettings();
    Controls* controls() {return &m_controls;}

    bool m_layerlighting; // not used
    bool m_fullscreen;
    unsigned int m_screenwidth;
    unsigned int m_screenheight;
    int m_sounddevice;
    int m_mastervol;
    int m_musicvol;
    int m_soundvol;
    char m_playername[16];
    char m_playerskin[32];
    char m_soundtheme[32];

private:
    Controls m_controls;
};

#endif // SETTINGS_MANAGER_H_INCLUDED
