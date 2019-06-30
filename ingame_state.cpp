#include "game_state.h"
#include "game_engine.h"
#include "ingame_state.h"
#include "menu_state.h"
#include "paused_state.h"
#include <SFML/Graphics.hpp>
#include <direct.h>
#include <math.h>

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
    m_world = World(engine);

    m_sky = sf::RectangleShape(sf::Vector2f(800, 480));
    m_sky.setFillColor(sf::Color(154, 190, 255));

    m_blockoutline.setSize(sf::Vector2f(31, 31));
    m_blockoutline.setFillColor(sf::Color::Transparent);
    m_blockoutline.setOutlineColor(sf::Color::Black);
    m_blockoutline.setOutlineThickness(1);

    text_cam_pos.setFont(engine->mc_font);
    text_cam_pos.setScale(0.16667f, 0.16667f);
    text_cam_pos.setCharacterSize(96);
    text_cam_pos.setOutlineColor(sf::Color::Black);
    text_cam_pos.setOutlineThickness(10);
}

void IngameState::destroy()
{
    m_engine->m_window.setView(m_engine->m_window.getDefaultView());
}

void IngameState::update(GameEngine *engine)
{
    if (engine->leavingGame())
    {
        int leave_type = engine->leavingGame();
        engine->leaveGame(0);
        m_world.saveWorld();

        if (leave_type == 1) // return to main menu
            engine->changeState(MenuState::Instance());
        else if (leave_type == 2) // leave game entirely
            engine->quit();
    }
    m_player.update(engine);
}

void IngameState::process_input(GameEngine *engine)
{
    sf::Event event;
    while (engine->app.pollEvent(event))
    {
        m_player.event_input(engine, event);

        if (event.type == sf::Event::Closed)
        {
            m_world.saveWorld();
            engine->quit();
        }

        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::R)
                cam_x = cam_y = 0.f;
            else if (event.key.code == sf::Keyboard::F2)
            {
                char aFile[96];
                time_t atime = time(0);
                std::tm* now = localtime(&atime);
                sprintf(aFile, "%.2d-%.2d-%d %.2d.%.2d.%.2d.png", now->tm_mday, now->tm_mon+1, now->tm_year+1900, now->tm_hour, now->tm_min, now->tm_sec);

                engine->takeScreenshot().copyToImage().saveToFile(aFile);
                printf("screenshot saved as '%s'\n", aFile);
            }
            else if (event.key.code == sf::Keyboard::Escape)
                engine->pushState(PausedState::Instance());
        }

        else if (event.type == sf::Event::LostFocus)
            engine->pushState(PausedState::Instance());
    }

    m_player.process_input(engine);

    sf::Vector2f pos = m_player.getPos();
    sf::Vector2f spd = m_player.getSpeed();
    float old_cam_x = cam_x;
    float old_cam_y = cam_y;

    float cam_x_dist = ((pos.x - old_cam_x)-400)/16.0f;
    float cam_y_dist = ((pos.y - old_cam_y)-240)/16.0f;

    cam_x += cam_x_dist + (spd.x*2.0f);
    if (m_player.blockCollide(pos.x/32, pos.y/32) or
        m_player.blockCollide((pos.x-4)/32, pos.y/32) or
        m_player.blockCollide((pos.x+4)/32, pos.y/32) or
        (cam_y_dist > 8 or cam_y_dist < -8))
        cam_y += cam_y_dist;

    cam_x = numwrap(cam_x, 0.0f, WORLD_W*32-800.f);
    cam_y = numwrap(cam_y, 0.0f, WORLD_H*32-480.f);

    sf::Vector2f m_mousepos = m_player.getMouse();
    m_blockoutline.setPosition(floor(m_mousepos.x/32)*32, floor((m_mousepos.y-56)/32)*32);
}

void IngameState::draw(GameEngine *engine)
{
    sf::View m_view(sf::FloatRect(cam_x, cam_y, 800, 480));
    engine->m_window.setView(m_view);

    m_sky.setPosition(cam_x, cam_y);
    engine->m_window.draw(m_sky);

    int xx = (cam_x+400)/32;
    int yy = (cam_y+240)/32;

    engine->m_window.draw(m_world.getBlocksFromPoint(xx-CHUNK_W, yy), &engine->m_blocks);
    engine->m_window.draw(m_world.getBlocksFromPoint(xx, yy), &engine->m_blocks);
    if ((xx+CHUNK_W)/CHUNK_W < WORLD_W/CHUNK_W+1)
        engine->m_window.draw(m_world.getBlocksFromPoint(xx+CHUNK_W, yy), &engine->m_blocks);

    engine->m_window.draw(m_world.getBlocksFromPoint(xx-CHUNK_W, yy-CHUNK_H), &engine->m_blocks);
    engine->m_window.draw(m_world.getBlocksFromPoint(xx, yy-CHUNK_H), &engine->m_blocks);
    if ((xx+CHUNK_W)/CHUNK_W < WORLD_W/CHUNK_W+1)
        engine->m_window.draw(m_world.getBlocksFromPoint(xx+CHUNK_W, yy-CHUNK_H), &engine->m_blocks);

    if ((yy+CHUNK_H)/CHUNK_H < CHUNK_H-2)
    {
        engine->m_window.draw(m_world.getBlocksFromPoint(xx-CHUNK_W, yy+CHUNK_H), &engine->m_blocks);
        engine->m_window.draw(m_world.getBlocksFromPoint(xx, yy+CHUNK_H), &engine->m_blocks);
        if ((xx+CHUNK_W)/CHUNK_W < WORLD_W/CHUNK_W+1)
            engine->m_window.draw(m_world.getBlocksFromPoint(xx+CHUNK_W, yy+CHUNK_H), &engine->m_blocks);
    }

    m_player.draw(engine);

    sf::Vector2f outlinepos = m_blockoutline.getPosition();
    if (m_world.getBlock((outlinepos.x+00)/32, (outlinepos.y+00)/32) or
        m_world.getBlock((outlinepos.x+32)/32, (outlinepos.y+00)/32) or
        m_world.getBlock((outlinepos.x-32)/32, (outlinepos.y+00)/32) or
        m_world.getBlock((outlinepos.x+00)/32, (outlinepos.y+32)/32) or
        m_world.getBlock((outlinepos.x+00)/32, (outlinepos.y-32)/32))
        engine->m_window.draw(m_blockoutline);

    char aBuf[192];
    sprintf(aBuf, "%.1f,%.1f\nChunk position: %d,%d\nBuilding layer: %d\n", cam_x/32, cam_y/32, xx/CHUNK_W, yy/CHUNK_H, m_player.getBuildLayer());
    text_cam_pos.setString(sf::String(aBuf));
    text_cam_pos.setPosition(cam_x, cam_y);
    engine->m_window.draw(text_cam_pos);
}

void IngameState::pause()
{
    m_engine->setPaused(true);
    m_engine->takeScreenshot(true);
    m_engine->m_window.setView(m_engine->m_window.getDefaultView());
}

void IngameState::resume()
{
    m_engine->setPaused(false);
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

    m_player = Player(&m_world, m_engine);
    m_player.setPlayer(true);

    printf("starting\n");
}
