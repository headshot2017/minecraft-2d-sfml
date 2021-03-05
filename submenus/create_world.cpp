#include "../menu_state.h"
#include "../ingame_state.h"
#include "create_world.h"
#include "singleplayer.h"

std::string biomeStrs[] = {
    "Random",
    "Plains",
    "Plains with trees",
    "Desert",
    "Mixed",
    "Superflat"
};

CreateWorldSubmenu::CreateWorldSubmenu(GameEngine* engine, MenuState* menu) : Submenu(engine, menu)
{
    selected_biome = -1;

    sf::Vector2u windowsize = engine->app.getSize();

    label_createworld = new Label(engine, "Create New World", windowsize.x/2, 48, 1, 1);
    label_worldname = new Label(engine, "World name", (windowsize.x/2)-200, 96);
    input_worldname = new TextInput(engine, "New World", sf::Vector2f((windowsize.x/2)-200, 96+24), 96);
    btn_worldbiome = new Button(engine, "Biome: " + biomeStrs[selected_biome+1], (windowsize.x/2)-200, 96+88);
    btn_confirmcreate = new Button(engine, "Create New World", (windowsize.x/2)-300-8, windowsize.y-48, 300);
    btn_cancelcreate = new Button(engine, "Cancel", (windowsize.x/2)+8, windowsize.y-48, 300);

    btn_worldbiome->onClicked(&onWorldBiomeClicked, this);
    btn_confirmcreate->onClicked(&onConfirmClicked, this);
    btn_cancelcreate->onClicked(&onCancelClicked, this);

    superflat_blocks.clear();
    for(int i=0; i<2; i++) superflat_blocks.push_back(BLOCK_BEDROCK);
    for(int i=0; i<5; i++) superflat_blocks.push_back(BLOCK_STONE);
    for(int i=0; i<3; i++) superflat_blocks.push_back(BLOCK_DIRT);
    superflat_blocks.push_back(BLOCK_GRASS);
}

CreateWorldSubmenu::~CreateWorldSubmenu()
{
    delete label_createworld;
    delete label_worldname;
    delete input_worldname;
    delete btn_worldbiome;
    delete btn_confirmcreate;
    delete btn_cancelcreate;
}

void CreateWorldSubmenu::onWorldBiomeClicked(void* pUserData)
{
    CreateWorldSubmenu* self = (CreateWorldSubmenu*)pUserData;

    self->selected_biome++;
    if (self->selected_biome > 4) self->selected_biome = -1;
    self->btn_worldbiome->setText("Biome: " + biomeStrs[self->selected_biome+1]);
}

void CreateWorldSubmenu::onConfirmClicked(void* pUserData)
{
    CreateWorldSubmenu* self = (CreateWorldSubmenu*)pUserData;

    self->m_engine->Sound()->stopMusic();
    IngameState* state = new IngameState(self->m_engine);
    state->loadWorld(self->input_worldname->getString().toAnsiString().c_str(), true, self->selected_biome, self->superflat_blocks);
    self->m_engine->changeState(state);
}

void CreateWorldSubmenu::onCancelClicked(void* pUserData)
{
    CreateWorldSubmenu* self = (CreateWorldSubmenu*)pUserData;

    SingleplayerSubmenu *newstate = new SingleplayerSubmenu(self->m_engine, self->m_menu);
    self->m_menu->changeSubmenu(newstate);
}

void CreateWorldSubmenu::update(float delta)
{
    input_worldname->update();
    btn_worldbiome->update();
    btn_confirmcreate->update();
    btn_cancelcreate->update();
}

void CreateWorldSubmenu::process_input(sf::Event& event)
{
    input_worldname->process_input(event);
    btn_worldbiome->process_input(event);
    btn_confirmcreate->process_input(event);
    btn_cancelcreate->process_input(event);
}

void CreateWorldSubmenu::draw()
{
    label_createworld->draw();
    label_worldname->draw();
    input_worldname->draw();
    btn_worldbiome->draw();
    btn_confirmcreate->draw();
    btn_cancelcreate->draw();
}

void CreateWorldSubmenu::onResolutionChange(sf::Vector2u res)
{

}
