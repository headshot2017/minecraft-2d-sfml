#include "game_engine.h"
#include "game_state.h"
#include <SFML/Graphics.hpp>
#include "bass.h"
#include <stdio.h>

void GameEngine::init()
{
    app.create(sf::VideoMode(640, 480), "minecrap clone test");
    app.setFramerateLimit(60);
    if (not BASS_Init(-1, 44100, 0, 0, 0))
        printf("bass could not start. error code: %d\n", BASS_ErrorGetCode());
}

void GameEngine::cleanup()
{
    while (not states.empty())
    {
        states.back()->destroy();
        states.pop_back();
    }
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
    states.back()->init();
}

void GameEngine::pushState(GameState* state)
{
    if (states.empty())
        return;

    states.back()->pause();
    states.push_back(state);
    states.back()->init();
}

void GameEngine::popState()
{
    if (states.empty())
        return;

    states.back()->destroy();
    states.pop_back();
    states.back()->resume();
}
