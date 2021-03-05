#include "delete_world.h"
#include "singleplayer.h"
#include "../menu_state.h"

DeleteWorldSubmenu::DeleteWorldSubmenu(GameEngine* engine, MenuState* menu, std::string worldname)
 : Submenu(engine, menu)
{
    m_worldname = worldname;
    sf::Vector2u windowsize = engine->app.getSize();

    char aBuf[256];
    sprintf(aBuf, "Are you sure you want to delete this world?\n\n'%s' will be lost forever! (A long time!)", m_worldname.c_str());

    label_confirm_delete = new Label(engine, aBuf, windowsize.x/2, windowsize.y/2-96, 1);
    btn_delete_yes = new Button(engine, "Delete", (windowsize.x/2)-300-8, windowsize.y/2+16, 300);
    btn_delete_no = new Button(engine, "Cancel", (windowsize.x/2)+8, windowsize.y/2+16, 300);

    btn_delete_yes->onClicked(&onYes, this);
    btn_delete_no->onClicked(&onNo, this);
}

DeleteWorldSubmenu::~DeleteWorldSubmenu()
{
    delete label_confirm_delete;
    delete btn_delete_yes;
    delete btn_delete_no;
}

void DeleteWorldSubmenu::onYes(void* pUserData)
{
    DeleteWorldSubmenu* self = (DeleteWorldSubmenu*)pUserData;

    // delete the world
    char aBuf[256];
    sprintf(aBuf, "worlds/%s.dat", self->m_worldname.c_str());
    remove(aBuf);

    SingleplayerSubmenu* newstate = new SingleplayerSubmenu(self->m_engine, self->m_menu);
    self->m_menu->changeSubmenu(newstate);
}

void DeleteWorldSubmenu::onNo(void* pUserData)
{
    DeleteWorldSubmenu* self = (DeleteWorldSubmenu*)pUserData;

    SingleplayerSubmenu* newstate = new SingleplayerSubmenu(self->m_engine, self->m_menu);
    self->m_menu->changeSubmenu(newstate);
}

void DeleteWorldSubmenu::update(float delta)
{
    btn_delete_yes->update();
    btn_delete_no->update();
}

void DeleteWorldSubmenu::process_input(sf::Event& event)
{
    btn_delete_yes->process_input(event);
    btn_delete_no->process_input(event);
}

void DeleteWorldSubmenu::draw()
{
    label_confirm_delete->draw();
    btn_delete_yes->draw();
    btn_delete_no->draw();
}

void DeleteWorldSubmenu::onResolutionChange(sf::Vector2u res)
{

}
