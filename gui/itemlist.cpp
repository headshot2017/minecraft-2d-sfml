#include "itemlist.h"

ItemList::ItemList(GameEngine *engine, float x, float y, float w, float h, const char *label)
{
    m_engine = engine;
    fit_on_page = 0; max_page = 0; currpage = 0; selected = -1;
    m_pos = sf::Vector2f(x,y);
    m_size = sf::Vector2f(w,h);
    m_label = std::string(label);
    createButtons();
}

ItemList::ItemList(GameEngine *engine, sf::Vector2f pos, sf::Vector2f size, const char *label)
{
    m_engine = engine;
    fit_on_page = 0; max_page = 0; currpage = 0; selected = -1;
    m_pos = pos;
    m_size = size;
    m_label = std::string(label);
    createButtons();
}

ItemList::ItemList(GameEngine *engine, sf::FloatRect rect, const char *label)
{
    m_engine = engine;
    fit_on_page = 0; max_page = 0; currpage = 0; selected = -1;
    m_pos = sf::Vector2f(rect.left, rect.top);
    m_size = sf::Vector2f(rect.width, rect.height);
    m_label = std::string(label);
    createButtons();
}

ItemList::ItemList(GameEngine *engine, float x, float y, float w, float h, std::vector<std::string>& items, const char *label)
{
    m_engine = engine;
    fit_on_page = 0; max_page = 0; currpage = 0; selected = -1;
    m_pos = sf::Vector2f(x,y);
    m_size = sf::Vector2f(w,h);
    m_label = std::string(label);
    setItems(items);
    createButtons();
}

ItemList::ItemList(GameEngine *engine, sf::Vector2f pos, sf::Vector2f size, std::vector<std::string>& items, const char *label)
{
    m_engine = engine;
    fit_on_page = 0; max_page = 0; currpage = 0; selected = -1;
    m_pos = pos;
    m_size = size;
    m_label = std::string(label);
    setItems(items);
    createButtons();
}

ItemList::ItemList(GameEngine *engine, sf::FloatRect rect, std::vector<std::string>& items, const char *label)
{
    m_engine = engine;
    fit_on_page = 0; max_page = 0; currpage = 0; selected = -1;
    m_pos = sf::Vector2f(rect.left, rect.top);
    m_size = sf::Vector2f(rect.width, rect.height);
    m_label = std::string(label);
    setItems(items);
    createButtons();
}

void ItemList::createButtons()
{
    m_prevpage = Button(m_engine, "<<", 0, 0, 40);
    m_nextpage = Button(m_engine, ">>", 0, 0, 40);
    m_prevpage.onClicked(&onPrevPage, this);
    m_nextpage.onClicked(&onNextPage, this);
    setupButtons();
}

void ItemList::setupButtons()
{
    m_prevpage.setPosition(m_pos.x - 48, m_pos.y + (m_size.y/2) - 20);
    m_nextpage.setPosition(m_pos.x + m_size.x + 8, m_pos.y + (m_size.y/2) - 20);
}

void ItemList::setPage(unsigned page)
{
    currpage = page;
}

void ItemList::onPrevPage(void *pUserData)
{
    ItemList* self = (ItemList* )pUserData;

    if (self->currpage > 0)
        self->setPage(self->currpage - self->fit_on_page);
}

void ItemList::onNextPage(void *pUserData)
{
    ItemList* self = (ItemList* )pUserData;

    if (self->currpage + self->fit_on_page < self->m_items.size())
        self->setPage(self->currpage + self->fit_on_page);
}

void ItemList::update()
{
    fit_on_page = (m_size.y)/24;
    max_page = fit_on_page+currpage;

    m_prevpage.update();
    m_nextpage.update();
}

bool ItemList::event_input(sf::Event& event)
{
    bool ret = false;
    m_prevpage.process_input(event);
    m_nextpage.process_input(event);

    if (event.type == sf::Event::MouseButtonPressed)
    {
        for(unsigned i=currpage; i<max_page; i++)
        {
            if (i >= m_items.size()) break;

            float x = m_pos.x+4;
            float y = m_pos.y+((i % fit_on_page)*24)+4;

            if (event.mouseButton.x >= x-4 and event.mouseButton.x < x+m_size.x and
                event.mouseButton.y >= y-4 and event.mouseButton.y < y+20)
            {
                ret = true;
                setSelected(static_cast<int>(i));
            }
        }
    }

    return ret;
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

    if (not m_label.empty())
    {
        Label l_label(m_engine, m_label, m_pos.x, m_pos.y-28);
        l_label.draw();
    }

    for(unsigned i=currpage; i<max_page; i++)
    {
        if (i >= m_items.size()) break;

        float x = m_pos.x+4;
        float y = m_pos.y+((i % fit_on_page)*24)+4;

        Label m_text(m_engine, m_items[i].c_str(), x, y);
        m_text.draw();
    }

    if (selected >= 0 and fit_on_page > 0)
    {
        if (selected / fit_on_page == currpage / fit_on_page)
        {
            sf::RectangleShape sRect(sf::Vector2f(m_size.x, 24));
            sRect.setFillColor(sf::Color(255, 255, 255, 96));
            sRect.setPosition(m_pos.x, m_pos.y+((selected % fit_on_page)*24)+2);
            m_engine->m_window.draw(sRect);
        }
    }

    m_engine->m_window.draw(Outline);
    m_prevpage.draw();
    m_nextpage.draw();
}
