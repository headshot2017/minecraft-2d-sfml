#ifndef PAUSED_STATE_H_INCLUDED
#define PAUSED_STATE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "game_state.h"
#include "gui/button.h"

class PausedState : public GameState
{
public:
    PausedState(GameEngine* engine);
    ~PausedState();

    void update(float delta);
    void event_input(sf::Event& event);
    void process_input() {}
    void draw();
    void pause();
    void resume();
    void onResolutionChange(sf::Vector2u res);

private:
    GameEngine* m_engine;

    sf::VertexArray m_gamescreen;
    Button b_resume;
    Button b_options;
    Button b_quit;
};

#endif // PAUSED_STATE_H_INCLUDED
