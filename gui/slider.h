#ifndef SLIDER_H_INCLUDED
#define SLIDER_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "../game_engine.h"
#include "label.h"

class Slider
{
public:
    Slider() {}
    Slider(GameEngine *engine, sf::Vector2f pos, float length = 400);
    Slider(GameEngine *engine, float x, float y, float length = 400);

	int getQuantum() const {return m_quantum;}

	void setQuantum(int quantum);

    int getValue() const {return m_value;}

	void setValue(int value);

	void setWidth(float length);
	void setPosition(sf::Vector2f pos);
	void setPosition(float x, float y) {setPosition(sf::Vector2f(x,y));}


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
	float m_length;
	int m_quantum;
	int m_value;

	Label m_label;
	sf::VertexArray m_box;
	sf::VertexArray m_handle;
	sf::Texture *m_handletex;
	sf::Vector2f m_pos;
};

#endif // SLIDER_H_INCLUDED
