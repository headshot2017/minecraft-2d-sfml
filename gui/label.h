#ifndef LABEL_H_INCLUDED
#define LABEL_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "../game_engine.h"

class Label
{
public:
    Label() {}
    Label(GameEngine *engine, const char *text, float x, float y, int halign=0, int valign=0);
    Label(GameEngine *engine, sf::String text, float x, float y, int halign=0, int valign=0);
    Label(GameEngine *engine, const char *text, sf::Vector2f pos, int halign=0, int valign=0);
    Label(GameEngine *engine, sf::String text, sf::Vector2f pos, int halign=0, int valign=0);

    void setAlign(int halign, int valign=0) {setHAlign(halign); setVAlign(valign);}
    void setHAlign(int align);
    void setVAlign(int align);
    void setText(const char *text);
    void setText(sf::String text);
    void setPosition(sf::Vector2f pos);
    void setPosition(float x, float y);
    void setColor(sf::Color color);
    void setScale(float amount);
    void setRotation(float angle);

    const sf::String getText() {return m_text.getString();}
    sf::Vector2f getPosition() {return m_pos;}
    int getHAlign() {return m_align.x;}
    int getVAlign() {return m_align.y;}

    void draw();

private:
    GameEngine *m_engine;
    sf::Text m_text;
    sf::Vector2i m_align;
    sf::Vector2f m_pos;
};

#endif // LABEL_H_INCLUDED
