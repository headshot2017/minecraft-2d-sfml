#include "label.h"

Label::Label(GameEngine *engine, const char *text, float x, float y, int halign, int valign)
{
    m_engine = engine;
    m_text.setString(sf::String(text));
    m_text.setFont(engine->mc_font);
    setScale(0.16667f);
    m_text.setCharacterSize(96);
    setPosition(sf::Vector2f(x,y));
    setAlign(halign, valign);
}

Label::Label(GameEngine *engine, sf::String text, float x, float y, int halign, int valign)
{
    m_engine = engine;
    m_text.setString(text);
    m_text.setFont(engine->mc_font);
    setScale(0.16667f);
    m_text.setCharacterSize(96);
    setPosition(sf::Vector2f(x,y));
    setAlign(halign, valign);
}

Label::Label(GameEngine *engine, const char *text, sf::Vector2f pos, int halign, int valign)
{
    m_engine = engine;
    m_text.setString(sf::String(text));
    m_text.setFont(engine->mc_font);
    setScale(0.16667f);
    m_text.setCharacterSize(96);
    setPosition(pos);
    setAlign(halign, valign);
}

Label::Label(GameEngine *engine, sf::String text, sf::Vector2f pos, int halign, int valign)
{
    m_engine = engine;
    m_text.setString(text);
    m_text.setFont(engine->mc_font);
    setScale(0.16667f);
    m_text.setCharacterSize(96);
    setPosition(pos);
    setAlign(halign, valign);
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

void Label::setHAlign(int align)
{
    m_align.x = align;
    setPosition(m_pos);
}

void Label::setVAlign(int align)
{
    m_align.y = align;
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

    if (m_align.x == 1) // middle
        m_pos.x = m_pos.x - (m_size.width/2.0f);
    else if (m_align.x == 2) // right
        m_pos.x - m_size.width;

    if (m_align.y == 1) // middle
        m_pos.y = m_pos.y - (m_size.height/2.0f);
    else if (m_align.y == 2) // bottom
        m_pos.y - m_size.height;

    m_text.setPosition(m_pos);
}

void Label::draw()
{
    m_engine->m_window.draw(m_text);
}
