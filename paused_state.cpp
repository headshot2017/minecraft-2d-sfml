#include "paused_state.h"
#include "menu_state.h"

PausedState::PausedState(GameEngine* engine) : GameState(engine)
{
    m_engine = engine;

    sf::Vector2u res = engine->app.getSize();

    m_gamescreen = engine->getGUIWindow(sf::Vector2f(res.x, res.y));
    m_gamescreen[0].texCoords = sf::Vector2f(0, 0);
    m_gamescreen[1].texCoords = sf::Vector2f(res.x, 0);
    m_gamescreen[2].texCoords = sf::Vector2f(res.x, res.y);
    m_gamescreen[3].texCoords = sf::Vector2f(0, res.y);
    m_gamescreen[0].color = sf::Color(128, 128, 128);
    m_gamescreen[1].color = sf::Color(128, 128, 128);
    m_gamescreen[2].color = sf::Color(128, 128, 128+64);
    m_gamescreen[3].color = sf::Color(128, 128, 128+64);

    b_resume = Button(engine, sf::String("Back to Game"), res.x/2-200, res.y/2-96);
    b_options = Button(engine, sf::String("Options"), res.x/2-200, res.y/2);
    b_quit = Button(engine, sf::String("Save and Quit to title"), res.x/2-200, res.y/2+48);
}

PausedState::~PausedState()
{

}

void PausedState::update(float delta)
{
    if (m_engine->leavingGame() == 2)
        m_engine->popState();

    /*
    if (b_resume.update())
        engine->popState();
    if (b_options.update())
        engine->pushState(MenuState::Instance());
    if (b_quit.update())
    {
        engine->leaveGame(1);
        engine->popState();
    }
    */
}

void PausedState::event_input(sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
            m_engine->popState();
    }

    b_resume.process_input(event);
    b_options.process_input(event);
    b_quit.process_input(event);
}

void PausedState::draw()
{
    m_engine->m_window.draw(m_gamescreen, &m_engine->m_screenshot);
    b_resume.draw();
    b_options.draw();
    b_quit.draw();
}

void PausedState::pause()
{

}

void PausedState::resume()
{
    onResolutionChange(m_engine->app.getSize());
}

void PausedState::onResolutionChange(sf::Vector2u res)
{
    m_gamescreen[0].position = m_gamescreen[0].texCoords = sf::Vector2f(0, 0);
    m_gamescreen[1].position = m_gamescreen[1].texCoords = sf::Vector2f(res.x, 0);
    m_gamescreen[2].position = m_gamescreen[2].texCoords = sf::Vector2f(res.x, res.y);
    m_gamescreen[3].position = m_gamescreen[3].texCoords = sf::Vector2f(0, res.y);
    b_resume.setPosition(res.x/2-200, res.y/2-96);
    b_options.setPosition(res.x/2-200, res.y/2);
    b_quit.setPosition(res.x/2-200, res.y/2+48);
}
