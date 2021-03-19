#ifndef RENAME_WORLD_H_INCLUDED
#define RENAME_WORLD_H_INCLUDED

/*
    Rename a world
*/

#include "submenu.h"
#include "../gui/label.h"
#include "../gui/button.h"
#include "../gui/textinput.h"

class RenameWorldSubmenu : public Submenu
{
public:
    RenameWorldSubmenu(GameEngine* engine, MenuState* menu, std::string original_name);
    ~RenameWorldSubmenu();

    void update(float delta);
    void process_input(sf::Event& event);
    void draw();
    void onResolutionChange(sf::Vector2u res);

    static void onConfirmClicked(void *pUserData);
    static void onCancelClicked(void *pUserData);

private:
    Label* label_renameworld;
    TextInput* input_worldname;
    Button* btn_confirm;
    Button* btn_cancel;

    std::string m_originalname;
};

#endif // RENAME_WORLD_H_INCLUDED
