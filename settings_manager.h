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
    bool m_fullscreen = false;
    unsigned int m_screenwidth = 800;
    unsigned int m_screenheight = 480;
    int m_mastervol = 100;
    int m_musicvol = 100;
    int m_soundvol = 100;
    char m_playername[16];
    char m_playerskin[32];

private:
    Controls m_controls;
};

#endif // SETTINGS_MANAGER_H_INCLUDED
