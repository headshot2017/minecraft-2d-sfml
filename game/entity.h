#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "../game_engine.h"
#include "world.h"

enum
{
    ENT_FALLINGBLOCK,
    ENT_TNT
};

class World;

class Entity
{
public:
    Entity() {}
    Entity(World *world, GameEngine *engine);

    virtual sf::Vector2u getSize() = 0;
    virtual bool groundCollide() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    bool blockCollide(int x, int y);
    void knockBack(float xx, float yy, int force);

    int entityId() {return m_entityid;}
    int getBlock() {return m_block;}
    int getTicks() {return m_ticks;}
    int getTicksLeft() {return m_ticksleft;}
    sf::Vector2f getPos() {return sf::Vector2f(x,y);}

protected:
    float x, y, hspeed, vspeed, x_acc, gravity;
    int m_entityid;
    int m_block;
    int m_ticks, m_ticksleft;
    GameEngine *m_engine;
    World *m_world;
};

#endif // ENTITY_H_INCLUDED
