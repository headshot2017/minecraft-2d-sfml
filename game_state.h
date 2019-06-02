#ifndef GAME_STATE_H_INCLUDED
#define GAME_STATE_H_INCLUDED

#include "game_engine.h"

class GameState
{
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual void update(GameEngine* engine) = 0;
    virtual void process_input(GameEngine* engine) = 0;
    virtual void draw(GameEngine* engine) = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;

    void changeState(GameEngine* engine, GameState* state)
    {
        engine->changeState(state);
    }

protected:
    GameState() {}
};

#endif // GAME_STATE_H_INCLUDED