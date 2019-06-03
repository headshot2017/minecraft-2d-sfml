#include <SFML/Graphics.hpp>
#include "button.h"
//#include "bass.h"
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

    m_sprite = engine->button;
    m_sprite.setPosition(m_pos);
}

Button::Button(GameEngine* _engine, sf::String text, float X, float Y)
{
    engine = _engine;
    m_pos = sf::Vector2f(X, Y);

    m_text.setString(text);
    m_text.setFont(engine->mc_font);
    m_text.setScale(0.25f, 0.25f);
    m_text.setCharacterSize(96);

    m_sprite = engine->button;
    m_sprite.setPosition(m_pos);
}

bool Button::update()
{
    if (sf::Mouse::getPosition(engine->app).x > m_pos.x and
        sf::Mouse::getPosition(engine->app).y > m_pos.y and
        sf::Mouse::getPosition(engine->app).x < m_pos.x + 400 and
        sf::Mouse::getPosition(engine->app).y < m_pos.y + 40)
    {
        m_sprite = engine->button_hover;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and not clicked) // left click
        {
            printf("clicked\n");
            clicked = true;
        }
        else if (not sf::Mouse::isButtonPressed(sf::Mouse::Left) and clicked)
        {
            printf("released\n");
            clicked = false;
        }
    }
    else
        m_sprite = engine->button;

    m_sprite.setPosition(m_pos);
    m_text.setPosition(m_pos.x + 200 - (m_text.getGlobalBounds().width/2.0f),
                       m_pos.y + 17 - (m_text.getGlobalBounds().height/2.0f));

    return clicked;
}

void Button::draw()
{
    engine->app.draw(m_sprite);
    engine->app.draw(m_text);
}
