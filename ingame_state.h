#ifndef INGAME_STATE_H_INCLUDED
#define INGAME_STATE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "game_state.h"
#include "game/world.h"
#include "game/player.h"
#include "ingame_gui.h"
#include <fstream>
#include <vector>

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
    void onResolutionChange(sf::Vector2u res);
    void generateFlatWorld(const char *name, const std::vector<int>& blocks) {m_world->generateFlatWorld(name, blocks);}
    void generateWorld(unsigned int seed, const char *name, int biome=-1) {m_world->generateWorld(seed, name, biome);}
    void loadWorld(const char *worldName, bool force_generate=false, int biome=-1, const std::vector<int>& blocks=std::vector<int>());

    void setHotbarSlot(int slot);
    void randomizeStars();

    static IngameState* Instance() {return &m_Instance;}

private:
    static IngameState m_Instance;

    World *m_world;
    GameEngine *m_engine;
    GameGUI *m_gamegui;

    int m_musicticks;
    int m_flytick;
    int m_gamemode;

    int m_skytime;
    sf::RectangleShape m_daysky;
    sf::RectangleShape m_nightsky;
    sf::RectangleShape m_blockoutline;
    sf::Texture m_nightstars;
    int m_starflip, m_starflipvert;
    sf::Uint8 m_staralpha;
    unsigned m_star_x;
    sf::Text text_cam_pos;

    float cam_x, cam_y;

    // player
    int m_inventory[INV_TOTAL][2];
    int m_hotbarslot = 0;
    bool lmb = false, rmb = false, mmb = false; // mouse buttons
    bool m_freecam;
    bool m_showgui;
    bool m_showdebug;
    int m_freecam_add = 32;
    int lmb_tick = 0;

protected:
    IngameState() {}
};

#endif // INGAME_STATE_H_INCLUDED
