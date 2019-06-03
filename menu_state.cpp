#include <SFML/Graphics.hpp>
#include "game_engine.h"
#include "menu_state.h"

MenuState MenuState::m_Instance;

void MenuState::init(GameEngine* engine)
{
    m_dirt_tile.loadFromFile("data/gui/options_background.png");
    m_dirt_tile.setRepeated(true);

    sf::Image m_minecraft_logo_raw;
    sf::Image m_minecraft_logo_final;

    m_minecraft_logo_raw.loadFromFile("data/gui/title/minecraft.png");
    m_minecraft_logo_final.create(274, 44, sf::Color(0, 0, 0, 0));
    m_minecraft_logo_final.copy(m_minecraft_logo_raw, 0, 0, sf::IntRect(0, 0, 155, 43)); // minec
    m_minecraft_logo_final.copy(m_minecraft_logo_raw, 155, 0, sf::IntRect(0, 45, 118, 88));

    dirt_tile.setTexture(m_dirt_tile);
    dirt_tile.setScale(2.0f, 2.0f);
    dirt_tile.setColor(sf::Color(128, 128, 128));
    dirt_tile.setTextureRect(sf::IntRect(0, 0, 400, 240));

    m_minecraft_logo.loadFromImage(m_minecraft_logo_final);
    minecraft_logo.setTexture(m_minecraft_logo);
    minecraft_logo.setPosition((800.0f/2.0f) - (274.0f/1.0f), 48.0f);
    minecraft_logo.setScale(2.0f, 2.0f);

    b_singleplayer = Button(engine, sf::String("Singleplayer"), 400 - 200, 192);
    b_multiplayer = Button(engine, sf::String("Multiplayer"), 400 - 200, b_singleplayer.getPos().y + 64);
    b_options = Button(engine, sf::String("Options"), 400 - 200, b_multiplayer.getPos().y + 64);
    b_quit = Button(engine, sf::String("Quit"), 400 - 200, b_options.getPos().y + 64);
}

void MenuState::destroy()
{

}

void MenuState::update(GameEngine* engine)
{
    b_singleplayer.update();
    b_multiplayer.update();
    b_options.update();
    if (b_quit.update())
        engine->quit();
}

void MenuState::process_input(GameEngine* engine)
{

}

void MenuState::draw(GameEngine* engine)
{
    engine->app.draw(dirt_tile);
    engine->app.draw(minecraft_logo);
    b_singleplayer.draw();
    b_multiplayer.draw();
    b_options.draw();
    b_quit.draw();
}

void MenuState::pause()
{

}

void MenuState::resume()
{

}
