#include "entity.h"

Entity::Entity(World *world, GameEngine *engine)
{
    m_world = world;
    m_engine = engine;
    x = y = hspeed = vspeed = x_acc = gravity = 0;
}

bool Entity::blockCollide(int x, int y)
{
    return m_world->getBlock(x, y) != BLOCK_AIR and m_world->getBlockLayer(x, y) == LAYER_BUILD;
}
