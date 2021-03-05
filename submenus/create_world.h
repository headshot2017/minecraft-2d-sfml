#ifndef CREATE_WORLD_H_INCLUDED
#define CREATE_WORLD_H_INCLUDED

/*
    World creation options
*/

#include "submenu.h"
#include "../gui/label.h"
#include "../gui/button.h"
#include "../gui/textinput.h"

class CreateWorldSubmenu : public Submenu
{
public:
    CreateWorldSubmenu(GameEngine* engine, MenuState* menu);
    ~CreateWorldSubmenu();

    void update(float delta);
    void process_input(sf::Event& event);
    void draw();
    void onResolutionChange(sf::Vector2u res);

    static void onWorldBiomeClicked(void *pUserData);
    static void onConfirmClicked(void *pUserData);
    static void onCancelClicked(void *pUserData);

private:
    Label* label_createworld;
    Label* label_worldname;
    TextInput* input_worldname;
    Button* btn_worldbiome;
    Button* btn_confirmcreate;
    Button* btn_cancelcreate;

    std::vector<int> superflat_blocks;
    int selected_biome = -1;
};

#endif // CREATE_WORLD_H_INCLUDED
