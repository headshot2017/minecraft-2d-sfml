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

    bool m_layerlighting = true;
    char m_playername[16];
    char m_playerskin[32];

private:
    Controls m_controls;
};

#endif // SETTINGS_MANAGER_H_INCLUDED
