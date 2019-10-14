#include "entity.h"
#include "math.h"

Entity::Entity(World *world, GameEngine *engine)
{
    m_world = world;
    m_engine = engine;
    x = y = hspeed = vspeed = x_acc = gravity = 0;
    m_ticks = m_ticksleft = 0;
    m_destroy = false;
}

bool Entity::blockCollide(int x, int y)
{
    return m_world->getBlock(x, y) != BLOCK_AIR and m_world->getBlockLayer(x, y) == LAYER_BUILD;
}

void Entity::knockBack(float xx, float yy, int maxdist)
{
    float dist;
    dist = sqrt(pow(xx - x, 2) + pow(yy - y, 2));
    if (dist < maxdist)
    {
        int force = abs(maxdist-dist)/4;
        float angle = atan2(y - yy, x - xx);
        hspeed += round(force * cos(angle));
        vspeed += round(force * sin(angle));
    }
}
