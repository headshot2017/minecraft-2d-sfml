#ifndef MENU_STATE_H_INCLUDED
#define MENU_STATE_H_INCLUDED

#include "game_state.h"
#include "gui/button.h"
#include "gui/slider.h"
#include "gui/textinput.h"
#include "gui/label.h"
#include "gui/itemlist.h"
#include <vector>

typedef enum
{
    MENU_MAINMENU,
    MENU_LOADWORLD,
    MENU_MULTIPLAYER,
    MENU_OPTIONS,

    MENU_CREATEWORLD,
    MENU_RENAMEWORLD,
    MENU_CONFIRMDELETE,
    MENU_SUPERFLAT_SETUP,

    MENU_OPTIONS_GRAPHICS,
    MENU_OPTIONS_CONTROLS,
    MENU_OPTIONS_PLAYER,
    MENU_OPTIONS_SOUND,

    MENU_OPTIONS_CONTROLS_CHANGE
} SUB_MENUS;

class MenuState : public GameState
{
public:
    void init(GameEngine* engine);
    void destroy();
    void update(GameEngine *engine, float delta);
    void event_input(GameEngine* engine, sf::Event& event);
    void process_input(GameEngine* engine) {}
    void draw(GameEngine* engine);
    void pause();
    void resume();
    void onResolutionChange(sf::Vector2u res);

    void changeBind(const char *keybind);
    void setAllPositions(sf::Vector2u& windowsize);
    void setSplashText();

    static MenuState* Instance() {return &m_Instance;}

private:
    static MenuState m_Instance;
    SUB_MENUS m_submenu = MENU_MAINMENU;
    GameEngine *m_engine;

    int m_musicticks;

    sf::Sprite dirt_tile;
    sf::Sprite minecraft_logo;
    sf::VertexArray parallax_bg;

    sf::Texture m_dirt_tile;
    sf::Texture m_minecraft_logo;
    sf::Texture m_parallax_bg;
    float m_parallax_x;
    sf::VertexArray m_gamescreen;

    Label m_splashtext;
    std::vector<std::string> m_splashtexts;
    std::vector<std::string> m_worldlist;

    float m_splashscale;
    float m_splashscaledir;

    Label m_versioninfo;
    Label m_fanmadeproject;

    // general buttons
    Button b_back;
    Button b_back_options;

    // main menu
    Button b_singleplayer;
    Button b_multiplayer;
    Button b_options;
    Button b_quit;

    // singleplayer menu
    Label l_selectworld;
    ItemList l_worlds;
    Button b_playworld;
    Button b_createworld_list;
    Button b_renameworld;
    Button b_deleteworld;
    Button b_recreate_world;
    Button b_cancel_list;

    Label label_confirm_delete;
    Button b_delete_yes;
    Button b_delete_no;

    Label label_renameworld;
    Button b_confirmrename;
    Button b_cancelrename;

    Label label_createworld;
    Label label_worldname;
    TextInput input_worldname;
    Button b_worldbiome;
    Button b_confirmcreate;
    Button b_cancelcreate;
    int selected_biome = -1;
    std::vector<int> superflat_blocks;

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


protected:
    MenuState() {}
};

#endif // MENU_STATE_H_INCLUDED
