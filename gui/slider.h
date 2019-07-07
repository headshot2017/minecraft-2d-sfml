#ifndef SLIDER_H_INCLUDED
#define SLIDER_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "../game_engine.h"
#include "label.h"

class Slider
{
public:
    Slider() {}
    Slider(GameEngine *engine, const char *text, sf::Vector2f pos, float length = 400);
    Slider(GameEngine *engine, sf::String text, sf::Vector2f pos, float length = 400);
    Slider(GameEngine *engine, const char *text, float x, float y, float length = 400);
    Slider(GameEngine *engine, sf::String text, float x, float y, float length = 400);

    float getValue() const {return m_value;}
    float getMinValue() const {return m_minvalue;}
    float getMaxValue() const {return m_maxvalue;}

	void setValue(float value);
	void setMinValue(float value);
	void setMaxValue(float value);
	void setValueRange(float minvalue, float maxvalue);
	void setFloatValue(bool use) {m_floats = use;}

	void setWidth(float length);
	void setPosition(sf::Vector2f pos);
	void setPosition(float x, float y) {setPosition(sf::Vector2f(x,y));}
	void setText(sf::String text);
	void setText(const char *text) {setText(sf::String(text));}


	// callbacks ---------------------------------------------------------------
	void onMousePressed(float x, float y);
	void onMouseReleased(float x, float y);
	void onMouseWheelMoved(int delta);

	void update();
	void process_input(sf::Event &event);
	void draw();

private:
	void updateHandlePosition();

	GameEngine *m_engine;
	bool m_holding = false;
	bool m_floats = false;
	float m_length;
	float m_value;
	float m_minvalue;
	float m_maxvalue;

	Label m_label;
	sf::String m_text;
	sf::VertexArray m_box;
	sf::VertexArray m_handle;
	sf::Texture *m_handletex;
	sf::Vector2f m_pos;
};

#endif // SLIDER_H_INCLUDED
