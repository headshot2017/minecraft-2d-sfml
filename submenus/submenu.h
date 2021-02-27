#ifndef _SUBMENU_H
#define _SUBMENU_H

#include "../game_engine.h"

class Submenu
{
public:
    Submenu(GameEngine* engine) {m_engine = engine; m_allowEsc = true;}
    virtual ~Submenu() {};

    virtual void update(float delta) = 0;
    virtual void process_input(sf::Event& event) = 0;
    virtual void draw() = 0;
    virtual void onResolutionChange(sf::Vector2u res) = 0;

    bool m_allowEsc;

protected:
    GameEngine *m_engine;
};

#endif // _SUBMENU_H
