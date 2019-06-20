#include "game_engine.h"
#include "game_state.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <string.h>
#include <fstream>

void GameEngine::init()
{
    app.create(sf::VideoMode(800, 480), "minecrap clone test");
    app.setFramerateLimit(60);

    m_blocks.loadFromFile("data/blocks.png");

    mc_font.loadFromFile("data/font/Minecraftia.ttf");

    m_sound = new SoundEngine;
    m_sound->init();
    m_sound->loadTheme("default");

    sf::Image widgets;
    widgets.loadFromFile("data/gui/widgets.png");

    m_button.loadFromImage(widgets, sf::IntRect(0, 66, 200, 20));
    m_button_hover.loadFromImage(widgets, sf::IntRect(0, 86, 200, 20));
    button.setTexture(m_button);
    button_hover.setTexture(m_button_hover);
    button.setScale(2.0f, 2.0f);
    button_hover.setScale(2.0f, 2.0f);
}

void GameEngine::cleanup()
{
    while (not states.empty())
    {
        states.back()->destroy();
        states.pop_back();
    }
    m_sound->cleanup();
    delete m_sound;
}

void GameEngine::update()
{
    states.back()->update(this);
}

void GameEngine::process_input()
{
    states.back()->process_input(this);
}

void GameEngine::draw()
{
    app.clear();
    states.back()->draw(this);
    app.display();
}

void GameEngine::quit()
{
    app.close();
    m_running = false;
}

void GameEngine::changeState(GameState* state)
{
    if (not states.empty())
    {
        states.back()->destroy();
        states.pop_back();
    }
    states.push_back(state);
    states.back()->init(this);
}

void GameEngine::pushState(GameState* state)
{
    if (states.empty())
        return;

    states.back()->pause();
    states.push_back(state);
    states.back()->init(this);
}

void GameEngine::popState()
{
    if (states.empty())
        return;

    states.back()->destroy();
    states.pop_back();
    states.back()->resume();
}
