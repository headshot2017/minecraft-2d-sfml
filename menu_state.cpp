#include <SFML/Graphics.hpp>
#include "game_engine.h"
#include "menu_state.h"
#include "ingame_state.h"

MenuState MenuState::m_Instance;

void MenuState::init(GameEngine* engine)
{
    if (not engine->isPaused())
        m_submenu = MENU_MAINMENU;
    else
        m_submenu = MENU_OPTIONS;

    sf::Vector2u windowsize = engine->app.getSize();

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

    m_minecraft_logo.loadFromImage(m_minecraft_logo_final);
    minecraft_logo.setTexture(m_minecraft_logo);
    minecraft_logo.setPosition((windowsize.x/2) - 274.0f, (windowsize.y/4)-64);
    minecraft_logo.setScale(2.0f, 2.0f);

    b_back = Button(engine, sf::String("Back"), (windowsize.x/2)-200, windowsize.y-48);
    b_back_options = Button(engine, sf::String("Back"), (windowsize.x/2)-200, windowsize.y-48);

    b_singleplayer = Button(engine, sf::String("Singleplayer"), (windowsize.x/2)-200, windowsize.y/4+64);
    b_multiplayer = Button(engine, sf::String("Multiplayer"), (windowsize.x/2)-200, b_singleplayer.getPos().y + 64);
    b_options = Button(engine, sf::String("Options"), (windowsize.x/2)-200, b_multiplayer.getPos().y + 64);
    b_quit = Button(engine, sf::String("Quit"), (windowsize.x/2)-200, b_options.getPos().y + 64);

    b_world1 = Button(engine, sf::String("World 1"), (windowsize.x/2)-200, (windowsize.y/4)-32);
    b_world2 = Button(engine, sf::String("World 2"), (windowsize.x/2)-200, (windowsize.y/4)-32+48);
    b_world3 = Button(engine, sf::String("World 3"), (windowsize.x/2)-200, (windowsize.y/4)-32+(48*2));
    b_world4 = Button(engine, sf::String("World 4"), (windowsize.x/2)-200, (windowsize.y/4)-32+(48*3));
    b_world5 = Button(engine, sf::String("World 5"), (windowsize.x/2)-200, (windowsize.y/4)-32+(48*4));

    b_connect = Button(engine, sf::String("Connect"), (windowsize.x/2)-200, (windowsize.y/4)-32+64);

    b_options_player = Button(engine, "Player", (windowsize.x/2)-200, 128);
    b_options_graphics = Button(engine, "Graphics", (windowsize.x/2)-200, 128+48);
    b_options_controls = Button(engine, "Controls", (windowsize.x/2)-200, 128+96);

    label_playername = Label(engine, "Player name", (windowsize.x/2)-200, 96);
    input_playername = TextInput(engine, engine->Settings()->m_playername, sf::Vector2f((windowsize.x/2)-200, 96+24), sizeof(engine->Settings()->m_playername));
    label_playerskin = Label(engine, "Player skin", (windowsize.x/2)-200, 96+88);
    input_playerskin = TextInput(engine, engine->Settings()->m_playerskin, sf::Vector2f((windowsize.x/2)-200, 96+112), sizeof(engine->Settings()->m_playerskin));
    char skin[64];
    sprintf(skin, "data/skins/%s.png", input_playerskin.getString().toAnsiString().c_str());
    txt_playerskin.loadFromFile(skin);

    b_moveleft = Button(engine, "Move left: ", (windowsize.x/2)-300-8, 64, 300);
    b_moveright = Button(engine, "Move right: ", (windowsize.x/2)-300-8, 64+48, 300);
    b_jump = Button(engine, "Jump: ", (windowsize.x/2)-300-8, 64+(48*2), 300);
    b_sneak = Button(engine, "Sneak: ", (windowsize.x/2)-300-8, 64+(48*3), 300);
    b_run = Button(engine, "Run: ", (windowsize.x/2)-300-8, 64+(48*4), 300);
    b_layerswap = Button(engine, "Swap layers: ", (windowsize.x/2)-300-8, 64+(48*5), 300);
    b_place = Button(engine, "Place block: ", (windowsize.x/2)+8, 64+(48*0), 300);
    b_destroy = Button(engine, "Destroy block: ", (windowsize.x/2)+8, 64+(48*1), 300);
    b_pick = Button(engine, "Pick block: ", (windowsize.x/2)+8, 64+(48*2), 300);

    l_pressakey = Label(engine, "", windowsize.x/2, windowsize.y/2-48, 1);

    setAllPositions(engine);
}

