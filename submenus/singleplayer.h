#ifndef SINGLEPLAYER_H_INCLUDED
#define SINGLEPLAYER_H_INCLUDED

/*
    List of local worlds to play on singleplayer
*/

#include "submenu.h"
#include "../gui/label.h"
#include "../gui/button.h"
#include "../gui/itemlist.h"

class SingleplayerSubmenu : public Submenu
{
public:
    SingleplayerSubmenu(GameEngine *engine, MenuState *menu);
    ~SingleplayerSubmenu();

    void update(float delta);
    void process_input(sf::Event& event);
    void draw();
    void onResolutionChange(sf::Vector2u res);

    static void onWorldSelected(int index, void *pUserData);
    static void onPlayClicked(void *pUserData);
    static void onCreateWorldClicked(void *pUserData);
    static void onRenameWorldClicked(void *pUserData);
    static void onDeleteWorldClicked(void *pUserData);
    //static void onRecreateWorldClicked(void *pUserData);
    static void onCancelClicked(void *pUserData);

private:
    std::vector<std::string> m_worldlist;

    Label* lbl_selectworld;
    ItemList* list_worlds;
    Button* btn_playworld;
    Button* btn_createworld;
    Button* btn_renameworld;
    Button* btn_deleteworld;
    Button* btn_recreate_world;
    Button* btn_cancel;
};

#endif // SINGLEPLAYER_H_INCLUDED
