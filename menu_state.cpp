#include <algorithm>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>

#include <SFML/Graphics.hpp>

#include "game_engine.h"
#include "menu_state.h"
#include "ingame_state.h"
#include "submenus/submenu.h"
#include "submenus/title_screen.h"
#include "bass.h"

void tokenize(std::string const &str, const char* delim,
			std::vector<std::string> &out)
{
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
}

bool is_folder(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

/*void listSoundThemes(std::vector<std::string>& strvec)
{
    DIR *dir;
    struct dirent *entry;
    strvec.clear();

    if ((dir = opendir("data/sounds")))
    {
        while ((entry = readdir(dir)))
        {
            std::string theme(entry->d_name);
            if (theme != "." and theme != "..")
            {
                strvec.push_back(theme);
            }
        }
    }
}*/

void listSoundDevices(std::vector<std::string>& strvec)
{
    strvec.clear();

    BASS_DEVICEINFO info;
    DWORD ind = 0;
    while (BASS_GetDeviceInfo(ind, &info))
    {
        strvec.push_back(info.name);
        ind++;
    }
}

int find_in_vector(std::vector<std::string> &strvec, const char *_element)
{
    std::string element(_element);

    std::vector<std::string>::iterator it = std::find(strvec.begin(), strvec.end(), element);
    if (it == strvec.end()) return -1;

    return std::distance(strvec.begin(), it);
}

MenuState::MenuState(GameEngine* engine) : GameState(engine)
{
    m_engine = engine;
    srand(time(0));

    m_submenu = (engine->isPaused()) ? new TitleScreenSubmenu(engine, this) : new TitleScreenSubmenu(engine, this);

    m_musicticks = 60*5;

    sf::Vector2u windowsize = engine->app.getSize();

    m_dirt_tile.loadFromFile("data/gui/options_background.png");
    m_dirt_tile.setRepeated(true);



    dirt_tile.setTexture(m_dirt_tile);
    dirt_tile.setScale(4.0f, 4.0f);
    dirt_tile.setColor(sf::Color(128, 128, 128));
    dirt_tile.setTextureRect(sf::IntRect(0, 0, windowsize.x/4, windowsize.y/4));

    m_gamescreen = engine->getGUIWindow(sf::Vector2f(windowsize.x, windowsize.y));
    m_gamescreen[0].texCoords = sf::Vector2f(0, 0);
    m_gamescreen[1].texCoords = sf::Vector2f(windowsize.x, 0);
    m_gamescreen[2].texCoords = sf::Vector2f(windowsize.x, windowsize.y);
    m_gamescreen[3].texCoords = sf::Vector2f(0, windowsize.y);
    m_gamescreen[0].color = sf::Color(128, 128, 128);
    m_gamescreen[1].color = sf::Color(128, 128, 128);
    m_gamescreen[2].color = sf::Color(128, 128, 128+64);
    m_gamescreen[3].color = sf::Color(128, 128, 128+64);




    /*
    b_back = Button(engine, sf::String("Back"), (windowsize.x/2)-200, windowsize.y-48);
    b_back_options = Button(engine, sf::String("Back"), (windowsize.x/2)-200, windowsize.y-48);

    b_singleplayer = Button(engine, sf::String("Singleplayer"), (windowsize.x/2)-200, windowsize.y/4+64);
    b_multiplayer = Button(engine, sf::String("Multiplayer"), (windowsize.x/2)-200, b_singleplayer.getPos().y + 64);
    b_options = Button(engine, sf::String("Options"), (windowsize.x/2)-200, b_multiplayer.getPos().y + 64);
    b_quit = Button(engine, sf::String("Quit"), (windowsize.x/2)-200, b_options.getPos().y + 64);

    l_selectworld = Label(engine, "Select World", windowsize.x/2, 48, 1, 1);
    l_worlds = ItemList(engine, (windowsize.x/2)-200, (windowsize.y/4)-32+(48*2), 400, 192, m_worldlist);
    b_playworld = Button(engine, "Play Selected World", (windowsize.x/2)-300-8, windowsize.y-96, 300);
    b_createworld_list = Button(engine, "Create New World", (windowsize.x/2)+8, windowsize.y-96, 300);
    b_renameworld = Button(engine, "Rename", b_playworld.getPos().x, windowsize.y-48, 144);
    b_deleteworld = Button(engine, "Delete", (windowsize.x/2)-144-8, windowsize.y-48, 144);
    b_recreate_world = Button(engine, "Re-Create", (windowsize.x/2)+8, windowsize.y-48, 144);
    b_cancel_list = Button(engine, "Cancel", b_recreate_world.getPos().x+144+12, windowsize.y-48, 144);

    label_createworld = Label(engine, "Create New World", windowsize.x/2, 48, 1, 1);
    label_worldname = Label(engine, "World name", (windowsize.x/2)-200, 96);
    input_worldname = TextInput(engine, "New World", sf::Vector2f((windowsize.x/2)-200, 96+24), 96);
    b_worldbiome = Button(engine, "Biome: Random", (windowsize.x/2)-200, 96+88);
    b_confirmcreate = Button(engine, "Create New World", (windowsize.x/2)-300-8, windowsize.y-48, 300);
    b_cancelcreate = Button(engine, "Cancel", (windowsize.x/2)+8, windowsize.y-48, 300);

    superflat_blocks.push_back(BLOCK_BEDROCK);
    superflat_blocks.push_back(BLOCK_BEDROCK);
    for(int i=0; i<5; i++) superflat_blocks.push_back(BLOCK_STONE);
    for(int i=0; i<3; i++) superflat_blocks.push_back(BLOCK_DIRT);
    superflat_blocks.push_back(BLOCK_GRASS);

    label_renameworld = Label(engine, "Rename World", windowsize.x/2, 48, 1, 1);
    b_cancelrename = Button(engine, "Cancel", windowsize.x/2-200, windowsize.y-40-32);
    b_confirmrename = Button(engine, "Rename", windowsize.x/2-200, b_cancelrename.getPos().y-48);

    std::vector<std::string> test;
    for (int i=0; i<30; i++)
    {
        char aBuf[8];
        sprintf(aBuf, "%d", i);
        test.push_back(std::string(aBuf));
    }
    b_connect = Button(engine, sf::String("Connect"), (windowsize.x/2)-200, (windowsize.y/4)-32+64);
    l_test = ItemList(engine, (windowsize.x/2)-150, (windowsize.y/4)-32+128, 300, 192, test);

    b_options_player = Button(engine, "Player", (windowsize.x/2)-200, 128+(48*0));
    b_options_graphics = Button(engine, "Graphics", (windowsize.x/2)-200, 128+(48*1));
    b_options_sound = Button(engine, "Sound", (windowsize.x/2)-200, 128+(48*2));
    b_options_controls = Button(engine, "Controls", (windowsize.x/2)-200, 128+(48*3));

    label_playername = Label(engine, "Player name", (windowsize.x/2)-200, 96);
    input_playername = TextInput(engine, engine->Settings()->m_playername, sf::Vector2f((windowsize.x/2)-200, 96+24), 24);
    label_playerskin = Label(engine, "Player skin", (windowsize.x/2)-200, 96+88);
    input_playerskin = TextInput(engine, engine->Settings()->m_playerskin, sf::Vector2f((windowsize.x/2)-200, 96+112), 64);
    char skin[64];
    sprintf(skin, "data/skins/%s.png", input_playerskin.getString().toAnsiString().c_str());
    txt_playerskin.loadFromFile(skin);

    b_moveleft = Button(engine, "Move left: ", (windowsize.x/2)-300-8, 64, 300);
    b_moveright = Button(engine, "Move right: ", (windowsize.x/2)-300-8, 64+48, 300);
    b_jump = Button(engine, "Jump: ", (windowsize.x/2)-300-8, 64+(48*2), 300);
    b_sneak = Button(engine, "Sneak: ", (windowsize.x/2)-300-8, 64+(48*3), 300);
    b_run = Button(engine, "Run: ", (windowsize.x/2)-300-8, 64+(48*4), 300);
    b_inventory = Button(engine, "Inventory: ", (windowsize.x/2)-300-8, 64+(48*5), 300);
    b_layerswap = Button(engine, "Swap layers: ", (windowsize.x/2)-300-8, 64+(48*6), 300);
    b_place = Button(engine, "Place block: ", (windowsize.x/2)+8, 64+(48*0), 300);
    b_destroy = Button(engine, "Destroy block: ", (windowsize.x/2)+8, 64+(48*1), 300);
    b_pick = Button(engine, "Pick block: ", (windowsize.x/2)+8, 64+(48*2), 300);
    b_drop = Button(engine, "Drop item: ", (windowsize.x/2)+8, 64+(48*3), 300);
    b_screenshot = Button(engine, "Screenshot: ", (windowsize.x/2)+8, 64+(48*4), 300);
    b_fullscreen_control = Button(engine, "Fullscreen: ", (windowsize.x/2)+8, 64+(48*5), 300);
    b_layer1_collide = Button(engine, "Layer 1 collisions: ", (windowsize.x/2)+8, 64+(48*6), 300);

    l_pressakey = Label(engine, "", windowsize.x/2, windowsize.y/2-48, 1);

    std::vector<sf::VideoMode> modes = engine->getResolutions();

    fullscreen = engine->Settings()->m_fullscreen;
    s_videores = Slider(engine, "Resolution:", (windowsize.x/2)-300-8, 64+(48*0), 300);
    s_videores.setMaxValue(modes.size()-1);
    s_videores.setFloatValue(false);
    s_videores.setAutoUpdateText(false);
    b_fullscreen = Button(engine, "Fullscreen:", (windowsize.x/2)+8, 64+(48*0), 300);
    b_applyvideo = Button(engine, "Apply", (windowsize.x/2)-200, windowsize.y-112);

    l_devices = ItemList(engine, (windowsize.x/2)-300-8, 64+(48*0), 616, 96, m_devicelist, "Audio device");
    s_mastervol = Slider(engine, "Master Volume:", (windowsize.x/2)-300-8, 64+(48*2)+16, 616);
    s_mastervol.setMaxValue(100);
    s_mastervol.setValue(engine->Settings()->m_mastervol);
    s_mastervol.setFloatValue(false);
    s_musicvol = Slider(engine, "Music Volume:", (windowsize.x/2)-300-8, 64+(48*3)+16, 300);
    s_musicvol.setMaxValue(100);
    s_musicvol.setValue(engine->Settings()->m_musicvol);
    s_musicvol.setFloatValue(false);
    s_soundvol = Slider(engine, "Sound Volume:", (windowsize.x/2)+8, 64+(48*3)+16, 300);
    s_soundvol.setMaxValue(100);
    s_soundvol.setValue(engine->Settings()->m_soundvol);
    s_soundvol.setFloatValue(false);
    l_soundthemes = ItemList(engine, (windowsize.x/2)-300-8, 64+(48*4)+20+48, 616, 96, m_soundthemes, "Sound theme");

    setAllPositions(windowsize);
    */
}

MenuState::~MenuState()
{
    delete m_submenu;
    m_submenu = 0;
}

/*
void MenuState::setAllPositions(sf::Vector2u& windowsize)
{
    minecraft_logo.setPosition((windowsize.x/2) - 274.0f, (windowsize.y/4)-64);
    sf::Vector2f aPos = minecraft_logo.getPosition();
    m_splashtext.setPosition(aPos.x+384+88, aPos.y+88+m_splashtext.getText().getSize());
    parallax_bg[0].position = sf::Vector2f(0, 0);
    parallax_bg[1].position = sf::Vector2f(windowsize.x, 0);
    parallax_bg[2].position = sf::Vector2f(windowsize.x, windowsize.y);
    parallax_bg[3].position = sf::Vector2f(0, windowsize.y);

    m_versioninfo.setPosition(2, windowsize.y-2);
    m_fanmadeproject.setPosition(windowsize.x-2, windowsize.y-2);

    b_back.setPosition((windowsize.x/2)-200, windowsize.y-48);
    b_back_options.setPosition((windowsize.x/2)-200, windowsize.y-48);

    b_singleplayer.setPosition((windowsize.x/2)-200, windowsize.y/4+96);
    b_multiplayer.setPosition((windowsize.x/2)-200, b_singleplayer.getPos().y + 64);
    b_options.setPosition((windowsize.x/2)-200, b_multiplayer.getPos().y + 64);
    b_quit.setPosition((windowsize.x/2)-200, b_options.getPos().y + 64);

    l_worlds.setPosition(64, (windowsize.y/4)-32);
    l_worlds.setSize(windowsize.x-128, b_back.getPos().y - l_worlds.getPosition().y - 64);
    l_selectworld.setPosition(windowsize.x/2, 48);
    b_playworld.setPosition((windowsize.x/2)-300-8, windowsize.y-96);
    b_createworld_list.setPosition((windowsize.x/2)+8, windowsize.y-96);
    b_renameworld.setPosition(b_playworld.getPos().x, windowsize.y-48);
    b_deleteworld.setPosition((windowsize.x/2)-144-8, windowsize.y-48);
    b_recreate_world.setPosition((windowsize.x/2)+8, windowsize.y-48);
    b_cancel_list.setPosition(b_recreate_world.getPos().x+144+12, windowsize.y-48);

    label_createworld.setPosition(windowsize.x/2, 48);
    label_worldname.setPosition((windowsize.x/2)-200, 96);
    input_worldname.setPosition((windowsize.x/2)-200, 96+24);
    b_worldbiome.setPosition((windowsize.x/2)-200, 96+88);
    b_confirmcreate.setPosition((windowsize.x/2)-300-8, windowsize.y-48);
    b_cancelcreate.setPosition((windowsize.x/2)+8, windowsize.y-48);

    label_confirm_delete.setPosition(windowsize.x/2, windowsize.y/2-96);
    b_delete_yes.setPosition((windowsize.x/2)-300-8, windowsize.y/2+16);
    b_delete_no.setPosition((windowsize.x/2)+8, windowsize.y/2+16);

    label_renameworld.setPosition(windowsize.x/2, 48);
    b_cancelrename.setPosition(windowsize.x/2-200, windowsize.y-40-32);
    b_confirmrename.setPosition(windowsize.x/2-200, b_cancelrename.getPos().y-48);

    b_connect.setPosition((windowsize.x/2)-200, (windowsize.y/4)-32+64);
    l_test.setPosition((windowsize.x/2)-150, (windowsize.y/4)-32+128);

    b_options_player.setPosition((windowsize.x/2)-200, 128);
    b_options_graphics.setPosition((windowsize.x/2)-200, 128+48);
    b_options_sound.setPosition((windowsize.x/2)-200, 128+(48*2));
    b_options_controls.setPosition((windowsize.x/2)-200, 128+(48*3));

    label_playername.setPosition((windowsize.x/2)-200, 96);
    input_playername.setPosition((windowsize.x/2)-200, 96+24);
    label_playerskin.setPosition((windowsize.x/2)-200, 96+88);
    input_playerskin.setPosition((windowsize.x/2)-200, 96+112);

    b_moveleft.setPosition((windowsize.x/2)-300-8, 64);
    b_moveright.setPosition((windowsize.x/2)-300-8, 64+48);
    b_jump.setPosition((windowsize.x/2)-300-8, 64+(48*2));
    b_sneak.setPosition((windowsize.x/2)-300-8, 64+(48*3));
    b_run.setPosition((windowsize.x/2)-300-8, 64+(48*4));
    b_inventory.setPosition((windowsize.x/2)-300-8, 64+(48*5));
    b_layerswap.setPosition((windowsize.x/2)-300-8, 64+(48*6));
    b_place.setPosition((windowsize.x/2)+8, 64+(48*0));
    b_destroy.setPosition((windowsize.x/2)+8, 64+(48*1));
    b_pick.setPosition((windowsize.x/2)+8, 64+(48*2));
    b_drop.setPosition((windowsize.x/2)+8, 64+(48*3));
    b_screenshot.setPosition((windowsize.x/2)+8, 64+(48*4));
    b_fullscreen_control.setPosition((windowsize.x/2)+8, 64+(48*5));
    b_layer1_collide.setPosition((windowsize.x/2)+8, 64+(48*6));

    l_devices.setPosition((windowsize.x/2)-300-8, 64+(48*0));
    s_mastervol.setPosition((windowsize.x/2)-300-8, 64+(48*2)+16);
    s_musicvol.setPosition((windowsize.x/2)-300-8, 64+(48*3)+16);
    s_soundvol.setPosition((windowsize.x/2)+8, 64+(48*3)+16);
    l_soundthemes.setPosition((windowsize.x/2)-300-8, 64+(48*4)+20+48);

    l_pressakey.setPosition(windowsize.x/2, windowsize.y/2-48);

    s_videores.setPosition((windowsize.x/2)-300-8, 64+(48*0));
    b_fullscreen.setPosition((windowsize.x/2)+8, 64+(48*0));
    b_applyvideo.setPosition((windowsize.x/2)-200, windowsize.y-112);
}*/

void MenuState::update(float delta)
{
    /*
    char aBuf[128];


    b_moveleft.setText(sf::String("Move left: ") + engine->Settings()->controls()->getKeyName("left"));
    b_moveright.setText(sf::String("Move right: ") + engine->Settings()->controls()->getKeyName("right"));
    b_jump.setText(sf::String("Jump: ") + engine->Settings()->controls()->getKeyName("jump"));
    b_sneak.setText(sf::String("Sneak: ") + engine->Settings()->controls()->getKeyName("sneak"));
    b_run.setText(sf::String("Run: ") + engine->Settings()->controls()->getKeyName("run"));
    b_inventory.setText(sf::String("Inventory: ") + engine->Settings()->controls()->getKeyName("inventory"));
    b_layerswap.setText(sf::String("Swap layers: ") + engine->Settings()->controls()->getKeyName("layerswap"));
    b_place.setText(sf::String("Place block: ") + engine->Settings()->controls()->getKeyName("place"));
    b_destroy.setText(sf::String("Destroy block: ") + engine->Settings()->controls()->getKeyName("destroy"));
    b_pick.setText(sf::String("Pick block: ") + engine->Settings()->controls()->getKeyName("pick"));
    b_drop.setText(sf::String("Drop item: ") + engine->Settings()->controls()->getKeyName("drop"));
    b_screenshot.setText(sf::String("Screenshot: ") + engine->Settings()->controls()->getKeyName("screenshot"));
    b_fullscreen_control.setText(sf::String("Fullscreen: ") + engine->Settings()->controls()->getKeyName("fullscreen"));
    b_layer1_collide.setText(sf::String("Layer 1 collisions: ") + engine->Settings()->controls()->getKeyName("layer1_collide"));

    sf::VideoMode mode = engine->getResolutions()[s_videores.getValue()];
    sprintf(aBuf, "Resolution: %dx%d", mode.width, mode.height);
    s_videores.setText(aBuf);

    //sprintf(aBuf, "Master Volume: %d%%", (int)s_mastervol.getValue());
    //s_mastervol.setText(aBuf);
    //sprintf(aBuf, "Music Volume: %d%%", (int)s_musicvol.getValue());
    //s_musicvol.setText(aBuf);
    //sprintf(aBuf, "Sound Volume: %d%%", (int)s_soundvol.getValue());
    //s_soundvol.setText(aBuf);

    sprintf(aBuf, "Fullscreen: %s", fullscreen ? "YES" : "NO");
    b_fullscreen.setText(aBuf);
    */

    if (not m_engine->isPaused())
    {
        m_musicticks--;
        if (not m_musicticks)
        {
            m_engine->Sound()->playMusic(MUSIC_MENU1 + (rand() % 4));
            HSTREAM menu_music = m_engine->Sound()->getCurrentMusic();
            m_musicticks = 60 * (BASS_ChannelBytes2Seconds(menu_music, BASS_ChannelGetLength(menu_music, BASS_POS_BYTE)) + 5);
        }
    }

    m_submenu->update(delta);
}

void MenuState::process_input()
{

}

void MenuState::event_input(sf::Event& event)
{
    if (event.type == sf::Event::Closed and not m_engine->isPaused())
    {
        m_engine->quit();
    }
    else if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape and m_submenu->m_allowEsc)
        {
            if (m_engine->isPaused())
                m_engine->popState();
            else
            {
                changeSubmenu(new TitleScreenSubmenu(m_engine, this));
            }
        }
        else if (event.key.code == sf::Keyboard::F5)
            m_musicticks = 1;
    }

    m_submenu->process_input(event);

    /*
    if (m_submenu == MENU_MAINMENU)
    {
        b_singleplayer.process_input(event);
        b_multiplayer.process_input(event);
        b_options.process_input(event);
        b_quit.process_input(event);
    }
    else if (m_submenu == MENU_LOADWORLD)
    {
        l_worlds.event_input(event);
        b_playworld.process_input(event);
        b_createworld_list.process_input(event);
        b_renameworld.process_input(event);
        b_deleteworld.process_input(event);
        b_recreate_world.process_input(event);
        b_cancel_list.process_input(event);
    }
    else if (m_submenu == MENU_MULTIPLAYER)
    {
        b_back.process_input(event);
        b_connect.process_input(event);
        l_test.event_input(event);
    }

    else if (m_submenu == MENU_CREATEWORLD)
    {
        input_worldname.process_input(event);
        b_worldbiome.process_input(event);
        b_confirmcreate.process_input(event);
        b_cancelcreate.process_input(event);
    }
    else if (m_submenu == MENU_CONFIRMDELETE)
    {
        b_delete_yes.process_input(event);
        b_delete_no.process_input(event);
    }
    else if (m_submenu == MENU_RENAMEWORLD)
    {
        input_worldname.process_input(event);
        b_confirmrename.process_input(event);
        b_cancelrename.process_input(event);
    }

    else if (m_submenu == MENU_OPTIONS)
    {
        b_back.process_input(event);
        b_options_player.process_input(event);
        b_options_controls.process_input(event);
        b_options_graphics.process_input(event);
        b_options_sound.process_input(event);
    }
    else if (m_submenu == MENU_OPTIONS_PLAYER)
    {
        if (input_playername.process_input(event) == 1) // character typed
            sprintf(engine->Settings()->m_playername, "%s", input_playername.getString().toAnsiString().c_str());
        if (input_playerskin.process_input(event) == 2) // enter key
        {
            char skin[64];
            sprintf(skin, "data/skins/%s.png", input_playerskin.getString().toAnsiString().c_str());
            if (txt_playerskin.loadFromFile(skin)) //success
                sprintf(engine->Settings()->m_playerskin, "%s", input_playerskin.getString().toAnsiString().c_str());
            else
                input_playerskin.setText(engine->Settings()->m_playerskin);
        }
        b_back_options.process_input(event);
    }
    else if (m_submenu == MENU_OPTIONS_GRAPHICS)
    {
        s_videores.process_input(event);
        b_fullscreen.process_input(event);
        b_applyvideo.process_input(event);
        b_back_options.process_input(event);
    }
    else if (m_submenu == MENU_OPTIONS_SOUND)
    {
        if (l_soundthemes.event_input(event)) sprintf(engine->Settings()->m_soundtheme, l_soundthemes.getSelectedItem().c_str());
        if (l_devices.event_input(event)) engine->Settings()->m_sounddevice = l_devices.getSelected();
        s_mastervol.process_input(event);
        s_musicvol.process_input(event);
        s_soundvol.process_input(event);
        b_back_options.process_input(event);
    }
    else if (m_submenu == MENU_OPTIONS_CONTROLS)
    {
        b_moveleft.process_input(event);
        b_moveright.process_input(event);
        b_jump.process_input(event);
        b_sneak.process_input(event);
        b_run.process_input(event);
        b_inventory.process_input(event);
        b_layerswap.process_input(event);
        b_place.process_input(event);
        b_destroy.process_input(event);
        b_pick.process_input(event);
        b_drop.process_input(event);
        b_screenshot.process_input(event);
        b_fullscreen_control.process_input(event);
        b_layer1_collide.process_input(event);
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

    // check clicked
    if (m_submenu == MENU_MAINMENU)
    {
        if (b_singleplayer.update())
        {
            m_submenu = MENU_LOADWORLD;
            listWorlds(m_worldlist);
            l_worlds.setItems(m_worldlist);
        }
        if (b_multiplayer.update())
            m_submenu = MENU_MULTIPLAYER;
        if (b_options.update())
            m_submenu = MENU_OPTIONS;
        if (b_quit.update())
            engine->quit();
    }
    else if (m_submenu == MENU_LOADWORLD)
    {
        int world = -1;
        if (b_cancel_list.update())
            m_submenu = MENU_MAINMENU;

        l_worlds.update();
        if (b_playworld.update() and l_worlds.isSelected())
            world = l_worlds.getSelected();
        if (b_createworld_list.update())
        {
            m_submenu = MENU_CREATEWORLD;
            input_worldname.setText("New World");
        }
        if (b_renameworld.update() and l_worlds.isSelected())
        {
            m_submenu = MENU_RENAMEWORLD;
            input_worldname.setText(l_worlds.getSelectedItem().c_str());
        }
        if (b_deleteworld.update() and l_worlds.isSelected())
        {
            m_submenu = MENU_CONFIRMDELETE;
            char aBuf[256];
            sprintf(aBuf, "Are you sure you want to delete this world?\n\n'%s' will be lost forever! (A long time!)", l_worlds.getSelectedItem().c_str());
            label_confirm_delete.setText(aBuf);
        }
        if (b_recreate_world.update() and l_worlds.isSelected())
        {
            m_submenu = MENU_CREATEWORLD;
            input_worldname.setText(l_worlds.getSelectedItem().c_str());
        }

        if (world >= 0) // world selected
        {
            engine->Sound()->stopMusic();
            IngameState* state = IngameState::Instance();
            engine->changeState(state);
            state->loadWorld(l_worlds.getSelectedItem().c_str());
        }
    }
    else if (m_submenu == MENU_MULTIPLAYER)
    {
        if (b_back.update())
            m_submenu = MENU_MAINMENU;
        b_connect.update();
        l_test.update();
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
        if (b_options_sound.update())
        {
            m_submenu = MENU_OPTIONS_SOUND;

            listSoundThemes(m_soundthemes);
            listSoundDevices(m_devicelist);

            l_soundthemes.setItems(m_soundthemes);
            l_soundthemes.setSelected(find_in_vector(m_soundthemes, engine->Sound()->getTheme()));
            l_devices.setItems(m_devicelist);
            l_devices.setSelected((int)BASS_GetDevice());
        }
    }

    else if (m_submenu == MENU_CREATEWORLD)
    {
        input_worldname.update();
        if (b_worldbiome.update())
        {
            selected_biome++;
            if (selected_biome > 4) selected_biome = -1;
            b_worldbiome.setText( (selected_biome == -1) ? "Biome: Random" :
                                  (selected_biome == 0) ? "Biome: Plains" :
                                  (selected_biome == 1) ? "Biome: Plains With Trees" :
                                  (selected_biome == 2) ? "Biome: Desert" :
                                  (selected_biome == 3) ? "Biome: Mixed" :
                                  (selected_biome == 4) ? "Biome: Superflat" :
                                  "Biome: ???" );
        }
        if (b_confirmcreate.update())
        {
            engine->Sound()->stopMusic();
            IngameState* state = IngameState::Instance();
            engine->changeState(state);
            state->loadWorld(input_worldname.getString().toAnsiString().c_str(), true, selected_biome, superflat_blocks);
        }
        if (b_cancelcreate.update())
            m_submenu = MENU_LOADWORLD;
    }
    else if (m_submenu == MENU_CONFIRMDELETE)
    {
        if (b_delete_yes.update())
        {
            char aBuf[256];
            sprintf(aBuf, "worlds/%s.dat", l_worlds.getSelectedItem().c_str());
            remove(aBuf);
            m_submenu = MENU_LOADWORLD;
            listWorlds(m_worldlist);
            l_worlds.setItems(m_worldlist);
        }
        if (b_delete_no.update())
            m_submenu = MENU_LOADWORLD;
    }
    else if (m_submenu == MENU_RENAMEWORLD)
    {
        input_worldname.update();
        if (b_confirmrename.update())
        {
            char aBuf[256], aBuf2[256];
            sprintf(aBuf, "worlds/%s.dat", l_worlds.getSelectedItem().c_str());
            sprintf(aBuf2, "worlds/%s.dat", input_worldname.getString().toAnsiString().c_str());
            rename(aBuf, aBuf2);
            m_submenu = MENU_LOADWORLD;
            listWorlds(m_worldlist);
            l_worlds.setItems(m_worldlist);
        }
        if (b_cancelrename.update())
            m_submenu = MENU_LOADWORLD;
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
        s_videores.update();
        if (b_fullscreen.update())
            fullscreen = not fullscreen;
        if (b_applyvideo.update())
        {
            sf::VideoMode mode = engine->getResolutions()[s_videores.getValue()];
            sf::Uint32 flags = (fullscreen) ? sf::Style::Fullscreen : sf::Style::Close;
            m_engine->Settings()->m_fullscreen = fullscreen;
            engine->setResolution(sf::Vector2u(mode.width, mode.height), flags);
        }
        if (b_back_options.update())
            m_submenu = MENU_OPTIONS;
    }
    else if (m_submenu == MENU_OPTIONS_SOUND)
    {
        l_soundthemes.update();
        l_devices.update();
        if (s_mastervol.update())
        {
            m_engine->Settings()->m_mastervol = (int)s_mastervol.getValue();
            m_engine->Sound()->updateVolume();
        }
        if (s_musicvol.update())
        {
            m_engine->Settings()->m_musicvol = (int)s_musicvol.getValue();
            m_engine->Sound()->updateVolume();
        }
        if (s_soundvol.update())
        {
            m_engine->Settings()->m_soundvol = (int)s_soundvol.getValue();
            m_engine->Sound()->updateVolume();
        }
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
        if (b_inventory.update())
            changeBind("inventory");
        if (b_layerswap.update())
            changeBind("layerswap");
        if (b_place.update())
            changeBind("place");
        if (b_destroy.update())
            changeBind("destroy");
        if (b_pick.update())
            changeBind("pick");
        if (b_drop.update())
            changeBind("drop");
        if (b_screenshot.update())
            changeBind("screenshot");
        if (b_fullscreen_control.update())
            changeBind("fullscreen");
        if (b_layer1_collide.update())
            changeBind("layer1_collide");
        if (b_back_options.update())
            m_submenu = MENU_OPTIONS;
    }
    */
}