void MenuState::destroy()
{

}

void MenuState::setAllPositions(GameEngine* engine)
{
    sf::Vector2u windowsize = engine->app.getSize();

    dirt_tile.setTextureRect(sf::IntRect(0, 0, windowsize.x/2, windowsize.y/2));
    minecraft_logo.setPosition((windowsize.x/2) - 274.0f, (windowsize.y/4)-64);

    b_back.setPosition((windowsize.x/2)-200, windowsize.y-48);
    b_back_options.setPosition((windowsize.x/2)-200, windowsize.y-48);

    b_singleplayer.setPosition((windowsize.x/2)-200, windowsize.y/4+64);
    b_multiplayer.setPosition((windowsize.x/2)-200, b_singleplayer.getPos().y + 64);
    b_options.setPosition((windowsize.x/2)-200, b_multiplayer.getPos().y + 64);
    b_quit.setPosition((windowsize.x/2)-200, b_options.getPos().y + 64);

    b_world1.setPosition((windowsize.x/2)-200, (windowsize.y/4)-32);
    b_world2.setPosition((windowsize.x/2)-200, (windowsize.y/4)-32+48);
    b_world3.setPosition((windowsize.x/2)-200, (windowsize.y/4)-32+(48*2));
    b_world4.setPosition((windowsize.x/2)-200, (windowsize.y/4)-32+(48*3));
    b_world5.setPosition((windowsize.x/2)-200, (windowsize.y/4)-32+(48*4));

    b_connect.setPosition((windowsize.x/2)-200, (windowsize.y/4)-32+64);

    b_options_player.setPosition((windowsize.x/2)-200, 128);
    b_options_graphics.setPosition((windowsize.x/2)-200, 128+48);
    b_options_controls.setPosition((windowsize.x/2)-200, 128+96);

    label_playername.setPosition((windowsize.x/2)-200, 96);
    input_playername.setPosition((windowsize.x/2)-200, 96+24);
    label_playerskin.setPosition((windowsize.x/2)-200, 96+88);
    input_playerskin.setPosition((windowsize.x/2)-200, 96+112);

    b_moveleft.setPosition((windowsize.x/2)-300-8, 64);
    b_moveright.setPosition((windowsize.x/2)-300-8, 64+48);
    b_jump.setPosition((windowsize.x/2)-300-8, 64+(48*2));
    b_sneak.setPosition((windowsize.x/2)-300-8, 64+(48*3));
    b_run.setPosition((windowsize.x/2)-300-8, 64+(48*4));
    b_layerswap.setPosition((windowsize.x/2)-300-8, 64+(48*5));
    b_place.setPosition((windowsize.x/2)+8, 64+(48*0));
    b_destroy.setPosition((windowsize.x/2)+8, 64+(48*1));
    b_pick.setPosition((windowsize.x/2)+8, 64+(48*2));

    l_pressakey.setPosition(windowsize.x/2, windowsize.y/2-48);
}

