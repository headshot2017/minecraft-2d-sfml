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
    cam_x = 0;
    cam_y = 0;
    m_engine = engine;

    m_sky = sf::RectangleShape(sf::Vector2f(800, 480));
    m_sky.setFillColor(sf::Color(154, 190, 255));

    text_cam_pos.setFont(engine->mc_font);
    text_cam_pos.setScale(0.16667f, 0.16667f);
    text_cam_pos.setCharacterSize(96);
    text_cam_pos.setOutlineColor(sf::Color::Black);
    text_cam_pos.setOutlineThickness(10);
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

    int xx = (cam_x+400)/32;
    int yy = (cam_y+240)/32;

    engine->app.draw(m_world.getBlocksFromPoint(xx, yy), &engine->m_blocks);
    engine->app.draw(m_world.getBlocksFromPoint(xx+CHUNK_W, yy), &engine->m_blocks);
    engine->app.draw(m_world.getBlocksFromPoint(xx-CHUNK_W, yy), &engine->m_blocks);

    engine->app.draw(m_world.getBlocksFromPoint(xx-CHUNK_W, yy-CHUNK_H), &engine->m_blocks);
    engine->app.draw(m_world.getBlocksFromPoint(xx, yy-CHUNK_H), &engine->m_blocks);
    engine->app.draw(m_world.getBlocksFromPoint(xx+CHUNK_W, yy-CHUNK_H), &engine->m_blocks);

    if ((yy+CHUNK_H)/CHUNK_H < CHUNK_H-2)
    {
        engine->app.draw(m_world.getBlocksFromPoint(xx-CHUNK_W, yy+CHUNK_H), &engine->m_blocks);
        engine->app.draw(m_world.getBlocksFromPoint(xx, yy+CHUNK_H), &engine->m_blocks);
        engine->app.draw(m_world.getBlocksFromPoint(xx+CHUNK_W, yy+CHUNK_H), &engine->m_blocks);
    }

    char aBuf[192];
    sprintf(aBuf, "%d,%d", cam_x/32, cam_y/32);
    text_cam_pos.setString(sf::String(aBuf));
    text_cam_pos.setPosition(cam_x, cam_y);
    engine->app.draw(text_cam_pos);
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
        srand(time(0));
        generateWorld(rand(), worldName);
    }
    else
    {
        worldFile.close();
        m_world.loadWorld(worldName);
    }
    printf("starting\n");
}
