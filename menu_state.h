#ifndef MENU_STATE_H_INCLUDED
#define MENU_STATE_H_INCLUDED

#include "game_state.h"
#include "gui/button.h"
#include "gui/textinput.h"
#include "gui/label.h"

typedef enum
{
    MENU_MAINMENU,
    MENU_LOADWORLD,
    MENU_MULTIPLAYER,
    MENU_OPTIONS,

    MENU_OPTIONS_GRAPHICS,
    MENU_OPTIONS_CONTROLS,
    MENU_OPTIONS_PLAYER,

    MENU_OPTIONS_CONTROLS_CHANGE
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

    void changeBind(const char *keybind);
    void setAllPositions(GameEngine* engine);

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
    Button b_options_player;
    Button b_options_graphics;
    Button b_options_controls;

    // player options menu
    Label label_playername;
    Label label_playerskin;
    TextInput input_playername;
    TextInput input_playerskin;
    sf::Texture txt_playerskin;

    // video options menu

    // controls options menu
    Button b_moveleft;
    Button b_moveright;
    Button b_jump;
    Button b_sneak;
    Button b_run;
    Button b_layerswap;
    Button b_place;
    Button b_destroy;
    Button b_pick;

    // changing control menu
    Label l_pressakey;
    char m_changingControl[32];


protected:
    MenuState() {}
};

#endif // MENU_STATE_H_INCLUDED
