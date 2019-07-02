#include "game_engine.h"
#include "game_state.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <string.h>
#include <fstream>

void GameEngine::init()
{
    m_settings.loadSettings();
    if (m_settings.m_fullscreen)
    {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        setResolution(sf::Vector2u(desktop.width, desktop.height), sf::Style::Fullscreen);
    }
    else
        setResolution(sf::Vector2u(800, 480));

    m_blocks.loadFromFile("data/blocks.png");

    mc_font.loadFromFile("data/font/Minecraftia.ttf");

    m_sound = new SoundEngine;
    m_sound->init();
    m_sound->loadTheme("default");

    sf::Image widgets;
    widgets.loadFromFile("data/gui/widgets.png");

    m_button.loadFromImage(widgets, sf::IntRect(0, 66, 200, 20));
    m_button_hover.loadFromImage(widgets, sf::IntRect(0, 86, 200, 20));
}

void GameEngine::cleanup()
{
    while (not states.empty())
    {
        states.back()->destroy();
        states.pop_back();
    }
    m_sound->cleanup();
    m_settings.saveSettings();
    delete m_sound;
}

void GameEngine::update()
{
    states.back()->update(this);
}

void GameEngine::process_input()
{
    states.back()->process_input(this);
}

void GameEngine::draw()
{
    app.clear();
    m_window.clear();
    states.back()->draw(this);
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
        states.back()->destroy();
        states.pop_back();
    }
    states.push_back(state);
    states.back()->init(this);
}

void GameEngine::pushState(GameState* state)
{
    if (states.empty())
        return;

    states.back()->pause();
    states.push_back(state);
    states.back()->init(this);
}

void GameEngine::popState()
{
    if (states.empty())
        return;

    states.back()->destroy();
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
    app.setSize(res);
    app.setFramerateLimit(60);
}
