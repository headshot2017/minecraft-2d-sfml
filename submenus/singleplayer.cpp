#include <dirent.h>

#include "../menu_state.h"
#include "../ingame_state.h"
#include "singleplayer.h"
#include "title_screen.h"

bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool replaceStr(std::string& str, const std::string& from, const std::string& to)
{
    std::size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void getWorlds(std::vector<std::string>& strvec)
{
    DIR *dir;
    struct dirent *entry;
    strvec.clear();

    if ((dir = opendir("worlds")))
    {
        while ((entry = readdir(dir)))
        {
            std::string worldname(entry->d_name);
            if (has_suffix(worldname, std::string(".dat")))
            {
                replaceStr(worldname, ".dat", "");
                strvec.push_back(worldname);
            }
        }
    }
    else
#ifdef _WIN32
        mkdir("worlds");
#else
        mkdir("worlds", 0777);
#endif
}



SingleplayerSubmenu::SingleplayerSubmenu(GameEngine* engine, MenuState *menu) : Submenu(engine, menu)
{
    sf::Vector2u windowsize = engine->app.getSize();

    getWorlds(m_worldlist);

    lbl_selectworld = new Label(engine, "Select World", windowsize.x/2, 48, 1, 1);
    list_worlds = new ItemList(engine, 64, (windowsize.y/4)-32, windowsize.x-128, windowsize.y-200, m_worldlist);
    //b_back.getPos().y - l_worlds.getPosition().y - 64

    btn_playworld = new Button(engine, "Play Selected World", (windowsize.x/2)-300-8, windowsize.y-96, 300);
    btn_createworld = new Button(engine, "Create New World", (windowsize.x/2)+8, windowsize.y-96, 300);
    btn_renameworld = new Button(engine, "Rename", btn_playworld->getPos().x, windowsize.y-48, 144);
    btn_deleteworld = new Button(engine, "Delete", (windowsize.x/2)-144-8, windowsize.y-48, 144);
    btn_recreate_world = new Button(engine, "Re-Create", (windowsize.x/2)+8, windowsize.y-48, 144);
    btn_cancel = new Button(engine, "Cancel", btn_recreate_world->getPos().x+144+12, windowsize.y-48, 144);

    list_worlds->onItemClicked(&onWorldSelected, this);
    btn_playworld->onClicked(&onPlayClicked, this);
    btn_cancel->onClicked(&onCancelClicked, this);
}

SingleplayerSubmenu::~SingleplayerSubmenu()
{

}

void SingleplayerSubmenu::onWorldSelected(int index, void* pUserData)
{

}

void SingleplayerSubmenu::onPlayClicked(void* pUserData)
{
    SingleplayerSubmenu* self = (SingleplayerSubmenu*)pUserData;

    self->m_engine->Sound()->stopMusic();

    IngameState* state = new IngameState(self->m_engine);
    state->loadWorld(self->list_worlds->getSelectedItem().c_str());

    self->m_engine->changeState(state);
}

void SingleplayerSubmenu::onCancelClicked(void* pUserData)
{
    SingleplayerSubmenu* self = (SingleplayerSubmenu*)pUserData;

    TitleScreenSubmenu* newstate = new TitleScreenSubmenu(self->m_engine, self->m_menu);
    self->m_menu->changeSubmenu(newstate);
}

void SingleplayerSubmenu::update(float delta)
{
    list_worlds->update();
    btn_playworld->update();
    btn_createworld->update();
    btn_renameworld->update();
    btn_deleteworld->update();
    btn_recreate_world->update();
    btn_cancel->update();
}

void SingleplayerSubmenu::process_input(sf::Event& event)
{
    list_worlds->event_input(event);
    btn_playworld->process_input(event);
    btn_createworld->process_input(event);
    btn_renameworld->process_input(event);
    btn_deleteworld->process_input(event);
    btn_recreate_world->process_input(event);
    btn_cancel->process_input(event);
}

void SingleplayerSubmenu::draw()
{
    lbl_selectworld->draw();
    list_worlds->draw();
    btn_playworld->draw();
    btn_createworld->draw();
    btn_renameworld->draw();
    btn_deleteworld->draw();
    btn_recreate_world->draw();
    btn_cancel->draw();
}

void SingleplayerSubmenu::onResolutionChange(sf::Vector2u res)
{

}
