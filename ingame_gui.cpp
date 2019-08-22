#include "ingame_gui.h"

void GameGUI::update(GameEngine *engine)
{

}

void GameGUI::event_input(GameEngine *engine, sf::Event& event)
{

}

void GameGUI::draw(GameEngine *engine)
{
    if (m_gui == GUI_INVENTORY)
    {
        engine->m_window.draw(engine->getGUIWindow(sf::Vector2f(400, 300)));
    }
}
