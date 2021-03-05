#ifndef DELETE_WORLD_H_INCLUDED
#define DELETE_WORLD_H_INCLUDED

/*
    Confirmation to delete a world
*/

#include "submenu.h"
#include "../gui/label.h"
#include "../gui/button.h"

class DeleteWorldSubmenu : public Submenu
{
public:
    DeleteWorldSubmenu(GameEngine* engine, MenuState* menu, std::string worldname);
    ~DeleteWorldSubmenu();

    void update(float delta);
    void process_input(sf::Event& event);
    void draw();
    void onResolutionChange(sf::Vector2u res);

    static void onYes(void* pUserData);
    static void onNo(void* pUserData);

private:
    Label* label_confirm_delete;
    Button* btn_delete_yes;
    Button* btn_delete_no;
    std::string m_worldname;
};

#endif // DELETE_WORLD_H_INCLUDED
