#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "../game_engine.h"

class Button
{
public:
    Button();
    Button(GameEngine* _engine, sf::String text, float X, float Y);
    Button(GameEngine* _engine, sf::String text, sf::Vector2f pos);

    sf::Vector2f getPos() {return m_pos;}
    bool update();
    void process_input();
    void draw();

private:
    sf::Text m_text;
    sf::Sprite m_sprite;
    sf::Vector2f m_pos;
    GameEngine* engine;
    bool clicked = false;
};

#endif // BUTTON_H_INCLUDED
