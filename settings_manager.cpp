#include "settings_manager.h"
#include <fstream>
#include <algorithm>
#include <cstring>

SettingsManager::SettingsManager()
{
    m_fullscreen = false;
    m_screenwidth = 800;
    m_screenheight = 480;
    m_sounddevice = -1;
    m_mastervol = 100;
    m_musicvol = 100;
    m_soundvol = 100;
}

void SettingsManager::generateSettings()
{
    sprintf(m_playername, "Alexn't");
    sprintf(m_playerskin, "steve");
    sprintf(m_soundtheme, "default");
    m_fullscreen = false;
    m_screenwidth = 800;
    m_screenheight = 480;
    m_sounddevice = -1;
    m_mastervol = 100;
    m_musicvol = 100;
    m_soundvol = 100;
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
            std::size_t delimiterPos = line.find(":");
            std::string name = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            int maxkeys = static_cast<int>(sf::Keyboard::KeyCount);
            if (strcmp(name.c_str(), "player_name") == 0)
                sprintf(m_playername, value.c_str());
            else if (strcmp(name.c_str(), "player_skin") == 0)
                sprintf(m_playerskin, value.c_str());
            else if (strcmp(name.c_str(), "fullscreen") == 0)
                m_fullscreen = std::atoi(value.c_str());
            else if (strcmp(name.c_str(), "screen_width") == 0)
                m_screenwidth = std::atoi(value.c_str());
            else if (strcmp(name.c_str(), "screen_height") == 0)
                m_screenheight = std::atoi(value.c_str());
            else if (strcmp(name.c_str(), "master_volume") == 0)
                m_mastervol = std::atoi(value.c_str());
            else if (strcmp(name.c_str(), "music_volume") == 0)
                m_musicvol = std::atoi(value.c_str());
            else if (strcmp(name.c_str(), "sound_volume") == 0)
                m_soundvol = std::atoi(value.c_str());
            else if (strcmp(name.c_str(), "sound_device") == 0)
                m_sounddevice = std::atoi(value.c_str());
            else if (strcmp(name.c_str(), "sound_theme") == 0)
                sprintf(m_soundtheme, value.c_str());

            else if (strcmp(name.c_str(), "control_destroy") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("destroy", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("destroy", static_cast<sf::Keyboard::Key>(key));
            }
            else if (strcmp(name.c_str(), "control_place") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("place", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("place", static_cast<sf::Keyboard::Key>(key));
            }
            else if (strcmp(name.c_str(), "control_pick") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("pick", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("pick", static_cast<sf::Keyboard::Key>(key));
            }
            else if (strcmp(name.c_str(), "control_drop") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("drop", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("drop", static_cast<sf::Keyboard::Key>(key));
            }
            else if (strcmp(name.c_str(), "control_left") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("left", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("left", static_cast<sf::Keyboard::Key>(key));
            }
            else if (strcmp(name.c_str(), "control_right") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("right", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("right", static_cast<sf::Keyboard::Key>(key));
            }
            else if (strcmp(name.c_str(), "control_jump") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("jump", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("jump", static_cast<sf::Keyboard::Key>(key));
            }
            else if (strcmp(name.c_str(), "control_sneak") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("sneak", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("sneak", static_cast<sf::Keyboard::Key>(key));
            }
            else if (strcmp(name.c_str(), "control_run") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("run", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("run", static_cast<sf::Keyboard::Key>(key));
            }
            else if (strcmp(name.c_str(), "control_inventory") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("inventory", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("inventory", static_cast<sf::Keyboard::Key>(key));
            }
            else if (strcmp(name.c_str(), "control_layerswap") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("layerswap", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("layerswap", static_cast<sf::Keyboard::Key>(key));
            }
            else if (strcmp(name.c_str(), "control_layer1_collide") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("layer1_collide", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("layer1_collide", static_cast<sf::Keyboard::Key>(key));
            }
            else if (strcmp(name.c_str(), "control_screenshot") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("screenshot", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("screenshot", static_cast<sf::Keyboard::Key>(key));
            }
            else if (strcmp(name.c_str(), "control_fullscreen") == 0)
            {
                int key = std::atoi(value.c_str());
                if (key >= maxkeys)
                    m_controls.setMouseBind("fullscreen", static_cast<sf::Mouse::Button>(key - maxkeys));
                else
                    m_controls.setKeyBind("fullscreen", static_cast<sf::Keyboard::Key>(key));
            }

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
    configfile << "fullscreen: " << ((m_fullscreen) ? "1" : "0") << "\n";
    configfile << "screen_width: " << m_screenwidth << "\n";
    configfile << "screen_height: " << m_screenheight << "\n";
    configfile << "\n";
    configfile << "sound_device: " << m_sounddevice << "\n";
    configfile << "sound_theme: " << m_soundtheme << "\n";
    configfile << "master_volume: " << m_mastervol << "\n";
    configfile << "music_volume: " << m_musicvol << "\n";
    configfile << "sound_volume: " << m_soundvol << "\n";
    configfile << "\n";

    keysMap keys = m_controls.getKeys();
    int maxkeys = static_cast<int>(sf::Keyboard::KeyCount);

    configfile << "control_destroy: " << (keys["destroy"].inputType == MouseInput ? (static_cast<int>(keys["destroy"].mouseButton)) + maxkeys : static_cast<int>(keys["destroy"].key)) << "\n";
    configfile << "control_place: " << (keys["place"].inputType == MouseInput ? (static_cast<int>(keys["place"].mouseButton)) + maxkeys : static_cast<int>(keys["place"].key)) << "\n";
    configfile << "control_pick: " << (keys["pick"].inputType == MouseInput ? (static_cast<int>(keys["pick"].mouseButton)) + maxkeys : static_cast<int>(keys["pick"].key)) << "\n";
    configfile << "control_drop: " << (keys["drop"].inputType == MouseInput ? (static_cast<int>(keys["drop"].mouseButton)) + maxkeys : static_cast<int>(keys["drop"].key)) << "\n";
    configfile << "control_left: " << (keys["left"].inputType == MouseInput ? (static_cast<int>(keys["left"].mouseButton)) + maxkeys : static_cast<int>(keys["left"].key)) << "\n";
    configfile << "control_right: " << (keys["right"].inputType == MouseInput ? (static_cast<int>(keys["right"].mouseButton)) + maxkeys : static_cast<int>(keys["right"].key)) << "\n";
    configfile << "control_jump: " << (keys["jump"].inputType == MouseInput ? (static_cast<int>(keys["jump"].mouseButton)) + maxkeys : static_cast<int>(keys["jump"].key)) << "\n";
    configfile << "control_sneak: " << (keys["sneak"].inputType == MouseInput ? (static_cast<int>(keys["sneak"].mouseButton)) + maxkeys : static_cast<int>(keys["sneak"].key)) << "\n";
    configfile << "control_run: " << (keys["run"].inputType == MouseInput ? (static_cast<int>(keys["run"].mouseButton)) + maxkeys : static_cast<int>(keys["run"].key)) << "\n";
    configfile << "control_inventory: " << (keys["inventory"].inputType == MouseInput ? (static_cast<int>(keys["inventory"].mouseButton)) + maxkeys : static_cast<int>(keys["inventory"].key)) << "\n";
    configfile << "control_layerswap: " << (keys["layerswap"].inputType == MouseInput ? (static_cast<int>(keys["layerswap"].mouseButton)) + maxkeys : static_cast<int>(keys["layerswap"].key)) << "\n";
    configfile << "control_layer1_collide: " << (keys["layer1_collide"].inputType == MouseInput ? (static_cast<int>(keys["layerswap"].mouseButton)) + maxkeys : static_cast<int>(keys["layer1_collide"].key)) << "\n";
    configfile << "control_screenshot: " << (keys["screenshot"].inputType == MouseInput ? (static_cast<int>(keys["screenshot"].mouseButton)) + maxkeys : static_cast<int>(keys["screenshot"].key)) << "\n";
    configfile << "control_fullscreen: " << (keys["fullscreen"].inputType == MouseInput ? (static_cast<int>(keys["fullscreen"].mouseButton)) + maxkeys : static_cast<int>(keys["fullscreen"].key)) << "\n";

    configfile.close();

    printf("settings saved\n");
}
