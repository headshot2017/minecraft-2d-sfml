#include "textinput.h"

TextInput::TextInput()
{
    m_pos = sf::Vector2f(0,0);
    m_caret = 0;
}

TextInput::TextInput(GameEngine *engine, sf::Vector2f pos)
{
    m_engine = engine;
    m_pos = pos;

    m_rect.setPosition(m_pos);
    m_rect.setFillColor(sf::Color::Black);
    m_rect.setOutlineColor(sf::Color(128, 128, 128));
    m_rect.setOutlineThickness(2);
    m_rect.setSize(sf::Vector2f(400, 40));

    m_caretline.setFillColor(sf::Color::White);
    m_caretline.setSize(sf::Vector2f(2, 20));

    m_text.setPosition(m_pos.x+4, m_pos.y+10);
    m_text.setFont(engine->mc_font);
    m_text.setScale(0.16667f, 0.16667f);
    m_text.setCharacterSize(96);

    m_textcaret.setFont(engine->mc_font);
    m_textcaret.setScale(0.16667f, 0.16667f);
    m_textcaret.setCharacterSize(96);
}

TextInput::TextInput(GameEngine *engine, sf::String text, sf::Vector2f pos)
{
    m_engine = engine;
    m_pos = pos;
    m_str = text;

    m_rect.setPosition(m_pos);
    m_rect.setFillColor(sf::Color::Black);
    m_rect.setOutlineColor(sf::Color(128, 128, 128));
    m_rect.setOutlineThickness(2);
    m_rect.setSize(sf::Vector2f(400, 40));

    m_caretline.setFillColor(sf::Color::White);
    m_caretline.setSize(sf::Vector2f(2, 20));

    m_text.setPosition(m_pos.x+4, m_pos.y+10);
    m_text.setFont(engine->mc_font);
    m_text.setScale(0.16667f, 0.16667f);
    m_text.setCharacterSize(96);

    m_textcaret.setFont(engine->mc_font);
    m_textcaret.setScale(0.16667f, 0.16667f);
    m_textcaret.setCharacterSize(96);
}

TextInput::TextInput(GameEngine *engine, const char *text, sf::Vector2f pos)
{
    m_engine = engine;
    m_pos = pos;
    m_str = sf::String(text);

    m_rect.setPosition(m_pos);
    m_rect.setFillColor(sf::Color::Black);
    m_rect.setOutlineColor(sf::Color(128, 128, 128));
    m_rect.setOutlineThickness(2);
    m_rect.setSize(sf::Vector2f(400, 40));

    m_caretline.setFillColor(sf::Color::White);
    m_caretline.setSize(sf::Vector2f(2, 20));

    m_text.setPosition(m_pos.x+4, m_pos.y+10);
    m_text.setFont(engine->mc_font);
    m_text.setScale(0.16667f, 0.16667f);
    m_text.setCharacterSize(96);

    m_textcaret.setFont(engine->mc_font);
    m_textcaret.setScale(0.16667f, 0.16667f);
    m_textcaret.setCharacterSize(96);
}

void TextInput::update()
{
    m_text.setString(m_str);
    m_textcaret.setString(m_str.substring(0, m_caret));
    m_caretline.setPosition(m_textcaret.getGlobalBounds().width+m_pos.x+4, m_pos.y+10);
}

void TextInput::process_input(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.x >= m_pos.x and
            event.mouseButton.x < m_pos.x+400 and
            event.mouseButton.y >= m_pos.y and
            event.mouseButton.y < m_pos.y+40)
            active = true;
        else
            active = false;
    }

    else if (event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode == '\b')
        {
            if (m_caret > 0)
            {
                m_str.erase(m_caret-1, 1);
                m_caret--;
            }
        }
        else
        {
            m_str.insert(m_caret, event.text.unicode);
            m_caret++;
        }
    }

    else if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Left)
        {
            if (m_caret > 0)
                m_caret--;
        }
        else if (event.key.code == sf::Keyboard::Right)
        {
            if (m_caret < m_str.getSize())
                m_caret++;
        }
        else if (event.key.code == sf::Keyboard::End)
            m_caret = m_str.getSize();
        else if (event.key.code == sf::Keyboard::Home)
            m_caret = 0;
    }
}

void TextInput::draw()
{
    m_engine->app.draw(m_rect);
    m_engine->app.draw(m_text);
    if (active)
        m_engine->app.draw(m_caretline);
}
