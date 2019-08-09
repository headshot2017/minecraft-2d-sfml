#ifndef INGAME_STATE_H_INCLUDED
#define INGAME_STATE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "game_state.h"
#include "game/world.h"
#include "game/player.h"
#include <fstream>

enum
{
    INV_HOTBAR,
    INV_PAGE1=9,
    INV_PAGE2=18,
    INV_PAGE3=27,
    INV_TOTAL=36
};

class IngameState : public GameState
{
public:
    void init(GameEngine* engine);
    void destroy();
    void update(GameEngine* engine);
    void event_input(GameEngine* engine, sf::Event& event);
    void process_input(GameEngine* engine);
    void draw(GameEngine* engine);
    void pause();
    void resume();
    void onResolutionChange(sf::Vector2u res) {}
    void generateWorld(unsigned int seed, const char *name) {m_world->generateWorld(seed, name);}
    void loadWorld(const char *worldName);

    static IngameState* Instance() {return &m_Instance;}

private:
    static IngameState m_Instance;

    World *m_world;
    GameEngine *m_engine;

    sf::RectangleShape m_sky;
    sf::RectangleShape m_blockoutline;
    sf::Text text_cam_pos;

    float cam_x, cam_y;

    // player
    int m_inventory[INV_TOTAL][1] = {0};
    int m_hotbarslot = 0;

protected:
    IngameState() {}
};

#endif // INGAME_STATE_H_INCLUDED
