#include "slider.h"
#include "math.h"

Slider::Slider(GameEngine *engine, const char *text, sf::Vector2f pos, float length)
{
    m_engine = engine;
    m_handletex = &m_engine->m_button;
    m_minvalue = 0;
    m_maxvalue = 10;
    m_value = 0;
    m_pos = pos;

    m_box.setPrimitiveType(sf::Quads);
    m_box.resize(8);
    m_handle.setPrimitiveType(sf::Quads);
    m_handle.resize(8);
    m_label = Label(engine, text, pos.x+(length/2), pos.y+10, 1);
    m_text = sf::String(text);

    setWidth(length);

    m_handle[0].texCoords = sf::Vector2f(0,0);
    m_handle[1].texCoords = sf::Vector2f(4,0);
    m_handle[2].texCoords = sf::Vector2f(4,20);
    m_handle[3].texCoords = sf::Vector2f(0,20);

    m_handle[4].texCoords = sf::Vector2f(200-4,0);
    m_handle[5].texCoords = sf::Vector2f(200,0);
    m_handle[6].texCoords = sf::Vector2f(200,20);
    m_handle[7].texCoords = sf::Vector2f(200-4,20);
}

Slider::Slider(GameEngine *engine, sf::String text, sf::Vector2f pos, float length)
{
    m_engine = engine;
    m_handletex = &m_engine->m_button;
    m_minvalue = 0;
    m_maxvalue = 10;
    m_value = 0;
    m_pos = pos;

    m_box.setPrimitiveType(sf::Quads);
    m_box.resize(8);
    m_handle.setPrimitiveType(sf::Quads);
    m_handle.resize(8);
    m_label = Label(engine, text, pos.x+(length/2), pos.y+10, 1);
    m_text = text;

    setWidth(length);

    m_handle[0].texCoords = sf::Vector2f(0,0);
    m_handle[1].texCoords = sf::Vector2f(4,0);
    m_handle[2].texCoords = sf::Vector2f(4,20);
    m_handle[3].texCoords = sf::Vector2f(0,20);

    m_handle[4].texCoords = sf::Vector2f(200-4,0);
    m_handle[5].texCoords = sf::Vector2f(200,0);
    m_handle[6].texCoords = sf::Vector2f(200,20);
    m_handle[7].texCoords = sf::Vector2f(200-4,20);
}

Slider::Slider(GameEngine *engine, const char *text, float x, float y, float length)
{
    m_engine = engine;
    m_handletex = &m_engine->m_button;
    m_minvalue = 0;
    m_maxvalue = 10;
    m_value = 0;
    m_pos = sf::Vector2f(x,y);
    m_label = Label(engine, text, x+(length/2), y+10, 1);
    m_text = sf::String(text);

    m_box.setPrimitiveType(sf::Quads);
    m_box.resize(8);
    m_handle.setPrimitiveType(sf::Quads);
    m_handle.resize(8);

    m_handle[0].texCoords = sf::Vector2f(0,0);
    m_handle[1].texCoords = sf::Vector2f(4,0);
    m_handle[2].texCoords = sf::Vector2f(4,20);
    m_handle[3].texCoords = sf::Vector2f(0,20);

    m_handle[4].texCoords = sf::Vector2f(200-4,0);
    m_handle[5].texCoords = sf::Vector2f(200,0);
    m_handle[6].texCoords = sf::Vector2f(200,20);
    m_handle[7].texCoords = sf::Vector2f(200-4,20);

    setWidth(length);
}

Slider::Slider(GameEngine *engine, sf::String text, float x, float y, float length)
{
    m_engine = engine;
    m_handletex = &m_engine->m_button;
    m_minvalue = 0;
    m_maxvalue = 10;
    m_value = 0;
    m_pos = sf::Vector2f(x,y);
    m_label = Label(engine, text, x+(length/2), y+10, 1);
    m_text = sf::String(text);

    m_box.setPrimitiveType(sf::Quads);
    m_box.resize(8);
    m_handle.setPrimitiveType(sf::Quads);
    m_handle.resize(8);

    m_handle[0].texCoords = sf::Vector2f(0,0);
    m_handle[1].texCoords = sf::Vector2f(4,0);
    m_handle[2].texCoords = sf::Vector2f(4,20);
    m_handle[3].texCoords = sf::Vector2f(0,20);

    m_handle[4].texCoords = sf::Vector2f(200-4,0);
    m_handle[5].texCoords = sf::Vector2f(200,0);
    m_handle[6].texCoords = sf::Vector2f(200,20);
    m_handle[7].texCoords = sf::Vector2f(200-4,20);

    setWidth(length);
}

void Slider::setPosition(sf::Vector2f pos)
{
    m_pos = pos;
    setWidth(m_length);
}

