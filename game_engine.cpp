#include "game_engine.h"
#include "game_state.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <algorithm>

void GameEngine::init()
{
    m_settings.loadSettings();

    sf::Uint32 flags = m_settings.m_fullscreen ? sf::Style::Fullscreen : sf::Style::Close;
    setResolution(sf::Vector2u(m_settings.m_screenwidth, m_settings.m_screenheight), flags);

    m_blocks.loadFromFile("data/blocks.png");
    m_explosion.loadFromFile("data/entity/explosion.png");
    m_sun.loadFromFile("data/environment/sun.png");
    m_moon.loadFromFile("data/environment/moon.png");

    mc_font.loadFromFile("data/font/Minecraftia.ttf");

    m_sound = new SoundEngine;
    m_sound->init(&m_settings);
    m_sound->loadTheme(m_settings.m_soundtheme);

    sf::Image widgets;
    widgets.loadFromFile("data/gui/widgets.png");

    m_button.loadFromImage(widgets, sf::IntRect(0, 66, 200, 20));
    m_button_hover.loadFromImage(widgets, sf::IntRect(0, 86, 200, 20));
    m_button_locked.loadFromImage(widgets, sf::IntRect(0, 46, 200, 20));
    m_hotbar.loadFromImage(widgets, sf::IntRect(0, 0, 182, 22));
    m_hotbarselect.loadFromImage(widgets, sf::IntRect(0, 22, 24, 24));

    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    bool original_res = false;
    for (unsigned int i=0; i<modes.size(); i++)
    {
        if (modes[i].bitsPerPixel == 32 and modes[i].width >= 640 and modes[i].height >= 480)
        {
            if (modes[i].width == 800 and modes[i].height == 480)
                original_res = true;
            m_videomodes.push_back(modes[i]);
        }
    }
    std::reverse(m_videomodes.begin(), m_videomodes.end());

    if (not original_res)
    {
        sf::VideoMode original(800, 480);
        std::vector<sf::VideoMode>::iterator it = m_videomodes.begin();
        it = m_videomodes.insert(it, original);
        m_videomodes.insert(it, 0, original);
    }

    m_clock.restart();
    m_ticks = 0;
    m_running = true;
    m_paused = false;
    m_leavegame = 0;
}

void GameEngine::cleanup()
{
    while (not states.empty())
    {
        delete states.back();
        states.pop_back();
    }
    m_sound->cleanup();
    m_settings.saveSettings();
    m_videomodes.clear();
    delete m_sound;
}

void GameEngine::update()
{
    float delta = m_clock.getElapsedTime().asSeconds();
    states.back()->update(delta);
    m_clock.restart();
}

void GameEngine::process_input()
{
    sf::Event event;
    while (app.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            if (isPaused())
            {
                leaveGame(2);
                popState();
            }
        }
        else if (m_settings.controls()->PressedEvent("fullscreen", event))
        {
            bool fullscreen = not m_settings.m_fullscreen;
            m_settings.m_fullscreen = fullscreen;

            sf::Uint32 flags = fullscreen ? sf::Style::Fullscreen : sf::Style::Close;
            setResolution(sf::Vector2u(m_settings.m_screenwidth, m_settings.m_screenheight), flags);
        }
        else if (m_settings.controls()->PressedEvent("screenshot", event))
        {
            char aFile[96];
            time_t atime = time(0);
            std::tm* now = localtime(&atime);
            sprintf(aFile, "%.2d-%.2d-%d %.2d.%.2d.%.2d.png", now->tm_mday, now->tm_mon+1, now->tm_year+1900, now->tm_hour, now->tm_min, now->tm_sec);

            takeScreenshot().copyToImage().saveToFile(aFile);
            printf("screenshot saved as '%s'\n", aFile);
        }

        states.back()->event_input(event);
    }

    states.back()->process_input();
}

void GameEngine::draw()
{
    app.clear();
    m_window.clear();
    states.back()->draw();
    m_window.display();
    app.draw(sf::Sprite(m_window.getTexture()));
    app.display();
}

void GameEngine::quit()
{
    app.close();
    m_running = false;
}

void GameEngine::changeState(GameState* state)
{
    if (not states.empty())
    {
        delete states.back();
        states.pop_back();
    }
    states.push_back(state);
}

void GameEngine::pushState(GameState* state)
{
    if (states.empty())
        return;

    states.back()->pause();
    states.push_back(state);
}

void GameEngine::popState()
{
    if (states.empty())
        return;

    delete states.back();
    states.pop_back();
    states.back()->resume();
}

const sf::Texture& GameEngine::takeScreenshot(bool saveTexture)
{
    if (saveTexture)
    {
        m_screenshot = m_window.getTexture();
        return m_screenshot;
    }
    return m_window.getTexture();
}

void GameEngine::setResolution(sf::Vector2u res, sf::Uint32 flags)
{
    app.create(sf::VideoMode(res.x, res.y), "Minecraft 2D", flags);
    m_window.create(res.x, res.y);
    //app.setSize(res);
    app.setFramerateLimit(60);

    sf::Image icon;
    icon.loadFromFile("data/icons/game.png");
    app.setIcon(16, 16, icon.getPixelsPtr());

    m_settings.m_screenwidth = res.x;
    m_settings.m_screenheight = res.y;

    if (not states.empty())
        states.back()->onResolutionChange(res);
}

sf::VertexArray GameEngine::getGUIWindow(sf::Vector2f size)
{
    sf::VertexArray guiwindow(sf::Quads, 4);
    sf::Vector2f view = m_window.getView().getCenter();
    for(int i=0; i<4; i++)
    {
        if (i<2)
            guiwindow[i].color = sf::Color(0, 0, 0, 192);
        else
            guiwindow[i].color = sf::Color(0, 0, 64, 192);
    }
    guiwindow[0].position = sf::Vector2f(view.x-(size.x/2.0f), view.y-(size.y/2.0f));
    guiwindow[1].position = sf::Vector2f(view.x+(size.x/2.0f), view.y-(size.y/2.0f));
    guiwindow[2].position = sf::Vector2f(view.x+(size.x/2.0f), view.y+(size.y/2.0f));
    guiwindow[3].position = sf::Vector2f(view.x-(size.x/2.0f), view.y+(size.y/2.0f));
    return guiwindow;
}
