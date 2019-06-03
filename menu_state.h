#ifndef MENU_STATE_H_INCLUDED
#define MENU_STATE_H_INCLUDED

#include "game_state.h"
#include "gui/button.h"

typedef enum
{
    MENU_MAINMENU,
    MENU_LOADWORLD,
    MENU_MULTIPLAYER,
    MENU_OPTIONS
} MENU_STATES;

class MenuState : public GameState
{
public:
    void init(GameEngine* engine);
    void destroy();
    void update(GameEngine* engine);
    void process_input(GameEngine* engine);
    void draw(GameEngine* engine);
    void pause();
    void resume();

    static MenuState* Instance() {return &m_Instance;}

private:
    static MenuState m_Instance;
    MENU_STATES m_menustate = MENU_MAINMENU;

    sf::Sprite dirt_tile;
    sf::Sprite minecraft_logo;

    sf::Texture m_dirt_tile;
    sf::Texture m_minecraft_logo;

    Button b_singleplayer;
    Button b_multiplayer;
    Button b_options;
    Button b_quit;

protected:
    MenuState() {}
};

#endif // MENU_STATE_H_INCLUDED
