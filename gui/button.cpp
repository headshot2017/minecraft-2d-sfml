#include <SFML/Graphics.hpp>
#include "button.h"
#include "../bass.h"
#include "../game_engine.h"
#include <stdio.h>

Button::Button()
{

}

Button::Button(GameEngine* _engine, sf::String text, sf::Vector2f pos)
{
    engine = _engine;
    m_pos = pos;

    m_text.setString(text);
    m_text.setFont(engine->mc_font);
    m_text.setScale(0.25f, 0.25f);
    m_text.setCharacterSize(96);
    m_text.setPosition(m_pos.x + 200 - (m_text.getGlobalBounds().width/2.0f),
                       m_pos.y + 17 - (m_text.getGlobalBounds().height/2.0f));

    m_sprite = engine->button;
    m_sprite.setPosition(m_pos);
}

Button::Button(GameEngine* _engine, sf::String text, float X, float Y)
{
    engine = _engine;
    m_pos = sf::Vector2f(X, Y);

    m_text.setString(text);
    m_text.setFont(engine->mc_font);
    m_text.setScale(0.16667f, 0.16667f);
    m_text.setCharacterSize(96);
    m_text.setPosition(m_pos.x + 200 - (m_text.getGlobalBounds().width/2.0f),
                       m_pos.y + 17 - (m_text.getGlobalBounds().height/2.0f));

    m_sprite = engine->button;
    m_sprite.setPosition(m_pos);
}

Button::Button(GameEngine* _engine, const char* text, sf::Vector2f pos)
{
    engine = _engine;
    m_pos = pos;

    m_text.setString(sf::String(text));
    m_text.setFont(engine->mc_font);
    m_text.setScale(0.25f, 0.25f);
    m_text.setCharacterSize(96);
    m_text.setPosition(m_pos.x + 200 - (m_text.getGlobalBounds().width/2.0f),
                       m_pos.y + 17 - (m_text.getGlobalBounds().height/2.0f));

    m_sprite = engine->button;
    m_sprite.setPosition(m_pos);
}

Button::Button(GameEngine* _engine, const char* text, float X, float Y)
{
    engine = _engine;
    m_pos = sf::Vector2f(X, Y);

    m_text.setString(sf::String(text));
    m_text.setFont(engine->mc_font);
    m_text.setScale(0.16667f, 0.16667f);
    m_text.setCharacterSize(96);
    m_text.setPosition(m_pos.x + 200 - (m_text.getGlobalBounds().width/2.0f),
                       m_pos.y + 17 - (m_text.getGlobalBounds().height/2.0f));

    m_sprite = engine->button;
    m_sprite.setPosition(m_pos);
}

void Button::setText(const char* text)
{
    m_text.setString(sf::String(text));
    m_text.setPosition(m_pos.x + 200 - (m_text.getGlobalBounds().width/2.0f),
                       m_pos.y + 17 - (m_text.getGlobalBounds().height/2.0f));
}

void Button::setText(sf::String text)
{
    m_text.setString(sf::String(text));
    m_text.setPosition(m_pos.x + 200 - (m_text.getGlobalBounds().width/2.0f),
                       m_pos.y + 17 - (m_text.getGlobalBounds().height/2.0f));
}

void Button::process_input(sf::Event& event)
{
    clicked = false;
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.x > m_pos.x and
            event.mouseButton.y > m_pos.y and
            event.mouseButton.x < m_pos.x + 400 and
            event.mouseButton.y < m_pos.y + 40)
            clicked = true;
    }
}

bool Button::update()
{
    if (sf::Mouse::getPosition(engine->app).x > m_pos.x and
        sf::Mouse::getPosition(engine->app).y > m_pos.y and
        sf::Mouse::getPosition(engine->app).x < m_pos.x + 400 and
        sf::Mouse::getPosition(engine->app).y < m_pos.y + 40)
    {
        m_sprite = engine->button_hover;
        m_text.setFillColor(sf::Color(255, 255, 128));
        if (clicked)
            engine->Sound()->playClickSound();
    }
    else
    {
        clicked = false;
        m_sprite = engine->button;
        m_text.setFillColor(sf::Color::White);
    }

    m_sprite.setPosition(m_pos);
    m_text.setPosition(m_pos.x + 200 - (m_text.getGlobalBounds().width/2.0f),
                       m_pos.y + 17 - (m_text.getGlobalBounds().height/2.0f));

    return clicked;
}

void Button::draw()
{
    engine->m_window.draw(m_sprite);
    engine->m_window.draw(m_text);
}
