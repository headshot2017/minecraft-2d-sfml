#ifndef MENU_STATE_H_INCLUDED
#define MENU_STATE_H_INCLUDED

#include "game_engine.h"
#include "game_state.h"
#include "submenus/submenu.h"

#include "gui/button.h"
#include "gui/slider.h"
#include "gui/textinput.h"
#include "gui/label.h"
#include "gui/itemlist.h"

#include <vector>
#include <string>

class MenuState : public GameState
{
public:
    MenuState(GameEngine* engine);
    ~MenuState();
    void update(float delta);
    void event_input(sf::Event& event);
    void process_input();
    void draw();
    void pause();
    void resume();
    void onResolutionChange(sf::Vector2u res);

    void changeSubmenu(Submenu* newMenu);

    void changeBind(const char *keybind);
    //void setAllPositions(sf::Vector2u& windowsize);

private:
    Submenu* m_submenu;
    GameEngine *m_engine;

    int m_musicticks;

    sf::Sprite dirt_tile;
    sf::Texture m_dirt_tile;
    sf::VertexArray m_gamescreen;

    // general buttons
    Button b_back;
    Button b_back_options;

    // singleplayer menu







    // multiplayer menu
    Button b_connect;
    ItemList l_test;

    // options menu
    Button b_options_player;
    Button b_options_graphics;
    Button b_options_controls;
    Button b_options_sound;

    // player options menu
    Label label_playername;
    Label label_playerskin;
    TextInput input_playername;
    TextInput input_playerskin;
    sf::Texture txt_playerskin;

    // sound options menu
    ItemList l_devices;
    Slider s_mastervol;
    Slider s_musicvol;
    Slider s_soundvol;
    ItemList l_soundthemes;
    std::vector<std::string> m_soundthemes;
    std::vector<std::string> m_devicelist;

    // video options menu
    bool fullscreen;
    Button b_applyvideo;
    Button b_fullscreen;
    Slider s_videores;

    // controls options menu
    Button b_moveleft;
    Button b_moveright;
    Button b_jump;
    Button b_sneak;
    Button b_run;
    Button b_inventory;
    Button b_layerswap;
    Button b_layer1_collide;
    Button b_place;
    Button b_destroy;
    Button b_pick;
    Button b_drop;
    Button b_screenshot;
    Button b_fullscreen_control;

    // changing control menu
    Label l_pressakey;
    char m_changingControl[32];
};

#endif // MENU_STATE_H_INCLUDED
