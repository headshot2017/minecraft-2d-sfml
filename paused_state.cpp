#include "paused_state.h"
#include "menu_state.h"

PausedState PausedState::m_Instance;

void PausedState::init(GameEngine* engine)
{
    m_engine = engine;

    m_gamescreen.resize(4);
    m_gamescreen.setPrimitiveType(sf::Quads);
    m_gamescreen[0].position = m_gamescreen[0].texCoords = sf::Vector2f(0, 0);
    m_gamescreen[1].position = m_gamescreen[1].texCoords = sf::Vector2f(800, 0);
    m_gamescreen[2].position = m_gamescreen[2].texCoords = sf::Vector2f(800, 480);
    m_gamescreen[3].position = m_gamescreen[3].texCoords = sf::Vector2f(0, 480);
    m_gamescreen[0].color = sf::Color(128, 128, 128);
    m_gamescreen[1].color = sf::Color(128, 128, 128);
    m_gamescreen[2].color = sf::Color(128, 128, 128);
    m_gamescreen[3].color = sf::Color(128, 128, 128);

    b_resume = Button(engine, sf::String("Back to Game"), 400-200, 240-96);
    b_options = Button(engine, sf::String("Options"), 400-200, 240);
    b_quit = Button(engine, sf::String("Save and Quit to title"), 400-200, 240+48);
}

void PausedState::destroy()
{

}

void PausedState::update(GameEngine* engine)
{
    if (b_resume.update())
        engine->popState();
    if (b_options.update())
        engine->pushState(MenuState::Instance());
    if (b_quit.update())
    {
        engine->leaveGame(1);
        engine->popState();
    }
}

void PausedState::process_input(GameEngine* engine)
{
    sf::Event event;
    while (engine->app.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            engine->leaveGame(2);
            engine->popState();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
                engine->popState();
        }

        b_resume.process_input(event);
        b_options.process_input(event);
        b_quit.process_input(event);
    }
}

void PausedState::draw(GameEngine* engine)
{
    engine->m_window.draw(m_gamescreen, &engine->m_screenshot);
    b_resume.draw();
    b_options.draw();
    b_quit.draw();
}

void PausedState::pause()
{

}

void PausedState::resume()
{

}
