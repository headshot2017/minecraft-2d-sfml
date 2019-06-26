#ifndef MENU_STATE_H_INCLUDED
#define MENU_STATE_H_INCLUDED

#include "game_state.h"
#include "gui/button.h"
#include "gui/textinput.h"

typedef enum
{
    MENU_MAINMENU,
    MENU_LOADWORLD,
    MENU_MULTIPLAYER,
    MENU_OPTIONS,

    MENU_OPTIONS_GRAPHICS,
    MENU_OPTIONS_CONTROLS
} SUB_MENUS;

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
    SUB_MENUS m_submenu = MENU_MAINMENU;

    sf::Sprite dirt_tile;
    sf::Sprite minecraft_logo;

    sf::Texture m_dirt_tile;
    sf::Texture m_minecraft_logo;

    // general buttons
    Button b_back;
    Button b_back_options;

    // main menu
    Button b_singleplayer;
    Button b_multiplayer;
    Button b_options;
    Button b_quit;

    // singleplayer menu
    Button b_world1;
    Button b_world2;
    Button b_world3;
    Button b_world4;
    Button b_world5;

    // multiplayer menu
    Button b_connect;

    // options menu
    Button b_options_graphics;
    Button b_options_controls;

    // video options menu
    Button b_layerlighting;

    // controls options menu


protected:
    MenuState() {}
};

#endif // MENU_STATE_H_INCLUDED
