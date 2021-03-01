#include "submenu.h"
#include "../menu_state.h"

Submenu::Submenu(GameEngine* engine, MenuState* menu)
{
    m_engine = engine;
    m_menu = menu;
    m_allowEsc = true;
}
