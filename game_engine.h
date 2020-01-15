#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED

#define MAX_FPS 60
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include "bass.h"
#include "sound.h"
#include "game/block.h"
#include "settings_manager.h"


class GameState;

class GameEngine
{
public:
    void init();
    void cleanup();
    void update();
    void process_input();
    void draw();
    void quit(); // rq

    void leaveGame(int setting) {m_leavegame = setting;}
    int leavingGame() {return m_leavegame;}
    void setPaused(bool paused) {m_paused = paused;}
    bool isPaused() {return m_paused;}

    void changeState(GameState* state);
    void pushState(GameState* state); // pause the current state and load this new one.
    void popState(); // destroy the current state and resume the paused one.

    const sf::Texture& takeScreenshot(bool saveTexture=false);
    void setResolution(sf::Vector2u res, sf::Uint32 flags=sf::Style::Close);
    std::vector<sf::VideoMode>& getResolutions() {return m_videomodes;}

    bool isRunning() {return m_running;}
    SoundEngine* Sound() {return m_sound;}
    SettingsManager* Settings() {return &m_settings;}

    sf::VertexArray getGUIWindow(sf::Vector2f size);

    sf::RenderTexture m_window;
    sf::Texture m_screenshot;

    sf::Texture m_blocks;
    sf::Texture m_explosion;
    sf::Texture m_sun;
    sf::Texture m_moon;

    sf::RenderWindow app;
    sf::Font mc_font;

    sf::Texture m_button;
    sf::Texture m_button_hover;
    sf::Texture m_button_locked;
    sf::Texture m_hotbar;
    sf::Texture m_hotbarselect;

    long int m_ticks = 0;

private:
    bool m_running = true;
    bool m_paused = false;
    int m_leavegame = 0;
    std::vector<sf::VideoMode> m_videomodes;
    std::vector<GameState*> states;
    SoundEngine *m_sound;
    SettingsManager m_settings;
    sf::Clock m_clock;
};

#endif // GAME_ENGINE_H_INCLUDED
