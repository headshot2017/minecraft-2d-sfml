#ifndef EXPLOSION_H_INCLUDED
#define EXPLOSION_H_INCLUDED

#include "../particle.h"
#include "../../../animated_sprite.h"

class ExplosionParticle : public Particle
{
public:
    ExplosionParticle() {}
    ExplosionParticle(World *world, GameEngine *engine, float x, float y);
    ExplosionParticle(World *world, GameEngine *engine, sf::Vector2f pos);

    sf::Vector2u getSize();
    bool groundCollide() {return false;}
    void update();
    void draw();

private:
    AnimatedSprite m_sprite;
};

#endif // EXPLOSION_H_INCLUDED
