#include "game_state.h"
#include "game_engine.h"
#include "ingame_state.h"
#include <SFML/Graphics.hpp>
#include <direct.h>

IngameState IngameState::m_Instance;

int intwrap(int number, int minimum, int maximum)
{
    if (number > maximum)
        return maximum;
    if (number < minimum)
        return minimum;
    return number;
}

void IngameState::init(GameEngine *engine)
{
    cam_x = 128;
    cam_y = 128;
    m_engine = engine;
    m_sky = sf::RectangleShape(sf::Vector2f(800, 480));
    m_sky.setFillColor(sf::Color(154, 190, 255));
    text_cam_pos.setFont(engine->mc_font);
    text_cam_pos.setScale(0.16667f, 0.16667f);
    text_cam_pos.setCharacterSize(96);
}

void IngameState::destroy()
{
    //m_engine->app.setView(m_engine->app.getDefaultView());
}

void IngameState::update(GameEngine *engine)
{

}

void IngameState::process_input(GameEngine *engine)
{
    sf::Event event;
    while (engine->app.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            engine->quit();

        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::R)
                cam_x = cam_y = 0;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        cam_y += 8;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        cam_y -= 8;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        cam_x += 8;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        cam_x -= 8;

    cam_x = intwrap(cam_x, 0, WORLD_W*32-800);
    cam_y = intwrap(cam_y, 0, WORLD_H*32-480);
}

void IngameState::draw(GameEngine *engine)
{
    sf::View m_view(sf::FloatRect(cam_x, cam_y, 800, 480));
    engine->app.setView(m_view);

    m_sky.setPosition(m_view.getCenter().x - 400.0f, m_view.getCenter().y - 240.0f);
    engine->app.draw(m_sky);

    char aBuf[192];
    sprintf(aBuf, "%d,%d", cam_x, cam_y);
    text_cam_pos.setString(sf::String(aBuf));
    text_cam_pos.setPosition(cam_x-400, cam_y-240);

    sf::RenderStates states;
    engine->app.draw(m_world.getBlocks(), &engine->m_blocks);
    return;
}

void IngameState::pause()
{

}

void IngameState::resume()
{

}

void IngameState::loadWorld(const char *worldName)
{
    char aName[96];
    sprintf(aName, "worlds/%s.dat", worldName);
    mkdir("worlds");
    std::fstream worldFile(aName);

    if (not worldFile.good())
    {
        printf("world %s does not exist, creating\n", aName);
        worldFile.close();
        generateWorld(rand(), worldName);
    }
    else
    {
        worldFile.close();
        m_world.loadWorld(worldName);
    }
    printf("starting\n");
}
