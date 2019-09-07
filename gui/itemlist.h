#ifndef ITEMLIST_H_INCLUDED
#define ITEMLIST_H_INCLUDED

#include "button.h"
#include "label.h"

class ItemList
{
public:
    ItemList() {fit_on_page = max_page = currpage = 0; selected = 0;}
    ItemList(GameEngine *engine, float x, float y, float w, float h);
    ItemList(GameEngine *engine, sf::Vector2f pos, sf::Vector2f size);
    ItemList(GameEngine *engine, sf::FloatRect rect);
    ItemList(GameEngine *engine, float x, float y, float w, float h, std::vector<std::string>& items);
    ItemList(GameEngine *engine, sf::Vector2f pos, sf::Vector2f size, std::vector<std::string>& items);
    ItemList(GameEngine *engine, sf::FloatRect rect, std::vector<std::string>& items);

    void update();
    void event_input(sf::Event& event);
    void draw();

    void setSize(sf::Vector2f newsize) {m_size = newsize;}
    void setSize(float w, float h) {m_size = sf::Vector2f(w,h);}
    void setPosition(sf::Vector2f newpos) {m_pos = newpos;}
    void setPosition(float x, float y) {m_pos = sf::Vector2f(x,y);}
    void setRect(sf::FloatRect rect) {m_pos = sf::Vector2f(rect.left, rect.top); m_size = sf::Vector2f(rect.width, rect.height);}
    void setItems(std::vector<std::string>& items) {m_items = items;}

    sf::Vector2f getSize() {return m_size;}
    sf::Vector2f getPosition() {return m_pos;}
    std::vector<std::string> getItems() {return m_items;}
    int getCurrPage() {return currpage;}

private:
    GameEngine *m_engine;
    sf::Vector2f m_size, m_pos;
    std::vector<std::string> m_items;
    Button m_prevpage, m_nextpage;
    unsigned fit_on_page, max_page, currpage;
    int selected;
};

#endif // ITEMLIST_H_INCLUDED
