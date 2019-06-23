#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include "bass.h"
#include "sound.h"
#include "game/block.h"


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
    void leaveGame(int setting=1) {m_leavegame = setting;}
    bool leavingGame() {return m_leavegame;}

    void changeState(GameState* state);
    void pushState(GameState* state); // pause the current state and load this new one.
    void popState(); // destroy the current state and resume the paused one.

    const sf::Texture& takeScreenshot(bool saveTexture=false);

    bool isRunning() {return m_running;}
    SoundEngine* Sound() {return m_sound;}

    sf::RenderTexture m_window;
    sf::Texture m_screenshot;

    sf::Texture m_blocks;

    sf::RenderWindow app;
    sf::Font mc_font;

    sf::Sprite button;
    sf::Sprite button_hover;

    long int m_ticks = 0;

private:
    bool m_running = true;
    int m_leavegame = 0;
    std::vector<GameState*> states;
    sf::Texture m_button;
    sf::Texture m_button_hover;
    SoundEngine *m_sound;
};

#endif // GAME_ENGINE_H_INCLUDED
