#ifndef ITEMLIST_H_INCLUDED
#define ITEMLIST_H_INCLUDED

#include "button.h"
#include "label.h"

class ItemList
{
public:
    ItemList()
    {
        fit_on_page = 0; max_page = 0; currpage = 0; selected = -1;
        m_pos = sf::Vector2f(0,0);
        m_size = sf::Vector2f(0,0);
    }
    ItemList(GameEngine *engine, float x, float y, float w, float h, const char *label="");
    ItemList(GameEngine *engine, sf::Vector2f pos, sf::Vector2f size, const char *label="");
    ItemList(GameEngine *engine, sf::FloatRect rect, const char *label="");
    ItemList(GameEngine *engine, float x, float y, float w, float h, std::vector<std::string>& items, const char *label="");
    ItemList(GameEngine *engine, sf::Vector2f pos, sf::Vector2f size, std::vector<std::string>& items, const char *label="");
    ItemList(GameEngine *engine, sf::FloatRect rect, std::vector<std::string>& items, const char *label="");
    void createButtons();
    void setupButtons();

    void update();
    bool event_input(sf::Event& event);
    void draw();

    void setSize(sf::Vector2f newsize) {m_size = newsize; setupButtons();}
    void setSize(float w, float h) {m_size = sf::Vector2f(w,h); setupButtons();}
    void setPosition(sf::Vector2f newpos) {m_pos = newpos; setupButtons();}
    void setPosition(float x, float y) {m_pos = sf::Vector2f(x,y); setupButtons();}
    void setRect(sf::FloatRect rect) {m_pos = sf::Vector2f(rect.left, rect.top); m_size = sf::Vector2f(rect.width, rect.height); setupButtons();}
    void setItems(std::vector<std::string>& items) {m_items = items; selected = -1;}
    void setPage(unsigned page);
    void setSelected(int selection) {selected = selection;}

    sf::Vector2f getSize() {return m_size;}
    sf::Vector2f getPosition() {return m_pos;}
    std::vector<std::string> getItems() {return m_items;}
    unsigned getCurrPage() {return currpage;}
    int getSelected() {return selected;}
    bool isSelected() {return selected >= 0;}
    std::string getSelectedItem() {return m_items[selected];}

private:
    GameEngine *m_engine;
    sf::Vector2f m_size, m_pos;
    std::vector<std::string> m_items;
    Button m_prevpage, m_nextpage;
    std::string m_label;
    unsigned fit_on_page = 0, max_page = 0, currpage = 0;
    int selected = -1;
};

#endif // ITEMLIST_H_INCLUDED
