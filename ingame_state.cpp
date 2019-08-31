#include "game_state.h"
#include "game_engine.h"
#include "ingame_state.h"
#include "menu_state.h"
#include "paused_state.h"
#include "gui/label.h"
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
    m_gamegui = new GameGUI;

    m_freecam = false;
    m_showgui = true;

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
    delete m_gamegui;
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
        m_gamegui->update(engine);
    }
}

void IngameState::event_input(GameEngine *engine, sf::Event& event)
{
    m_world->getPlayer()->event_input(engine, event);
    int guiresult = m_gamegui->event_input(engine, event);
    if (guiresult > -1)
    {
        if (m_gamegui->getGUI() == GUI_INVENTORY)
        {
            if (m_inventory[m_hotbarslot][0] != guiresult or not m_inventory[m_hotbarslot][0])
            {
                m_inventory[m_hotbarslot][0] = guiresult;
                m_inventory[m_hotbarslot][1] = 1;
                setHotbarSlot(m_hotbarslot);
            }
            else if (m_inventory[m_hotbarslot][0] == guiresult and m_inventory[m_hotbarslot][1] < 64)
                m_inventory[m_hotbarslot][1]++;
        }
    }

    if (engine->Settings()->controls()->PressedEvent("inventory", event))
    {
        if (not m_gamegui->isOpen())
        {
            m_gamegui->openInventory();
            m_world->getPlayer()->setCanMove(false);
        }
        else
        {
            m_gamegui->closeGUI();
            m_world->getPlayer()->setCanMove(true);
        }
    }
    else if (engine->Settings()->controls()->PressedEvent("drop", event))
    {
        if (m_inventory[m_hotbarslot][0])
        {
            m_inventory[m_hotbarslot][1]--;
            if (m_inventory[m_hotbarslot][1] <= 0)
            {
                m_inventory[m_hotbarslot][0] = BLOCK_AIR;
                setHotbarSlot(m_hotbarslot);
            }
        }
    }
    else if (engine->Settings()->controls()->PressedEvent("pick", event))
    {
        sf::Vector2f mousepos = m_world->getPlayer()->getMouse();
        int xx = mousepos.x/32;
        int yy = (mousepos.y-56)/32;
        int block = m_world->getBlock(xx, yy);
        int hotbar_filled = 0;

        for(int ii=0; ii<18; ii++)
        {
            int i = ii%9;
            if (ii < 9 and m_inventory[i]) hotbar_filled++;

            if (ii < 9 and m_inventory[i][0] == block and m_inventory[i][1] < 64) // check if the block is already in the hotbar, and add amount
            {
                m_inventory[i][1]++;
                setHotbarSlot(i);
                break;
            }
            else if (ii >= 9 and not m_inventory[i][0]) // this block is not in the hotbar so add it to the nearest empty slot
            {
                m_inventory[i][0] = block;
                m_inventory[i][1] = 1;
                setHotbarSlot(i);
                break;
            }
        }

        if (hotbar_filled == 9) // hotbar is full, replace the current slot
        {
            m_inventory[m_hotbarslot][0] = block;
            m_inventory[m_hotbarslot][1] = 1;
        }
    }

    if (event.type == sf::Event::Closed)
    {
        m_world->saveWorld();
        engine->quit();
    }

    else if (event.type == sf::Event::KeyPressed)
    {
        int intcode = static_cast<int>(event.key.code);
        int num1code = static_cast<int>(sf::Keyboard::Num1);
        int num9code = static_cast<int>(sf::Keyboard::Num9);

        if (event.key.code == sf::Keyboard::R)
            cam_x = cam_y = 0.f;
        else if (event.key.code == sf::Keyboard::Escape)
            engine->pushState(PausedState::Instance());
        else if (intcode >= num1code and intcode <= num9code) // hotbar
            setHotbarSlot(intcode - num1code);
        else if (event.key.code == sf::Keyboard::F1) // show hotbar, etc
            m_showgui = not m_showgui;
        else if (event.key.code == sf::Keyboard::F4) // freecam
        {
            m_freecam = not m_freecam;
            m_world->getPlayer()->setCanMove(not m_freecam);
        }
        else if (event.key.code == sf::Keyboard::Right and m_freecam)
            cam_x = (floor(cam_x/32)*32) + m_freecam_add;
        else if (event.key.code == sf::Keyboard::Left and m_freecam)
            cam_x = (floor(cam_x/32)*32) - m_freecam_add;
        else if (event.key.code == sf::Keyboard::Down and m_freecam)
            cam_y = (floor(cam_y/32)*32) + m_freecam_add;
        else if (event.key.code == sf::Keyboard::Up and m_freecam)
            cam_y = (floor(cam_y/32)*32) - m_freecam_add;
    }
    else if (event.type == sf::Event::MouseWheelScrolled)
    {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            m_hotbarslot -= event.mouseWheelScroll.delta;

        if (m_hotbarslot < 0)
            setHotbarSlot(9+m_hotbarslot);
        else if (m_hotbarslot >= 9)
            setHotbarSlot(-9+m_hotbarslot);
        else
            setHotbarSlot(m_hotbarslot);
    }

    else if (event.type == sf::Event::LostFocus)
        engine->pushState(PausedState::Instance());
}

