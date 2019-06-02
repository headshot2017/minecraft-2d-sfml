#ifndef MENU_STATE_H_INCLUDED
#define MENU_STATE_H_INCLUDED

#include "game_state.h"

class MenuState : public GameState
{
public:
    void init();
    void destroy();
    void update(GameEngine* engine);
    void process_input(GameEngine* engine);
    void draw(GameEngine* engine);
    void pause();
    void resume();

    static MenuState* Instance() {return &m_Instance;}

private:
    static MenuState m_Instance;
};

#endif // MENU_STATE_H_INCLUDED
