#ifndef INGAME_STATE_H_INCLUDED
#define INGAME_STATE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "game_state.h"
#include "game/world.h"
#include "game/player.h"
#include <fstream>

class IngameState : public GameState
{
public:
    void init(GameEngine* engine);
    void destroy();
    void update(GameEngine* engine);
    void process_input(GameEngine* engine);
    void draw(GameEngine* engine);
    void pause();
    void resume();
    void generateWorld(unsigned int seed, const char *name) {m_world.generateWorld(seed, name);}
    void loadWorld(const char *worldName);

    static IngameState* Instance() {return &m_Instance;}

private:
    static IngameState m_Instance;

    World m_world;
    Player m_player; // will later be changed to pointers for multiplayer
    GameEngine *m_engine;

    sf::RectangleShape m_sky;
    sf::RectangleShape m_blockoutline;
    sf::Text text_cam_pos;

    float cam_x, cam_y;

protected:
    IngameState() {}
};

#endif // INGAME_STATE_H_INCLUDED
