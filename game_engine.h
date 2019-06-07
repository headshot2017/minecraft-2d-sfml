#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include "bass.h"


enum
{
    BLOCK_AIR,
    BLOCK_GRASS,
    BLOCK_DIRT,
    BLOCK_STONE,
    BLOCK_BEDROCK
};


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

    void changeState(GameState* state);
    void pushState(GameState* state); // pause the current state and load this new one.
    void popState(); // destroy the current state and resume the paused one.

    bool isRunning() {return m_running;}

    sf::Texture m_blocks;

    sf::RenderWindow app;
    sf::Font mc_font;

    sf::Sprite button;
    sf::Sprite button_hover;

    HSTREAM snd_button_click;

private:
    bool m_running = true;
    std::vector<GameState*> states;
    sf::Texture m_button;
    sf::Texture m_button_hover;
};

#endif // GAME_ENGINE_H_INCLUDED
