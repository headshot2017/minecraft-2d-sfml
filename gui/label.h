#ifndef LABEL_H_INCLUDED
#define LABEL_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "../game_engine.h"

class Label
{
public:
    Label() {}
    Label(GameEngine *engine, const char *text, float x, float y, int align=0);
    Label(GameEngine *engine, sf::String text, float x, float y, int align=0);
    Label(GameEngine *engine, const char *text, sf::Vector2f pos, int align=0);
    Label(GameEngine *engine, sf::String text, sf::Vector2f pos, int align=0);

    void setAlign(int align);
    void setText(const char *text);
    void setText(sf::String text);
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f pos);

    void draw();

private:
    GameEngine *m_engine;
    sf::Text m_text;
    int m_align = 0;
    sf::Vector2f m_pos;
};

#endif // LABEL_H_INCLUDED
