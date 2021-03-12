#ifndef TEXTINPUT_H_INCLUDED
#define TEXTINPUT_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "../game_engine.h"
#include "defs.h"

class TextInput
{
public:
    TextInput();
    TextInput(GameEngine *engine, sf::Vector2f pos, std::size_t length=0);
    TextInput(GameEngine *engine, sf::String text, sf::Vector2f pos, std::size_t length=0);
    TextInput(GameEngine *engine, const char *text, sf::Vector2f pos, std::size_t length=0);

    void setText(const char *text) {m_str = sf::String(text);}
    void setText(sf::String text) {m_str = text;}
    void setPosition(sf::Vector2f pos);
    void setPosition(float x, float y) {setPosition(sf::Vector2f(x,y));}
    void setCursor(unsigned int pos);

    sf::String getString() {return m_str;}

    void update();
    void process_input(sf::Event& event);
    void draw();

private:
    sf::String m_str;
    sf::Text m_text;
    sf::Vector2f m_pos;
    sf::RectangleShape m_rect;
    sf::RectangleShape m_caretline;

    std::size_t m_caret;
    std::size_t m_length;
    bool active;
    GameEngine *m_engine;

    StringCallback onEnterPressed;
    StringCallback onClickAway;
    void* pUserData;
};

#endif // TEXTINPUT_H_INCLUDED
