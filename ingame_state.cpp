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
    m_skytime = 0;
    m_engine = engine;
    m_world = new World(engine);

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
    delete m_world;
}

void IngameState::update(GameEngine *engine)
{
    if (engine->leavingGame())
    {
        int leave_type = engine->leavingGame();
        engine->leaveGame(0);
        m_world->saveWorld();

        if (leave_type == 1) // return to main menu
        {
            engine->changeState(MenuState::Instance());
        }
        else if (leave_type == 2) // leave game entirely
            engine->quit();
    }
    else
    {
        m_world->getPlayer()->update(engine);
        m_world->updateEntities();
    }
}

void IngameState::event_input(GameEngine *engine, sf::Event& event)
{
    m_world->getPlayer()->event_input(engine, event);

    if (event.type == sf::Event::Closed)
    {
        m_world->saveWorld();
        engine->quit();
    }

    else if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::R)
            cam_x = cam_y = 0.f;
        else if (event.key.code == sf::Keyboard::Escape)
            engine->pushState(PausedState::Instance());
    }

    else if (event.type == sf::Event::LostFocus)
        engine->pushState(PausedState::Instance());
}

void IngameState::process_input(GameEngine* engine)
{
    m_world->getPlayer()->process_input(engine);

    sf::Vector2f pos = m_world->getPlayer()->getPos();
    sf::Vector2f spd = m_world->getPlayer()->getSpeed();
    sf::Vector2u res = engine->app.getSize();
    float old_cam_x = cam_x;
    float old_cam_y = cam_y;

    float cam_x_dist = ((pos.x - old_cam_x)-(res.x/2))/16.0f;
    float cam_y_dist = ((pos.y - old_cam_y)-(res.y/2))/16.0f;

    cam_x += cam_x_dist + (spd.x*2.0f);
    if (m_world->getPlayer()->blockCollide(pos.x/32, pos.y/32) or
        m_world->getPlayer()->blockCollide((pos.x-4)/32, pos.y/32) or
        m_world->getPlayer()->blockCollide((pos.x+4)/32, pos.y/32) or
        (cam_y_dist > 8 or cam_y_dist < -8))
        cam_y += cam_y_dist;

    cam_x = numwrap(cam_x, 0.0f, WORLD_W*32-(res.x));
    cam_y = numwrap(cam_y, 0.0f, WORLD_H*32-(res.y));

    sf::Vector2f m_mousepos = m_world->getPlayer()->getMouse();
    m_blockoutline.setPosition(floor(m_mousepos.x/32)*32, floor((m_mousepos.y-56)/32)*32);
}

void IngameState::draw(GameEngine *engine)
{
    sf::Vector2u windowsize = engine->app.getSize();
    sf::View m_view(sf::FloatRect(cam_x, cam_y, windowsize.x, windowsize.y));
    engine->m_window.setView(m_view);

    m_sky.setSize(sf::Vector2f(windowsize.x, windowsize.y));
    m_sky.setPosition(cam_x, cam_y);
    engine->m_window.draw(m_sky);

    int xx = (cam_x+400)/32;
    int yy = (cam_y+240)/32;

    // draw visible chunks
    engine->m_window.draw(m_world->getBlocksFromPoint(xx, yy), &engine->m_blocks);
    unsigned int maxColumns = windowsize.x/(CHUNK_W*32);
    unsigned int maxRows = windowsize.y/(CHUNK_H*32);
    if (windowsize.x % (CHUNK_W*32)) maxColumns++;
    if (windowsize.y % (CHUNK_H*32)) maxRows++;
    if (windowsize.x / (CHUNK_W*32) < 1) maxColumns++;
    if (windowsize.y / (CHUNK_H*32) < 1) maxRows++;
    for(unsigned int xsep=1; xsep<maxColumns; xsep++)
    {
        engine->m_window.draw(m_world->getBlocksFromPoint(xx-(CHUNK_W*xsep), yy), &engine->m_blocks);
        if ((xx+(CHUNK_W*xsep))/CHUNK_W < WORLD_W/CHUNK_W+1)
            engine->m_window.draw(m_world->getBlocksFromPoint(xx+(CHUNK_W*xsep), yy), &engine->m_blocks);

        for (unsigned int ysep=1; ysep<maxRows; ysep++)
        {
            engine->m_window.draw(m_world->getBlocksFromPoint(xx-(CHUNK_W*xsep), yy-(CHUNK_H*ysep)), &engine->m_blocks);
            engine->m_window.draw(m_world->getBlocksFromPoint(xx, yy-(CHUNK_H*ysep)), &engine->m_blocks);
            if ((xx+(CHUNK_W*xsep))/CHUNK_W < WORLD_W/CHUNK_W+1)
                engine->m_window.draw(m_world->getBlocksFromPoint(xx+(CHUNK_W*xsep), yy-(CHUNK_H*ysep)), &engine->m_blocks);

            if ((yy+(CHUNK_H*ysep))/CHUNK_H < WORLD_H/CHUNK_H-2)
            {
                engine->m_window.draw(m_world->getBlocksFromPoint(xx-(CHUNK_W*xsep), yy+(CHUNK_H*ysep)), &engine->m_blocks);
                engine->m_window.draw(m_world->getBlocksFromPoint(xx, yy+(CHUNK_H*ysep)), &engine->m_blocks);
                if ((xx+(CHUNK_W*xsep))/CHUNK_W < WORLD_W/CHUNK_W+1)
                    engine->m_window.draw(m_world->getBlocksFromPoint(xx+(CHUNK_W*xsep), yy+(CHUNK_H*ysep)), &engine->m_blocks);
            }
        }
    }

    // world
    m_world->getPlayer()->draw(engine);
    m_world->drawEntities();

    // mouse block outline
    sf::Vector2f outlinepos = m_blockoutline.getPosition();
    if (m_world->getBlock((outlinepos.x+00)/32, (outlinepos.y+00)/32) or
        m_world->getBlock((outlinepos.x+32)/32, (outlinepos.y+00)/32) or
        m_world->getBlock((outlinepos.x-32)/32, (outlinepos.y+00)/32) or
        m_world->getBlock((outlinepos.x+00)/32, (outlinepos.y+32)/32) or
        m_world->getBlock((outlinepos.x+00)/32, (outlinepos.y-32)/32))
        engine->m_window.draw(m_blockoutline);

    // inventory
    sf::Sprite hotbar(engine->m_hotbar);
    sf::Sprite hotbarselect(engine->m_hotbarselect);
    hotbar.scale(2,2);
    hotbarselect.scale(2,2);
    hotbar.setPosition(cam_x + (windowsize.x/2) - engine->m_hotbar.getSize().x, windowsize.y - (engine->m_hotbar.getSize().y*2)+cam_y);
    hotbarselect.setPosition(hotbar.getPosition().x - 2 + (m_hotbarslot*32), hotbar.getPosition().y - 2);
    engine->m_window.draw(hotbar);
    engine->m_window.draw(hotbarselect);

    char aBuf[192];
    sprintf(aBuf, "%.1f,%.1f\nChunk position: %d,%d\nBuilding layer: %d\n", cam_x/32, cam_y/32, xx/CHUNK_W, yy/CHUNK_H, m_world->getPlayer()->getBuildLayer());
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
        m_world->loadWorld(worldName);
    }

    m_world->getPlayer()->setPlayer(true);
    m_world->getPlayer()->setSkin(m_engine->Settings()->m_playerskin);

    printf("starting\n");
}
