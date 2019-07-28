#include "tnt.h"

Dynamite::Dynamite(World *world, GameEngine *engine, int xx, int yy, int startingticks) : FallingBlock(world, engine, xx, yy, BLOCK_TNT)
{
    m_entityid = ENT_TNT;
    m_ticksleft = startingticks;
    m_whiteFlash = false;

    hspeed = -2 + (rand() % 5);
    vspeed = -(rand() % 3);
}

void Dynamite::update()
{
    FallingBlock::update();

    if (m_ticksleft % 15 == 0)
        m_whiteFlash = not m_whiteFlash;
    m_ticksleft--;
}

void Dynamite::draw()
{
    if (m_whiteFlash)
        m_engine->m_window.draw(m_blockarray);
    else
        m_engine->m_window.draw(m_blockarray, &m_engine->m_blocks);
}