void MenuState::draw()
{
    m_dirt_tile.setRepeated(true);
    dirt_tile.setTexture(m_dirt_tile);

    if (m_engine->isPaused())
        m_engine->m_window.draw(m_gamescreen, &m_engine->m_screenshot);
    else
        m_engine->m_window.draw(dirt_tile);

    m_submenu->draw();

    /*
    if (m_submenu == MENU_MAINMENU)
    {

        b_singleplayer.draw();
        b_multiplayer.draw();
        b_options.draw();
        b_quit.draw();
    }
    else if (m_submenu == MENU_LOADWORLD)
    {
        l_selectworld.draw();
        l_worlds.draw();
        b_playworld.draw();
        b_createworld_list.draw();
        b_renameworld.draw();
        b_deleteworld.draw();
        b_recreate_world.draw();
        b_cancel_list.draw();
    }
    else if (m_submenu == MENU_MULTIPLAYER)
    {
        b_connect.draw();
        l_test.draw();
        b_back.draw();
    }
    else if (m_submenu == MENU_OPTIONS)
    {
        b_options_player.draw();
        b_options_graphics.draw();
        b_options_controls.draw();
        b_options_sound.draw();
        b_back.draw();
    }

    else if (m_submenu == MENU_CREATEWORLD)
    {
        label_createworld.draw();
        label_worldname.draw();
        input_worldname.draw();
        b_worldbiome.draw();
        b_confirmcreate.draw();
        b_cancelcreate.draw();
    }
    else if (m_submenu == MENU_CONFIRMDELETE)
    {
        label_confirm_delete.draw();
        b_delete_yes.draw();
        b_delete_no.draw();
    }
    else if (m_submenu == MENU_RENAMEWORLD)
    {
        label_worldname.draw();
        input_worldname.draw();
        b_confirmrename.draw();
        b_cancelrename.draw();
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
        s_videores.draw();
        b_fullscreen.draw();
        b_applyvideo.draw();
        b_back_options.draw();
    }
    else if (m_submenu == MENU_OPTIONS_SOUND)
    {
        l_soundthemes.draw();
        l_devices.draw();
        s_mastervol.draw();
        s_musicvol.draw();
        s_soundvol.draw();
        b_back_options.draw();
    }
    else if (m_submenu == MENU_OPTIONS_CONTROLS)
    {
        b_moveleft.draw();
        b_moveright.draw();
        b_jump.draw();
        b_sneak.draw();
        b_run.draw();
        b_inventory.draw();
        b_layerswap.draw();
        b_place.draw();
        b_destroy.draw();
        b_pick.draw();
        b_drop.draw();
        b_screenshot.draw();
        b_fullscreen_control.draw();
        b_layer1_collide.draw();
        b_back_options.draw();
    }
    else if (m_submenu == MENU_OPTIONS_CONTROLS_CHANGE)
        l_pressakey.draw();
    */
}

void MenuState::pause()
{

}

void MenuState::resume()
{

}

void MenuState::onResolutionChange(sf::Vector2u res)
{
    m_gamescreen[0].position = m_gamescreen[0].texCoords = sf::Vector2f(0, 0);
    m_gamescreen[1].position = m_gamescreen[1].texCoords = sf::Vector2f(res.x, 0);
    m_gamescreen[2].position = m_gamescreen[2].texCoords = sf::Vector2f(res.x, res.y);
    m_gamescreen[3].position = m_gamescreen[3].texCoords = sf::Vector2f(0, res.y);

    fullscreen = m_engine->Settings()->m_fullscreen;
    dirt_tile.setTextureRect(sf::IntRect(0, 0, res.x/4, res.y/4));
}

void MenuState::changeBind(const char* keybind)
{
    /*
    char aBuf[192];
    sprintf(m_changingControl, "%s", keybind);
    sprintf(aBuf, "Press any key to change control \"%s\".\nPress ESC to cancel.", keybind);
    l_pressakey.setText(aBuf);
    m_submenu = MENU_OPTIONS_CONTROLS_CHANGE;
    */
}

void MenuState::changeSubmenu(Submenu* submenu)
{
    delete m_submenu;
    m_submenu = submenu;
}