void MenuState::update(GameEngine* engine)
{
    b_moveleft.setText(sf::String("Move left: ") + engine->Settings()->controls()->getKeyName("left"));
    b_moveright.setText(sf::String("Move right: ") + engine->Settings()->controls()->getKeyName("right"));
    b_jump.setText(sf::String("Jump: ") + engine->Settings()->controls()->getKeyName("jump"));
    b_sneak.setText(sf::String("Sneak: ") + engine->Settings()->controls()->getKeyName("sneak"));
    b_run.setText(sf::String("Run: ") + engine->Settings()->controls()->getKeyName("run"));
    b_layerswap.setText(sf::String("Swap layers: ") + engine->Settings()->controls()->getKeyName("layerswap"));
    b_place.setText(sf::String("Place block: ") + engine->Settings()->controls()->getKeyName("place"));
    b_destroy.setText(sf::String("Destroy block: ") + engine->Settings()->controls()->getKeyName("destroy"));
    b_pick.setText(sf::String("Pick block: ") + engine->Settings()->controls()->getKeyName("pick"));
}

void MenuState::process_input(GameEngine* engine)
{
    sf::Event event;
    while (engine->app.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            if (engine->isPaused())
            {
                engine->leaveGame(2);
                engine->popState();
                engine->popState();
            }
            else
            {
                engine->quit();
            }
        }
        else if (event.type == sf::Event::Resized and not engine->Settings()->m_fullscreen)
        {
            unsigned int width = (event.size.width > 640) ? event.size.width : 640;
            unsigned int height = (event.size.height > 480) ? event.size.height : 480;
            engine->setResolution(sf::Vector2u(width, height));
            setAllPositions(engine);
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape and m_submenu != MENU_OPTIONS_CONTROLS_CHANGE)
            {
                if (engine->isPaused())
                    engine->popState();
                else
                    m_submenu = MENU_MAINMENU;
            }

            if (event.key.code == sf::Keyboard::F11)
            {
                if (engine->Settings()->m_fullscreen)
                    engine->setResolution(sf::Vector2u(800,480));
                else
                {
                    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
                    engine->setResolution(sf::Vector2u(desktop.width, desktop.height), sf::Style::Fullscreen);
                }
                engine->Settings()->m_fullscreen = not engine->Settings()->m_fullscreen;
            }
        }

        if (m_submenu == MENU_MAINMENU)
        {
            b_singleplayer.process_input(event);
            b_multiplayer.process_input(event);
            b_options.process_input(event);
            b_quit.process_input(event);
        }
        else if (m_submenu == MENU_LOADWORLD)
        {
            b_back.process_input(event);
            b_world1.process_input(event);
            b_world2.process_input(event);
            b_world3.process_input(event);
            b_world4.process_input(event);
            b_world5.process_input(event);
        }
        else if (m_submenu == MENU_MULTIPLAYER)
        {
            b_back.process_input(event);
            b_connect.process_input(event);
        }
        else if (m_submenu == MENU_OPTIONS)
        {
            b_back.process_input(event);
            b_options_player.process_input(event);
            b_options_controls.process_input(event);
            b_options_graphics.process_input(event);
        }
        else if (m_submenu == MENU_OPTIONS_PLAYER)
        {
            if (input_playername.process_input(event) == 1) // character typed
                sprintf(engine->Settings()->m_playername, "%s", input_playername.getString().toAnsiString().c_str());
            if (input_playerskin.process_input(event) == 2) // enter key
            {
                const char *newskin = input_playerskin.getString().toAnsiString().c_str();
                char skin[64];
                sprintf(skin, "data/skins/%s.png", newskin);
                if (txt_playerskin.loadFromFile(skin)) //success
                    sprintf(engine->Settings()->m_playerskin, "%s", newskin);
                else
                    input_playerskin.setText(engine->Settings()->m_playerskin);
            }
            b_back_options.process_input(event);
        }
        else if (m_submenu == MENU_OPTIONS_GRAPHICS)
        {
            b_back_options.process_input(event);
        }
        else if (m_submenu == MENU_OPTIONS_CONTROLS)
        {
            b_moveleft.process_input(event);
            b_moveright.process_input(event);
            b_jump.process_input(event);
            b_sneak.process_input(event);
            b_run.process_input(event);
            b_layerswap.process_input(event);
            b_place.process_input(event);
            b_destroy.process_input(event);
            b_pick.process_input(event);
            b_back_options.process_input(event);
        }
        else if (m_submenu == MENU_OPTIONS_CONTROLS_CHANGE)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code != sf::Keyboard::Escape)
                    engine->Settings()->controls()->setKeyBind(m_changingControl, event.key.code);
                m_submenu = MENU_OPTIONS_CONTROLS;
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                engine->Settings()->controls()->setMouseBind(m_changingControl, event.mouseButton.button);
                m_submenu = MENU_OPTIONS_CONTROLS;
            }
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
        {
            if (not engine->isPaused())
                m_submenu = MENU_MAINMENU;
            else
                engine->popState();
        }
        if (b_options_player.update())
            m_submenu = MENU_OPTIONS_PLAYER;
        if (b_options_controls.update())
            m_submenu = MENU_OPTIONS_CONTROLS;
        if (b_options_graphics.update())
            m_submenu = MENU_OPTIONS_GRAPHICS;
    }
    else if (m_submenu == MENU_OPTIONS_PLAYER)
    {
        input_playername.update();
        input_playerskin.update();
        if (b_back_options.update())
            m_submenu = MENU_OPTIONS;
    }
    else if (m_submenu == MENU_OPTIONS_GRAPHICS)
    {
        if (b_back_options.update())
            m_submenu = MENU_OPTIONS;
    }
    else if (m_submenu == MENU_OPTIONS_CONTROLS)
    {
        if (b_moveleft.update())
            changeBind("left");
        if (b_moveright.update())
            changeBind("right");
        if (b_jump.update())
            changeBind("jump");
        if (b_sneak.update())
            changeBind("sneak");
        if (b_run.update())
            changeBind("run");
        if (b_layerswap.update())
            changeBind("layerswap");
        if (b_place.update())
            changeBind("place");
        if (b_destroy.update())
            changeBind("destroy");
        if (b_pick.update())
            changeBind("pick");
        if (b_back_options.update())
            m_submenu = MENU_OPTIONS;
    }
}

