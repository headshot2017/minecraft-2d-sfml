#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "../game_engine.h"

class Button
{
public:
    Button();
    Button(GameEngine* _engine, sf::String text, float X, float Y, int width=400);
    Button(GameEngine* _engine, const char* text, float X, float Y, int width=400);
    Button(GameEngine* _engine, sf::String text, sf::Vector2f pos, int width=400);
    Button(GameEngine* _engine, const char* text, sf::Vector2f pos, int width=400);
    void setButtonWidth(int width);

    sf::Vector2f getPos() {return m_pos;}
    void setText(const char *text);
    void setText(sf::String text);
    bool update();
    void process_input(sf::Event& event);
    void draw();

private:
    sf::Text m_text;
    sf::VertexArray m_sprite;
    sf::Texture* m_texture;
    sf::Vector2f m_pos;
    GameEngine* engine;
    bool clicked = false;
    int m_width = 0;
};

#endif // BUTTON_H_INCLUDED
