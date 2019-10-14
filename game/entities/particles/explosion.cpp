#include "explosion.h"

ExplosionParticle::ExplosionParticle(World *world, GameEngine *engine, float x, float y) : Particle(world, engine, x, y)
{
    m_sprite = AnimatedSprite(engine, engine->m_explosion, x-16, y-16, 32, 32, 16, 1, false);
    m_sprite.setScale(2);
    sf::Color randoms[3] = {sf::Color(128,128,128), sf::Color(192,192,192), sf::Color::White};
    m_sprite.setColor(randoms[rand() % 3]);
}

ExplosionParticle::ExplosionParticle(World *world, GameEngine *engine, sf::Vector2f pos) : Particle(world, engine, pos)
{
    m_sprite = AnimatedSprite(engine, engine->m_explosion, pos.x-16, pos.y-16, 32, 32, 16, 1, false);
    m_sprite.setScale(2);
    sf::Color randoms[3] = {sf::Color(128,128,128), sf::Color(192,192,192), sf::Color::White};
    m_sprite.setColor(randoms[rand() % 3]);
}

sf::Vector2u ExplosionParticle::getSize()
{
    return m_engine->m_explosion.getSize();
}

void ExplosionParticle::update()
{
    m_sprite.update();
    if (m_sprite.isFinished())
        m_destroy = true;
}

void ExplosionParticle::draw()
{
    m_sprite.draw();
}