void MenuState::draw(GameEngine* engine)
{
    engine->m_window.draw(dirt_tile);
    if (m_submenu == MENU_MAINMENU)
    {
        engine->m_window.draw(minecraft_logo);
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
        b_options_player.draw();
        b_options_graphics.draw();
        b_options_controls.draw();
        b_back.draw();
    }
    else if (m_submenu == MENU_OPTIONS_PLAYER)
    {
        sf::Sprite skin(txt_playerskin);
        skin.setPosition((engine->app.getSize().x/2)-128, 96+112+48);
        skin.setScale(4, 4);

        label_playername.draw();
        input_playername.draw();
        label_playerskin.draw();
        input_playerskin.draw();
        engine->m_window.draw(skin);
        b_back_options.draw();
    }
    else if (m_submenu == MENU_OPTIONS_GRAPHICS)
    {
        b_back_options.draw();
    }
    else if (m_submenu == MENU_OPTIONS_CONTROLS)
    {
        b_moveleft.draw();
        b_moveright.draw();
        b_jump.draw();
        b_sneak.draw();
        b_run.draw();
        b_layerswap.draw();
        b_place.draw();
        b_destroy.draw();
        b_pick.draw();
        b_back_options.draw();
    }
    else if (m_submenu == MENU_OPTIONS_CONTROLS_CHANGE)
        l_pressakey.draw();
}

void MenuState::pause()
{

}

void MenuState::resume()
{

}

void MenuState::changeBind(const char* keybind)
{
    char aBuf[192];
    sprintf(m_changingControl, "%s", keybind);
    sprintf(aBuf, "Press any key to change control \"%s\".\nPress ESC to cancel.", keybind);
    l_pressakey.setText(aBuf);
    m_submenu = MENU_OPTIONS_CONTROLS_CHANGE;
}
