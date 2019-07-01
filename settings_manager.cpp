#include "settings_manager.h"
#include <fstream>
#include <algorithm>
#include <cstring>

SettingsManager::SettingsManager()
{

}

void SettingsManager::generateSettings()
{
    sprintf(m_playername, "Alexn't");
    sprintf(m_playerskin, "steve");
    saveSettings();
}

void SettingsManager::loadSettings()
{
    std::ifstream configfile("options.txt");
    if (configfile.is_open())
    {
        std::string line;
        while (getline(configfile, line))
        {
            line.erase(std::remove_if(line.begin(), line.end(), isspace),
                                 line.end());
            if (line[0] == '#' or line.empty())
                continue;
            auto delimiterPos = line.find(":");
            std::string name = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            if (strcmp(name.c_str(), "player_name") == 0)
                sprintf(m_playername, "%s", value.c_str());
            else if (strcmp(name.c_str(), "player_skin") == 0)
                sprintf(m_playerskin, "%s", value.c_str());

            printf("%s : %s\n", name.c_str(), value.c_str());
        }
    }
    else
    {
        printf("could not open \"options.txt\", generating one\n");
        generateSettings();
    }
}

void SettingsManager::saveSettings()
{
    std::ofstream configfile("options.txt");
    configfile << "player_name: " << m_playername << "\n";
    configfile << "player_skin: " << m_playerskin << "\n";
    printf("settings saved\n");
}
