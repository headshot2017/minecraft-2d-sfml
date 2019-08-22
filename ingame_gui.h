#ifndef INGAME_GUI_H_INCLUDED
#define INGAME_GUI_H_INCLUDED

#include "game_engine.h"

enum GUIS
{
    GUI_NONE,
    GUI_INVENTORY
};

class GameGUI
{
public:
    GameGUI() {m_gui = GUI_NONE;}

    void update(GameEngine *engine);
    void event_input(GameEngine *engine, sf::Event& event);
    void draw(GameEngine *engine);

    void setGUI(GUIS gui) {m_gui = gui;}
    void openInventory() {setGUI(GUI_INVENTORY);}
    void closeGUI() {setGUI(GUI_NONE);}

    bool isOpen() {return m_gui != GUI_NONE;}

private:
    GUIS m_gui;
};

#endif // INGAME_GUI_H_INCLUDED
