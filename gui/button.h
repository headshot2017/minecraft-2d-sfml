#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <game_engine.h>
#include "defs.h"
#include "label.h"

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
    void setText(const char *text) {setText(sf::String(text));}
    void setText(sf::String text);
    void setPosition(sf::Vector2f pos);
    void setPosition(float x, float y) {setPosition(sf::Vector2f(x,y));}
    void onClicked(Callback newAction, void *pUser) {callback = newAction; pUserData = pUser;}
    void update();
    void process_input(sf::Event& event);
    void draw();

private:
    Label m_text;
    sf::VertexArray m_sprite;
    sf::Texture* m_texture;
    sf::Vector2f m_pos;
    GameEngine* engine;

    Callback callback;
    void *pUserData;

    int m_width = 0;
};

#endif // BUTTON_H_INCLUDED