void Slider::setWidth(float width)
{
    m_length = width;

    m_label.setPosition(m_pos.x+(width/2), m_pos.y+10);

    // left button
    m_box[0].position = m_pos;
    m_box[1].position = sf::Vector2f(m_pos.x+(width/2), m_pos.y);
    m_box[2].position = sf::Vector2f(m_pos.x+(width/2), m_pos.y+40);
    m_box[3].position = sf::Vector2f(m_pos.x, m_pos.y+40);

    m_box[0].texCoords = sf::Vector2f(0, 0);
    m_box[1].texCoords = sf::Vector2f(width/4, 0);
    m_box[2].texCoords = sf::Vector2f(width/4, 20);
    m_box[3].texCoords = sf::Vector2f(0, 20);

    // right button
    m_box[4].position = sf::Vector2f(m_pos.x+(width/2), m_pos.y);
    m_box[5].position = sf::Vector2f(m_pos.x+width, m_pos.y);
    m_box[6].position = sf::Vector2f(m_pos.x+width, m_pos.y+40);
    m_box[7].position = sf::Vector2f(m_pos.x+(width/2), m_pos.y+40);

    m_box[4].texCoords = sf::Vector2f(200-(width/4), 0);
    m_box[5].texCoords = sf::Vector2f(200, 0);
    m_box[6].texCoords = sf::Vector2f(200, 20);
    m_box[7].texCoords = sf::Vector2f(200-(width/4), 20);

    updateHandlePosition();
}

void Slider::updateHandlePosition()
{
    float scale = (m_value - m_minvalue) / (m_maxvalue - m_minvalue);
    float x = (m_length-16) * scale;

    m_handle[0].position = sf::Vector2f(m_pos.x+x, m_pos.y);
    m_handle[1].position = sf::Vector2f(m_pos.x+x+8, m_pos.y);
    m_handle[2].position = sf::Vector2f(m_pos.x+x+8, m_pos.y+40);
    m_handle[3].position = sf::Vector2f(m_pos.x+x, m_pos.y+40);

    m_handle[4].position = sf::Vector2f(m_pos.x+x+8, m_pos.y);
    m_handle[5].position = sf::Vector2f(m_pos.x+x+16, m_pos.y);
    m_handle[6].position = sf::Vector2f(m_pos.x+x+16, m_pos.y+40);
    m_handle[7].position = sf::Vector2f(m_pos.x+x+8, m_pos.y+40);
}

void Slider::setValue(float value)
{
    if (value < m_minvalue)
		value = m_minvalue;
	else if (value > m_maxvalue)
		value = m_maxvalue;

    if (value != m_value)
	{
		m_value = value;

		// Move the handle on the slider
		updateHandlePosition();
    }
}

void Slider::setMaxValue(float value)
{
    m_maxvalue = value;
    updateHandlePosition();
}

void Slider::setMinValue(float value)
{
    m_minvalue = value;
    updateHandlePosition();
}

void Slider::setValueRange(float minvalue, float maxvalue)
{
    m_minvalue = minvalue;
    m_maxvalue = maxvalue;
    updateHandlePosition();
}

void Slider::setText(sf::String text)
{
    m_text = text;
    m_label.setText(text);
}

void Slider::onMousePressed(float x, float y)
{
    float scale = x / m_length;
    if (not m_floats)
        setValue(floor(scale * (m_maxvalue - m_minvalue) + m_minvalue));
    else
        setValue(scale * (m_maxvalue - m_minvalue) + m_minvalue);
}

void Slider::onMouseWheelMoved(int delta)
{
	setValue(m_value + (delta > 0 ? 1 : -1));
}

void Slider::update()
{
    sf::Vector2i mouse = sf::Mouse::getPosition(m_engine->app);

    if (mouse.x > m_pos.x and mouse.y > m_pos.y and
        mouse.x < m_pos.x + m_length and mouse.y < m_pos.y + 40)
        m_handletex = &m_engine->m_button_hover;
    else
        m_handletex = &m_engine->m_button;

    if (m_holding)
    {
        onMousePressed(mouse.x-m_pos.x, mouse.y-m_pos.y);
    }

    char aBuf[64];
    sprintf(aBuf, "%s %.1f\n", m_text.toAnsiString().c_str(), m_value);
    m_label.setText(aBuf);
}

void Slider::process_input(sf::Event &event)
{
    bool in_bounds = (m_handletex == &m_engine->m_button_hover);
    if (event.type == sf::Event::MouseButtonPressed and in_bounds)
    {
        m_holding = true;
    }
    else if (event.type == sf::Event::MouseButtonReleased and m_holding)
    {
        m_holding = false;
        m_engine->Sound()->playClickSound();
    }
    else if (event.type == sf::Event::MouseWheelScrolled and in_bounds)
        onMouseWheelMoved(event.mouseWheelScroll.delta);
}

void Slider::draw()
{
    m_engine->m_window.draw(m_box, &m_engine->m_button_locked);
    m_engine->m_window.draw(m_handle, m_handletex);
    m_label.draw();
}
