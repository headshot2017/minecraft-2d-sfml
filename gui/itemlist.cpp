#include "itemlist.h"

ItemList::ItemList(GameEngine *engine, float x, float y, float w, float h)
{
    m_engine = engine;
    setPosition(x,y);
    setSize(w,h);
}

ItemList::ItemList(GameEngine *engine, sf::Vector2f pos, sf::Vector2f size)
{
    m_engine = engine;
    setPosition(pos);
    setSize(size);
}

ItemList::ItemList(GameEngine *engine, sf::FloatRect rect)
{
    m_engine = engine;
    setRect(rect);
}

ItemList::ItemList(GameEngine *engine, float x, float y, float w, float h, std::vector<std::string>& items)
{
    m_engine = engine;
    setPosition(x,y);
    setSize(w,h);
    setItems(items);
}

ItemList::ItemList(GameEngine *engine, sf::Vector2f pos, sf::Vector2f size, std::vector<std::string>& items)
{
    m_engine = engine;
    setPosition(pos);
    setSize(size);
    setItems(items);
}

ItemList::ItemList(GameEngine *engine, sf::FloatRect rect, std::vector<std::string>& items)
{
    m_engine = engine;
    setRect(rect);
    setItems(items);
}

void ItemList::update()
{

}

void ItemList::event_input(sf::Event& event)
{

}

void ItemList::draw()
{
    sf::RectangleShape aRect(m_size);
    sf::RectangleShape Outline(m_size);
    aRect.setPosition(m_pos);
    Outline.setPosition(m_pos);
    aRect.setFillColor(sf::Color(0,0,0,160));
    Outline.setFillColor(sf::Color::Transparent);
    Outline.setOutlineThickness(6);
    Outline.setOutlineColor(sf::Color::Black);
    m_engine->m_window.draw(aRect);
    m_engine->m_window.draw(Outline);

    fit_on_page = (m_size.y-96)/16;
    max_page = fit_on_page+currpage;
    for(unsigned i=currpage; i<max_page; i++)
    {
        if (i >= m_items.size()) break;

        Label m_text(m_engine, m_items[i].c_str(), m_pos.x+4, m_pos.y+(i*24)+4);
        m_text.draw();
    }
}
