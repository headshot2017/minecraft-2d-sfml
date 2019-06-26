#include "settings_manager.h"
#include <fstream>

SettingsManager::SettingsManager()
{
    loadSettings();
}

void SettingsManager::loadSettings()
{
    std::ifstream configfile("options.txt", std::ios::binary);
    if (configfile.is_open())
    {

    }
    else
        printf("could not open \"options.txt\"\n");
}

void SettingsManager::saveSettings()
{
    printf("settings saved\n");
}
