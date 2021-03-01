#include "title_screen.h"

#include <fstream>

#include "../menu_state.h"
#include "singleplayer.h"

TitleScreenSubmenu::TitleScreenSubmenu(GameEngine* engine, MenuState *menu) : Submenu(engine, menu)
{
    m_allowEsc = false;
    sf::Vector2u windowsize = engine->app.getSize();

    // minecraft logo
    m_minecraft_logo.loadFromFile("data/gui/title/minecraft.png");
    spr_minecraft_logo.setTexture(m_minecraft_logo);
    spr_minecraft_logo.setPosition((windowsize.x/2) - 274.0f, (windowsize.y/4)-64);
    spr_minecraft_logo.setScale(2.0f, 2.0f);

    // main menu 2d panorama (that's called a parallax...)
    char bgfile[128];
    sprintf(bgfile, "data/gui/background/%d.png", rand() % 3); // pick from 3 different backgrounds;
    m_parallax_bg.loadFromFile(bgfile);
    m_parallax_bg.setRepeated(true);
    vtx_parallax_bg.setPrimitiveType(sf::Quads);
    vtx_parallax_bg.resize(4);
    vtx_parallax_bg[0].position = sf::Vector2f(0, 0);
    vtx_parallax_bg[1].position = sf::Vector2f(windowsize.x, 0);
    vtx_parallax_bg[2].position = sf::Vector2f(windowsize.x, windowsize.y);
    vtx_parallax_bg[3].position = sf::Vector2f(0, windowsize.y);
    m_parallax_x = 0.f;

    m_splashscale = 0.2;
    m_splashscaledir = 0.002;

    m_splashtext = new Label(engine, "splash", 0, 0, 0, 0);
    m_splashtext->setHAlign(1);
    m_splashtext->setColor(sf::Color::Yellow);
    m_splashtext->setRotation(-20);
    m_splashtext->setScale(0.2);
    sf::Vector2f aPos = spr_minecraft_logo.getPosition();
    m_splashtext->setPosition(aPos.x+384+88, aPos.y+88+m_splashtext->getText().getSize());
    setSplashText();

    m_versioninfo = new Label(engine, "Minecraft 2D 0.2", 2, windowsize.y-2, 0, 2);
    m_fanmadeproject = new Label(engine, "Fanmade project", windowsize.x-2, windowsize.y-2, 2, 2);

    btn_singleplayer = new Button(engine, sf::String("Singleplayer"), (windowsize.x/2)-200, windowsize.y/4+64);
    btn_multiplayer = new Button(engine, sf::String("Multiplayer"), (windowsize.x/2)-200, btn_singleplayer->getPos().y + 64);
    btn_options = new Button(engine, sf::String("Options"), (windowsize.x/2)-200, btn_multiplayer->getPos().y + 64);
    btn_quit = new Button(engine, sf::String("Quit"), (windowsize.x/2)-200, btn_options->getPos().y + 64);

    btn_singleplayer->onClicked(&onSinglePlayerClicked, this);
    btn_quit->onClicked(&onQuitClicked, this);

    btn_multiplayer->setDisabled(true); // for now...
}

TitleScreenSubmenu::~TitleScreenSubmenu()
{
    m_splashtexts.clear();
    delete m_splashtext;
    delete m_versioninfo;
    delete m_fanmadeproject;
    delete btn_singleplayer;
    delete btn_multiplayer;
    delete btn_options;
    delete btn_quit;
}

void TitleScreenSubmenu::onSinglePlayerClicked(void* pUserData)
{
    TitleScreenSubmenu* self = (TitleScreenSubmenu*)pUserData;

    SingleplayerSubmenu* newstate = new SingleplayerSubmenu(self->m_engine, self->m_menu);
    self->m_menu->changeSubmenu(newstate);
}

void TitleScreenSubmenu::onQuitClicked(void* pUserData)
{
    TitleScreenSubmenu* self = (TitleScreenSubmenu*)pUserData;

    self->m_engine->quit();
}

void TitleScreenSubmenu::setSplashText()
{
    m_splashtexts.clear();
    std::ifstream texts = std::ifstream("data/splashes.txt");
    char major[16];
    char minor[16];
    sprintf(major, "%d", m_engine->app.getSettings().majorVersion);
    sprintf(minor, "%d", m_engine->app.getSettings().minorVersion);
    if (texts.good())
    {
        std::string line;
        while (getline(texts, line))
        {
            // "OpenGL [M].[m]" splashtext
            std::size_t pos_major = line.find("[M]");
            if (pos_major != std::string::npos)
                line.replace(pos_major, 3, major);
            std::size_t pos_minor = line.find("[m]");
            if (pos_minor != std::string::npos)
                line.replace(pos_minor, 3, minor);

            m_splashtexts.push_back(line);
        }
    }
    else
        m_splashtexts.push_back(std::string("splashes.txt broken go home"));

    m_splashtext->setText(m_splashtexts[rand() % m_splashtexts.size()].c_str());
}

void TitleScreenSubmenu::onResolutionChange(sf::Vector2u res)
{

}

void TitleScreenSubmenu::update(float delta)
{
    m_splashscale += m_splashscaledir * (MAX_FPS * delta);
    if (m_splashscale >= 0.215)
    {
        m_splashscale = 0.215;
        m_splashscaledir = -0.001;
    }
    else if (m_splashscale < 0.2)
    {
        m_splashscale = 0.2;
        m_splashscaledir = 0.001;
    }
    m_splashtext->setScale(m_splashscale);
    m_parallax_x += 1.0f * (MAX_FPS * delta);
    if (m_parallax_x >= m_parallax_bg.getSize().x)
        m_parallax_x = 0;
    vtx_parallax_bg[0].texCoords = sf::Vector2f(m_parallax_x, 0);
    vtx_parallax_bg[1].texCoords = sf::Vector2f(m_parallax_x + m_engine->app.getSize().x, 0);
    vtx_parallax_bg[2].texCoords = sf::Vector2f(m_parallax_x + m_engine->app.getSize().x, m_parallax_bg.getSize().y);
    vtx_parallax_bg[3].texCoords = sf::Vector2f(m_parallax_x, m_parallax_bg.getSize().y);

    btn_singleplayer->update();
    btn_multiplayer->update();
    btn_options->update();
    btn_quit->update();
}

void TitleScreenSubmenu::process_input(sf::Event& event)
{
    btn_singleplayer->process_input(event);
    btn_multiplayer->process_input(event);
    btn_options->process_input(event);
    btn_quit->process_input(event);
}

void TitleScreenSubmenu::draw()
{
    m_engine->m_window.draw(vtx_parallax_bg, &m_parallax_bg);
    m_engine->m_window.draw(spr_minecraft_logo);

    m_splashtext->draw();
    m_versioninfo->draw();
    m_fanmadeproject->draw();

    btn_singleplayer->draw();
    btn_multiplayer->draw();
    btn_options->draw();
    btn_quit->draw();
}
