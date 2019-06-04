#include "game_engine.h"
#include "game_state.h"
#include <SFML/Graphics.hpp>
#include "bass.h"
#include <stdio.h>

void GameEngine::init()
{
    app.create(sf::VideoMode(800, 480), "minecrap clone test");
    app.setFramerateLimit(60);

    mc_font.loadFromFile("data/font/Minecraftia.ttf");

    if (not BASS_Init(-1, 44100, 0, 0, 0))
        printf("bass could not start. error code: %d\n", BASS_ErrorGetCode());

    sf::Image widgets;
    widgets.loadFromFile("data/gui/widgets.png");

    m_button.loadFromImage(widgets, sf::IntRect(0, 66, 200, 20));
    m_button_hover.loadFromImage(widgets, sf::IntRect(0, 86, 200, 20));
    button.setTexture(m_button);
    button_hover.setTexture(m_button_hover);
    button.setScale(2.0f, 2.0f);
    button_hover.setScale(2.0f, 2.0f);

    snd_button_click = BASS_StreamCreateFile(false, "data/sounds/click.wav", 0, 0, 0);
}

void GameEngine::cleanup()
{
    while (not states.empty())
    {
        states.back()->destroy();
        states.pop_back();
    }
    BASS_StreamFree(snd_button_click);
    BASS_Free();
}

void GameEngine::update()
{
    states.back()->update(this);
}

void GameEngine::process_input()
{
    sf::Event event;
    while (app.pollEvent(event))
    {
        // Close window : exit
        if (event.type == sf::Event::Closed)
            quit();
    }

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

void GameEngine::loadWorld(sf::String worldName)
{

}
