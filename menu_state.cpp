#include <SFML/Graphics.hpp>
#include "game_engine.h"
#include "menu_state.h"

MenuState MenuState::m_Instance;

void MenuState::init()
{
    m_dirt_tile.loadFromFile("data/gui/options_background.png");
    m_dirt_tile.setRepeated(true);

    dirt_tile.setTexture(m_dirt_tile);
    dirt_tile.setScale(2.0f, 2.0f);
    dirt_tile.setColor(sf::Color(128, 128, 128));
    dirt_tile.setTextureRect(sf::IntRect(0, 0, 320, 240));
}

void MenuState::destroy()
{

}

void MenuState::update(GameEngine* engine)
{

}

void MenuState::process_input(GameEngine* engine)
{

}

void MenuState::draw(GameEngine* engine)
{
    engine->app.draw(dirt_tile);
}

void MenuState::pause()
{

}

void MenuState::resume()
{

}
