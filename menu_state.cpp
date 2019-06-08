#include <SFML/Graphics.hpp>
#include "game_engine.h"
#include "menu_state.h"
#include "ingame_state.h"

MenuState MenuState::m_Instance;

void MenuState::init(GameEngine* engine)
{
    m_dirt_tile.loadFromFile("data/gui/options_background.png");
    m_dirt_tile.setRepeated(true);

    sf::Image m_minecraft_logo_raw;
    sf::Image m_minecraft_logo_final;

    m_minecraft_logo_raw.loadFromFile("data/gui/title/minecraft.png");
    m_minecraft_logo_final.create(274, 44, sf::Color(0, 0, 0, 0));
    m_minecraft_logo_final.copy(m_minecraft_logo_raw, 0, 0, sf::IntRect(0, 0, 155, 44)); // minec
    m_minecraft_logo_final.copy(m_minecraft_logo_raw, 155, 0, sf::IntRect(0, 45, 119, 88));

    dirt_tile.setTexture(m_dirt_tile);
    dirt_tile.setScale(2.0f, 2.0f);
    dirt_tile.setColor(sf::Color(128, 128, 128));
    dirt_tile.setTextureRect(sf::IntRect(0, 0, 400, 240));

    m_minecraft_logo.loadFromImage(m_minecraft_logo_final);
    minecraft_logo.setTexture(m_minecraft_logo);
    minecraft_logo.setPosition((800.0f/2.0f) - (274.0f/1.0f), 48.0f);
    minecraft_logo.setScale(2.0f, 2.0f);

    b_back = Button(engine, sf::String("Back"), 400-200, 480-48);

    b_singleplayer = Button(engine, sf::String("Singleplayer"), 400 - 200, 192);
    b_multiplayer = Button(engine, sf::String("Multiplayer"), 400 - 200, b_singleplayer.getPos().y + 64);
    b_options = Button(engine, sf::String("Options"), 400 - 200, b_multiplayer.getPos().y + 64);
    b_quit = Button(engine, sf::String("Quit"), 400 - 200, b_options.getPos().y + 64);

    b_world1 = Button(engine, sf::String("World 1"), 400 - 200, 96);
    b_world2 = Button(engine, sf::String("World 2"), 400 - 200, 96+48);
    b_world3 = Button(engine, sf::String("World 3"), 400 - 200, 96+(48*2));
    b_world4 = Button(engine, sf::String("World 4"), 400 - 200, 96+(48*3));
    b_world5 = Button(engine, sf::String("World 5"), 400 - 200, 96+(48*4));

    b_connect = Button(engine, sf::String("Connect"), 400-200, 96+64);
}

void MenuState::destroy()
{

}

void MenuState::update(GameEngine* engine)
{

}

void MenuState::process_input(GameEngine* engine)
{
    sf::Event event;
    while (engine->app.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            engine->quit();

        else if (event.type == sf::Event::MouseButtonPressed)
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                if (m_submenu == MENU_MAINMENU)
                {
                    b_singleplayer.process_input();
                    b_multiplayer.process_input();
                    b_options.process_input();
                    b_quit.process_input();
                }
                else if (m_submenu == MENU_LOADWORLD)
                {
                    b_back.process_input();
                    b_world1.process_input();
                    b_world2.process_input();
                    b_world3.process_input();
                    b_world4.process_input();
                    b_world5.process_input();
                }
                else if (m_submenu == MENU_MULTIPLAYER)
                {
                    b_back.process_input();
                    b_connect.process_input();
                }
                else if (m_submenu == MENU_OPTIONS)
                    b_back.process_input();
            }
    }

    if (m_submenu == MENU_MAINMENU)
    {
        if (b_singleplayer.update())
            m_submenu = MENU_LOADWORLD;
        if (b_multiplayer.update())
            m_submenu = MENU_MULTIPLAYER;
        if (b_options.update())
            m_submenu = MENU_OPTIONS;
        if (b_quit.update())
            engine->quit();
    }
    else if (m_submenu == MENU_LOADWORLD)
    {
        int world = 0;
        if (b_back.update())
            m_submenu = MENU_MAINMENU;
        if (b_world1.update())
            world = 1;
        if (b_world2.update())
            world = 2;
        if (b_world3.update())
            world = 3;
        if (b_world4.update())
            world = 4;
        if (b_world5.update())
            world = 5;

        if (world > 0) // world selected
        {
            IngameState* state = IngameState::Instance();
            engine->changeState(state);
            if (world == 1)
                state->loadWorld("world 1");
            if (world == 2)
                state->loadWorld("world 2");
            if (world == 3)
                state->loadWorld("world 3");
            if (world == 4)
                state->loadWorld("world 4");
            if (world == 5)
                state->loadWorld("world 5");
            printf("leaving menu state\n");
            return;
        }
    }
    else if (m_submenu == MENU_MULTIPLAYER)
    {
        if (b_back.update())
            m_submenu = MENU_MAINMENU;
        b_connect.update();
    }
    else if (m_submenu == MENU_OPTIONS)
    {
        if (b_back.update())
            m_submenu = MENU_MAINMENU;
    }
}

void MenuState::draw(GameEngine* engine)
{
    engine->app.draw(dirt_tile);
    if (m_submenu == MENU_MAINMENU)
    {
        engine->app.draw(minecraft_logo);
        b_singleplayer.draw();
        b_multiplayer.draw();
        b_options.draw();
        b_quit.draw();
    }
    else if (m_submenu == MENU_LOADWORLD)
    {
        b_world1.draw();
        b_world2.draw();
        b_world3.draw();
        b_world4.draw();
        b_world5.draw();
        b_back.draw();
    }
    else if (m_submenu == MENU_MULTIPLAYER)
    {
        b_connect.draw();
        b_back.draw();
    }
    else if (m_submenu == MENU_OPTIONS)
    {
        b_back.draw();
    }
}

void MenuState::pause()
{

}

void MenuState::resume()
{

}
