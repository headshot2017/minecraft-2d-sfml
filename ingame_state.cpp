#include "game_state.h"
#include "game_engine.h"
#include "ingame_state.h"
#include <SFML/Graphics.hpp>
#include <direct.h>

IngameState IngameState::m_Instance;

float numwrap(float number, float minimum, float maximum)
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
    m_player.update(engine);
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
                cam_x = cam_y = 0.f;
        }
    }

    sf::Vector2f pos = m_player.getPos();
    sf::Vector2f spd = m_player.getSpeed();
    float old_cam_x = cam_x;
    float old_cam_y = cam_y;

    float cam_x_dist = ((pos.x - old_cam_x)-400)/16.0f;
    float cam_y_dist = ((pos.y - old_cam_y)-240)/16.0f;

    cam_x += cam_x_dist + (spd.x*2.0f);
    if (spd.y == 0 or (cam_y_dist > 360 or cam_y_dist < -360))
        cam_y += cam_y_dist;

    m_player.process_input(engine);

    cam_x = numwrap(cam_x, 0.0f, WORLD_W*32-800.f);
    cam_y = numwrap(cam_y, 0.0f, WORLD_H*32-480.f);
}

void IngameState::draw(GameEngine *engine)
{
    sf::View m_view(sf::FloatRect(cam_x, cam_y, 800, 480));
    engine->app.setView(m_view);

    m_sky.setPosition(cam_x, cam_y);
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

    m_player.draw(engine);

    char aBuf[192];
    sprintf(aBuf, "%.1f,%.1f", cam_x/32, cam_y/32);
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

    m_player = Player(m_world, m_engine);
    m_player.setPlayer(true);

    printf("starting\n");
}
