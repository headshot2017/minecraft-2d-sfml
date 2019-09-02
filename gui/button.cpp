#include <SFML/Graphics.hpp>
#include "button.h"
#include "../bass.h"
#include "../game_engine.h"
#include <stdio.h>

Button::Button()
{

}

Button::Button(GameEngine* _engine, sf::String text, sf::Vector2f pos, int width)
{
    engine = _engine;
    m_pos = pos;
    m_texture = &engine->m_button;

    m_text = Label(_engine, text, pos, 1, 1);
    m_text.setPosition(m_pos.x + (width/2), m_pos.y + 17);
    //m_text.setPosition(m_pos.x + (width/2) - (m_text.getGlobalBounds().width/2.0f),
    //                   m_pos.y + 17 - (m_text.getGlobalBounds().height/2.0f));

    m_sprite.resize(8);
    m_sprite.setPrimitiveType(sf::Quads);
    setButtonWidth(width);
}

Button::Button(GameEngine* _engine, sf::String text, float X, float Y, int width)
{
    engine = _engine;
    m_pos = sf::Vector2f(X, Y);
    m_texture = &engine->m_button;

    m_text = Label(_engine, text, X, Y, 1, 1);
    m_text.setPosition(m_pos.x + (width/2), m_pos.y + 17);
    //m_text.setPosition(m_pos.x + (width/2) - (m_text.getGlobalBounds().width/2.0f),
    //                   m_pos.y + 17 - (m_text.getGlobalBounds().height/2.0f));

    m_sprite.resize(8);
    m_sprite.setPrimitiveType(sf::Quads);
    setButtonWidth(width);
}

Button::Button(GameEngine* _engine, const char* text, sf::Vector2f pos, int width)
{
    engine = _engine;
    m_pos = pos;
    m_texture = &engine->m_button;

    m_text = Label(_engine, text, pos, 1, 1);
    m_text.setPosition(m_pos.x + (width/2), m_pos.y + 17);
    //m_text.setPosition(m_pos.x + (width/2) - (m_text.getGlobalBounds().width/2.0f),
    //                   m_pos.y + 17 - (m_text.getGlobalBounds().height/2.0f));

    m_sprite.resize(8);
    m_sprite.setPrimitiveType(sf::Quads);
    setButtonWidth(width);
}

Button::Button(GameEngine* _engine, const char* text, float X, float Y, int width)
{
    engine = _engine;
    m_pos = sf::Vector2f(X, Y);
    m_texture = &engine->m_button;

    m_text = Label(_engine, text, X, Y, 1, 1);
    m_text.setPosition(m_pos.x + (width/2), m_pos.y + 17);
    //m_text.setPosition(m_pos.x + (width/2) - (m_text.getGlobalBounds().width/2.0f),
    //                   m_pos.y + 17 - (m_text.getGlobalBounds().height/2.0f));

    m_sprite.resize(8);
    m_sprite.setPrimitiveType(sf::Quads);
    setButtonWidth(width);
}

void Button::setButtonWidth(int width)
{
    m_width = width;
    m_text.setPosition(m_pos.x + (width/2), m_pos.y + 17);

    // left button
    m_sprite[0].position = m_pos;
    m_sprite[1].position = sf::Vector2f(m_pos.x+(width/2), m_pos.y);
    m_sprite[2].position = sf::Vector2f(m_pos.x+(width/2), m_pos.y+40);
    m_sprite[3].position = sf::Vector2f(m_pos.x, m_pos.y+40);

    m_sprite[0].texCoords = sf::Vector2f(0, 0);
    m_sprite[1].texCoords = sf::Vector2f(width/4, 0);
    m_sprite[2].texCoords = sf::Vector2f(width/4, 20);
    m_sprite[3].texCoords = sf::Vector2f(0, 20);

    // right button
    m_sprite[4].position = sf::Vector2f(m_pos.x+(width/2), m_pos.y);
    m_sprite[5].position = sf::Vector2f(m_pos.x+width, m_pos.y);
    m_sprite[6].position = sf::Vector2f(m_pos.x+width, m_pos.y+40);
    m_sprite[7].position = sf::Vector2f(m_pos.x+(width/2), m_pos.y+40);

    m_sprite[4].texCoords = sf::Vector2f(200-(width/4), 0);
    m_sprite[5].texCoords = sf::Vector2f(200, 0);
    m_sprite[6].texCoords = sf::Vector2f(200, 20);
    m_sprite[7].texCoords = sf::Vector2f(200-(width/4), 20);
}

void Button::setText(sf::String text)
{
    m_text.setText(text);
}

void Button::setPosition(sf::Vector2f pos)
{
    m_pos = pos;
    setButtonWidth(m_width);
}

void Button::process_input(sf::Event& event)
{
    clicked = false;
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.x > m_pos.x and
            event.mouseButton.y > m_pos.y and
            event.mouseButton.x < m_pos.x + m_width and
            event.mouseButton.y < m_pos.y + 40 and
            event.mouseButton.button == sf::Mouse::Left)
            clicked = true;
    }
}

bool Button::update()
{
    if (sf::Mouse::getPosition(engine->app).x > m_pos.x and
        sf::Mouse::getPosition(engine->app).y > m_pos.y and
        sf::Mouse::getPosition(engine->app).x < m_pos.x + m_width and
        sf::Mouse::getPosition(engine->app).y < m_pos.y + 40)
    {
        m_texture = &engine->m_button_hover;
        m_text.setColor(sf::Color(255, 255, 128));
        if (clicked)
            engine->Sound()->playClickSound();
    }
    else
    {
        clicked = false;
        m_texture = &engine->m_button;
        m_text.setColor(sf::Color::White);
    }

    return clicked;
}

void Button::draw()
{
    //printf("draw. %s %s\n", m_text.getString().toAnsiString().c_str(), (m_texture == nullptr) ? "YES" : "no");
    engine->m_window.draw(m_sprite, m_texture);
    m_text.draw();
}
