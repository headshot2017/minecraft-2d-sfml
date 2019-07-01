#ifndef TEXTINPUT_H_INCLUDED
#define TEXTINPUT_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "../game_engine.h"

class TextInput
{
public:
    TextInput();
    TextInput(GameEngine *engine, sf::Vector2f pos, std::size_t length=0);
    TextInput(GameEngine *engine, sf::String text, sf::Vector2f pos, std::size_t length=0);
    TextInput(GameEngine *engine, const char *text, sf::Vector2f pos, std::size_t length=0);

    void setText(const char *text) {m_str = sf::String(text);}
    void setText(sf::String text) {m_str = text;}

    sf::String getString() {return m_str;}

    void update();
    int process_input(sf::Event& event);
    void draw();

private:
    sf::String m_str;
    sf::Text m_text;
    sf::Text m_textcaret;
    sf::Vector2f m_pos;
    sf::RectangleShape m_rect;
    sf::RectangleShape m_caretline;

    std::size_t m_caret = 0;
    std::size_t m_length;
    bool active = false;
    GameEngine *m_engine;
};

#endif // TEXTINPUT_H_INCLUDED
