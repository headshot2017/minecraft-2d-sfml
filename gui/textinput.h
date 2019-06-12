#ifndef TEXTINPUT_H_INCLUDED
#define TEXTINPUT_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "../game_engine.h"

class TextInput
{
public:
    TextInput();
    TextInput(GameEngine *engine, sf::Vector2f pos);
    TextInput(GameEngine *engine, sf::String text, sf::Vector2f pos);
    TextInput(GameEngine *engine, const char *text, sf::Vector2f pos);

    void update();
    void process_input(sf::Event& event);
    void draw();

private:
    sf::String m_str;
    sf::Text m_text;
    sf::Text m_textcaret;
    sf::Vector2f m_pos;
    sf::RectangleShape m_rect;
    sf::RectangleShape m_caretline;

    std::size_t m_caret = 0;
    bool active = false;
    GameEngine *m_engine;
};

#endif // TEXTINPUT_H_INCLUDED
