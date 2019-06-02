#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

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

    sf::RenderWindow app;

private:
    bool m_running = true;
    std::vector<GameState*> states;
};

#endif // GAME_ENGINE_H_INCLUDED
