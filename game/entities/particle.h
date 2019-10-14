#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include "../entity.h"

class Particle : public Entity
{
public:
    Particle() {}
    Particle(World *world, GameEngine *engine, float _x, float _y);
    Particle(World *world, GameEngine *engine, sf::Vector2f pos);

    virtual sf::Vector2u getSize() = 0;
    virtual bool groundCollide() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
};

#endif // PARTICLE_H_INCLUDED
