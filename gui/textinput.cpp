#ifndef __APPLE__
    #include <GL/gl.h>
#else
    #include <OpenGL/gl.h>
#endif
#include "textinput.h"

TextInput::TextInput()
{
    m_pos = sf::Vector2f(0,0);
    m_caret = 0;
}

TextInput::TextInput(GameEngine *engine, sf::Vector2f pos, std::size_t length)
{
    m_engine = engine;
    m_pos = pos;
    m_length = length;
    m_caret = 0;
    active = false;

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

    onClickAway = NULL;
    onEnterPressed = NULL;
}

TextInput::TextInput(GameEngine *engine, sf::String text, sf::Vector2f pos, std::size_t length)
{
    m_engine = engine;
    m_pos = pos;
    m_str = text;
    m_length = length;
    active = false;

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
    m_text.setString(m_str);

    setCursor(m_str.getSize());

    onClickAway = NULL;
    onEnterPressed = NULL;
}

TextInput::TextInput(GameEngine *engine, const char *text, sf::Vector2f pos, std::size_t length)
{
    m_engine = engine;
    m_str = sf::String(text);
    m_length = length;
    active = false;

    m_rect.setFillColor(sf::Color::Black);
    m_rect.setOutlineColor(sf::Color(128, 128, 128));
    m_rect.setOutlineThickness(2);
    m_rect.setSize(sf::Vector2f(400, 40));

    m_caretline.setFillColor(sf::Color::White);
    m_caretline.setSize(sf::Vector2f(2, 20));

    m_text.setFont(engine->mc_font);
    m_text.setScale(0.16667f, 0.16667f);
    m_text.setCharacterSize(96);
    m_text.setString(m_str);

    setPosition(pos);
    setCursor(m_str.getSize());

    onClickAway = NULL;
    onEnterPressed = NULL;
}

void TextInput::setPosition(sf::Vector2f pos)
{
    m_pos = pos;
    m_rect.setPosition(m_pos);
    m_text.setPosition(m_pos.x+6, m_pos.y+10);
}

void TextInput::setCursor(unsigned int pos)
{
    if (pos <= m_str.getSize())
    {
        m_caret = pos;

        float padding = 4;
        m_caretline.setPosition(m_text.findCharacterPos(pos).x - 2, m_pos.y+10);

        if (m_caretline.getPosition().x > m_pos.x + 400 - padding)
        {
            // shift left
            float diff = m_caretline.getPosition().x - m_pos.x - 400 + padding;
            m_text.move(-diff, 0);
            m_caretline.move(-diff, 0);
        }
        else if (m_caretline.getPosition().x < m_pos.x + padding)
        {
            // shift right
            float diff = m_pos.x + padding - m_caretline.getPosition().x;
            m_text.move(diff, 0);
            m_caretline.move(diff, 0);
        }

        float textwidth = m_text.getGlobalBounds().width;
        if (m_text.getPosition().x < m_pos.x + padding && m_pos.x + m_text.getPosition().x + textwidth < 400 - padding)
        {
            float diff = (400 - padding) - (m_text.getPosition().x + textwidth);
            m_text.move(diff, 0);
            m_caretline.move(diff, 0);
            // if text is smaller than textbox, force align left
            if (textwidth < (400 - padding * 2))
            {
                diff = padding - m_text.getPosition().x + m_pos.x;
                m_text.move(diff, 0);
                m_caretline.move(diff, 0);
            }
        }
    }
}

void TextInput::update()
{
    if (m_caret > m_str.getSize())
        m_caret = m_str.getSize();
}

void TextInput::process_input(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        bool doActive = (event.mouseButton.x >= m_pos.x &&
                        event.mouseButton.x < m_pos.x+400 &&
                        event.mouseButton.y >= m_pos.y &&
                        event.mouseButton.y < m_pos.y+40);

        if (!doActive && active && onClickAway)
            onClickAway(m_str.toAnsiString(), pUserData);
        active = doActive;
    }
    else if (event.type == sf::Event::TextEntered and active)
    {
        if (event.text.unicode == '\b')
        {
            if (m_caret > 0)
            {
                m_str.erase(m_caret-1, 1);
                m_text.setString(m_str);
                setCursor(m_caret-1);
            }
        }
        else
        {
            if (event.text.unicode != '\r' and event.text.unicode != '\n' and (m_length > 0 and m_str.getSize() < m_length))
            {
                m_str.insert(m_caret, event.text.unicode);
                m_text.setString(m_str);
                setCursor(m_caret+1);
            }
        }
    }

    else if (event.type == sf::Event::KeyPressed and active)
    {
        if (event.key.code == sf::Keyboard::Left)
            setCursor(m_caret-1);
        else if (event.key.code == sf::Keyboard::Right)
            setCursor(m_caret+1);
        else if (event.key.code == sf::Keyboard::End)
            setCursor(m_str.getSize());
        else if (event.key.code == sf::Keyboard::Home)
            setCursor(0);
        else if (event.key.code == sf::Keyboard::Enter)
        {
            active = false;
            if (onEnterPressed)
                onEnterPressed(m_str.toAnsiString(), pUserData);
        }
        else if (event.key.code == sf::Keyboard::Delete)
        {
            m_text.setString(m_str);
            setCursor(m_caret);
        }
    }
}

void TextInput::draw()
{
    m_engine->m_window.draw(m_rect);

    // crop text with GL scissor
    glEnable(GL_SCISSOR_TEST);
    glScissor(m_pos.x + 2, m_engine->m_window.getSize().y - (m_pos.y + 40), 400-4, 40);

    m_engine->m_window.draw(m_text);
    if (active)
        m_engine->m_window.draw(m_caretline);

    glDisable(GL_SCISSOR_TEST);
}