void IngameState::process_input(GameEngine* engine)
{
    m_world->getPlayer()->process_input(engine);
    if (m_world->getPlayer()->getCanMove())
    {
        sf::Vector2f mousepos = m_world->getPlayer()->getMouse();
        if (engine->Settings()->controls()->Pressed("place") and not rmb)
        {
            if (m_world->getPlayer()->canBuild(mousepos.x/32, (mousepos.y-56)/32))
            {
                if (m_world->getBlock(mousepos.x/32, (mousepos.y-56)/32))
                    m_world->getPlayer()->interactBlock(mousepos.x/32, (mousepos.y-56)/32);
                else
                {
                    if (m_inventory[m_hotbarslot][0])
                        m_world->getPlayer()->placeBlock(mousepos.x/32, (mousepos.y-56)/32, m_inventory[m_hotbarslot][0], m_world->getPlayer()->getBuildLayer());
                }
            }
            rmb = true;
        }
        else if (not engine->Settings()->controls()->Pressed("place") and rmb)
            rmb = false;

        if (engine->Settings()->controls()->Pressed("destroy"))
        {
            if (not lmb_tick)
            {
                m_world->getPlayer()->destroyBlock(mousepos.x/32, (mousepos.y-56)/32);
                lmb_tick = 60 * 0.22;
            }
            else
                lmb_tick--;
            lmb = true;
        }
        else
        {
            lmb = false;
            lmb_tick = 0;
        }
    }

    // smooth camera
    sf::Vector2u res = engine->app.getSize();
    if (not m_freecam)
    {
        sf::Vector2f pos = m_world->getPlayer()->getPos();
        sf::Vector2f spd = m_world->getPlayer()->getSpeed();
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
    }

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

    int xx = (cam_x+(windowsize.x/2))/32;
    int yy = (cam_y+(windowsize.y/2))/32;

    // draw visible chunks
    engine->m_window.draw(m_world->getBlocksFromPoint(xx, yy), &engine->m_blocks);
    unsigned int maxColumns = (windowsize.x/(CHUNK_W*32));
    unsigned int maxRows = (windowsize.y/(CHUNK_H*32));
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

    if (m_showgui)
    {
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
        hotbarselect.setPosition(hotbar.getPosition().x - 2 + (m_hotbarslot*40), hotbar.getPosition().y - 2);
        engine->m_window.draw(hotbar);
        engine->m_window.draw(hotbarselect);
        for (int i=0; i<9; i++) // hotbar blocks
        {
            if (m_inventory[i][0])
            {
                char amount[16];
                sprintf(amount, "%d", m_inventory[i][1]);
                sf::Sprite block(engine->m_blocks, sf::IntRect(m_inventory[i][0]*32, 0, 32, 32));
                Label text_amount(engine, amount, cam_x + (windowsize.x/2) - engine->m_hotbar.getSize().x + (i*40)+36, windowsize.y - (engine->m_hotbar.getSize().y*2)+cam_y + 20, 2);

                block.setPosition(cam_x + (windowsize.x/2) - engine->m_hotbar.getSize().x + (i*40)+6, windowsize.y - (engine->m_hotbar.getSize().y*2)+cam_y + 6);

                engine->m_window.draw(block);
                if (m_inventory[i][1] > 1)
                    text_amount.draw();
            }
        }

        char aBuf[192];
        sprintf(aBuf, "%.1f,%.1f\nChunk position: %d,%d\nBuilding layer: %d\nLayer 1 collisions: %s\n", cam_x/32, cam_y/32, xx/CHUNK_W, yy/CHUNK_H, m_world->getPlayer()->getBuildLayer(), m_world->getPlayer()->getLayer1Collisions() ? "Yes" : "No");
        text_cam_pos.setString(sf::String(aBuf));
        text_cam_pos.setPosition(cam_x, cam_y);
        engine->m_window.draw(text_cam_pos);
    }

    m_gamegui->draw(engine); // inventory window, crafting window, etc
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
    setHotbarSlot(0);

    printf("starting\n");
}

void IngameState::setHotbarSlot(int slot)
{
    m_hotbarslot = slot;
    m_world->getPlayer()->setCurrBlock(m_inventory[slot][0]);
}
