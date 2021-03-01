#ifndef _TITLE_SCREEN_H
#define _TITLE_SCREEN_H

/*
    The title screen with the game logo and parallax background.
*/

#include "submenu.h"
#include "../gui/label.h"
#include "../gui/button.h"

class TitleScreenSubmenu : public Submenu
{
public:
    TitleScreenSubmenu(GameEngine *engine, MenuState *menu);
    ~TitleScreenSubmenu();

    void update(float delta);
    void process_input(sf::Event& event);
    void draw();
    void onResolutionChange(sf::Vector2u res);

    static void onSinglePlayerClicked(void *pUserData);
    //static void onMultiPlayerClicked(void *pUserData);
    //static void onOptionsClicked(void *pUserData);
    static void onQuitClicked(void *pUserData);

private:
    sf::Texture m_minecraft_logo;
    sf::Texture m_parallax_bg;
    float m_parallax_x;
    sf::Sprite spr_minecraft_logo;
    sf::VertexArray vtx_parallax_bg;

    Label* m_splashtext;
    std::vector<std::string> m_splashtexts;
    float m_splashscale;
    float m_splashscaledir;

    Label* m_versioninfo;
    Label* m_fanmadeproject;

    Button* btn_singleplayer;
    Button* btn_multiplayer;
    Button* btn_options;
    Button* btn_quit;

    void setSplashText();
};

#endif // _TITLE_SCREEN_H
