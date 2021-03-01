#ifndef GAME_STATE_H_INCLUDED
#define GAME_STATE_H_INCLUDED

#include <SFML/Graphics.hpp>

class GameEngine;

class GameState
{
public:
    GameState(GameEngine* engine);
    virtual ~GameState() {};

    virtual void update(float delta) = 0;
    virtual void event_input(sf::Event& event) = 0;
    virtual void process_input() = 0;
    virtual void draw() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void onResolutionChange(sf::Vector2u res) = 0;

protected:
    GameEngine* m_engine;
};

#endif // GAME_STATE_H_INCLUDED
