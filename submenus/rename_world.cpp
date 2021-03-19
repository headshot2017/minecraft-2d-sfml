#include "../menu_state.h"
#include "rename_world.h"
#include "singleplayer.h"

RenameWorldSubmenu::RenameWorldSubmenu(GameEngine* engine, MenuState* menu, std::string original_name)
 : Submenu(engine, menu)
{
    m_originalname = original_name;
    sf::Vector2u windowsize = engine->app.getSize();

    label_renameworld = new Label(engine, "Rename World", windowsize.x/2, 48, 1, 1);
    input_worldname = new TextInput(engine, original_name, sf::Vector2f((windowsize.x/2)-200, 96+24), 96);
    btn_cancel = new Button(engine, "Cancel", windowsize.x/2-200, windowsize.y-40-32);
    btn_confirm = new Button(engine, "Rename", windowsize.x/2-200, btn_cancel->getPos().y-48);

    btn_cancel->onClicked(&onCancelClicked, this);
    btn_confirm->onClicked(&onConfirmClicked, this);
}

RenameWorldSubmenu::~RenameWorldSubmenu()
{
    delete label_renameworld;
    delete input_worldname;
    delete btn_cancel;
    delete btn_confirm;
}

void RenameWorldSubmenu::onConfirmClicked(void *pUserData)
{
    RenameWorldSubmenu* self = (RenameWorldSubmenu*)pUserData;

    // rename world file
    char oldname[256], newname[256];
    sprintf(oldname, "worlds/%s.dat", self->m_originalname.c_str());
    sprintf(newname, "worlds/%s.dat", self->input_worldname->getString().toAnsiString().c_str());
    rename(oldname, newname);

    SingleplayerSubmenu* newstate = new SingleplayerSubmenu(self->m_engine, self->m_menu);
    self->m_menu->changeSubmenu(newstate);
}

void RenameWorldSubmenu::onCancelClicked(void *pUserData)
{
    RenameWorldSubmenu* self = (RenameWorldSubmenu*)pUserData;

    SingleplayerSubmenu* newstate = new SingleplayerSubmenu(self->m_engine, self->m_menu);
    self->m_menu->changeSubmenu(newstate);
}

void RenameWorldSubmenu::update(float delta)
{
    input_worldname->update();
    btn_confirm->update();
    btn_cancel->update();
}

void RenameWorldSubmenu::process_input(sf::Event& event)
{
    input_worldname->process_input(event);
    btn_confirm->process_input(event);
    btn_cancel->process_input(event);
}

void RenameWorldSubmenu::draw()
{
    label_renameworld->draw();
    input_worldname->draw();
    btn_confirm->draw();
    btn_cancel->draw();
}

void RenameWorldSubmenu::onResolutionChange(sf::Vector2u res)
{

}
