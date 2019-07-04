#ifndef PAUSED_STATE_H_INCLUDED
#define PAUSED_STATE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "game_state.h"
#include "gui/button.h"

class PausedState : public GameState
{
public:
    void init(GameEngine* engine);
    void destroy();
    void update(GameEngine* engine);
    void event_input(GameEngine* engine, sf::Event& event);
    void process_input(GameEngine* engine) {}
    void draw(GameEngine* engine);
    void pause();
    void resume();
    void onResolutionChange(sf::Vector2u res);

    static PausedState* Instance() {return &m_Instance;}

private:
    static PausedState m_Instance;
    GameEngine* m_engine;

    sf::VertexArray m_gamescreen;
    Button b_resume;
    Button b_options;
    Button b_quit;

protected:
    PausedState() {}
};

#endif // PAUSED_STATE_H_INCLUDED
