#include "label.h"

Label::Label(GameEngine *engine, const char *text, float x, float y, int align)
{
    m_engine = engine;
    m_text.setString(sf::String(text));
    m_text.setFont(engine->mc_font);
    setScale(0.16667f);
    m_text.setCharacterSize(96);
    setPosition(sf::Vector2f(x,y));
    setAlign(align);
}

Label::Label(GameEngine *engine, sf::String text, float x, float y, int align)
{
    m_engine = engine;
    m_text.setString(text);
    m_text.setFont(engine->mc_font);
    setScale(0.16667f);
    m_text.setCharacterSize(96);
    setPosition(sf::Vector2f(x,y));
    setAlign(align);
}

Label::Label(GameEngine *engine, const char *text, sf::Vector2f pos, int align)
{
    m_engine = engine;
    m_text.setString(sf::String(text));
    m_text.setFont(engine->mc_font);
    setScale(0.16667f);
    m_text.setCharacterSize(96);
    setPosition(pos);
    setAlign(align);
}

Label::Label(GameEngine *engine, sf::String text, sf::Vector2f pos, int align)
{
    m_engine = engine;
    m_text.setString(text);
    m_text.setFont(engine->mc_font);
    setScale(0.16667f);
    m_text.setCharacterSize(96);
    setPosition(pos);
    setAlign(align);
}

void Label::setScale(float amount)
{
    m_text.setScale(amount, amount);
    setPosition(m_pos);
}

void Label::setRotation(float angle)
{
    m_text.setRotation(angle);
    setPosition(m_pos);
}

void Label::setColor(sf::Color color)
{
    m_text.setFillColor(color);
}

void Label::setAlign(int align)
{
    m_align = align;
    setPosition(m_pos);
}

void Label::setText(sf::String text)
{
    m_text.setString(sf::String(text));
    setPosition(m_pos);
}

void Label::setText(const char* text)
{
    m_text.setString(sf::String(text));
    setPosition(m_pos);
}

void Label::setPosition(sf::Vector2f pos)
{
    m_pos = pos;
    sf::FloatRect m_size = m_text.getGlobalBounds();

    if (m_align == 0) // left
        m_text.setPosition(m_pos.x, m_pos.y);
    else if (m_align == 1) // middle
        m_text.setPosition(m_pos.x - (m_size.width/2.0f), m_pos.y);
    else if (m_align == 2) // right
        m_text.setPosition(m_pos.x - m_size.width, m_pos.y);
}

void Label::setPosition(float x, float y)
{
    m_pos = sf::Vector2f(x,y);
    sf::FloatRect m_size = m_text.getGlobalBounds();

    if (m_align == 0) // left
        m_text.setPosition(m_pos.x, m_pos.y);
    else if (m_align == 1) // middle
        m_text.setPosition(m_pos.x - (m_size.width/2.0f), m_pos.y);
    else if (m_align == 2) // right
        m_text.setPosition(m_pos.x - m_size.width, m_pos.y);
}

void Label::draw()
{
    m_engine->m_window.draw(m_text);
}
