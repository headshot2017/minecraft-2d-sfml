#include "particle.h"

Particle::Particle(World *world, GameEngine *engine, float _x, float _y) : Entity(world, engine)
{
    m_entityid = ENT_PARTICLE;
    x = _x;
    y = _y;
}

Particle::Particle(World *world, GameEngine *engine, sf::Vector2f pos) : Entity(world, engine)
{
    m_entityid = ENT_PARTICLE;
    x = pos.x;
    y = pos.y;
}
