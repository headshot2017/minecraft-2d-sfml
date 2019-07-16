#include "falling_block.h"

FallingBlock::FallingBlock(World *world, GameEngine *engine, int xx, int yy, int block) : Entity(world, engine)
{
    m_block = block;
    x = xx*32;
    y = yy*32;

    m_entityid = ENT_FALLINGBLOCK;

    m_blockarray.setPrimitiveType(sf::Quads);
    m_blockarray.resize(4);

    m_blockarray[0].position = sf::Vector2f((xx+0)*32, (yy+0)*32);
    m_blockarray[1].position = sf::Vector2f((xx+1)*32, (yy+0)*32);
    m_blockarray[2].position = sf::Vector2f((xx+1)*32, (yy+1)*32);
    m_blockarray[3].position = sf::Vector2f((xx+0)*32, (yy+1)*32);

    m_blockarray[0].texCoords = sf::Vector2f(block*32,0);
    m_blockarray[1].texCoords = sf::Vector2f(block*32+32,0);
    m_blockarray[2].texCoords = sf::Vector2f(block*32+32,32);
    m_blockarray[3].texCoords = sf::Vector2f(block*32,32);
}

bool FallingBlock::groundCollide()
{
    return blockCollide((x+hspeed+16)/32, (y+vspeed+32)/32) or blockCollide((x+hspeed+16)/32, (y+vspeed)/32);
}

void FallingBlock::update()
{
    if (groundCollide()) // gravity.
    {
        gravity = vspeed = 0;
        if (hspeed > 0)
            x_acc = -0.25f;
        else if (hspeed < 0)
            x_acc = 0.25f;
        else
            x_acc = 0;
    }
    else
        gravity = 0.25f;

    hspeed += x_acc;
    vspeed += gravity;

    x += hspeed;
    y += vspeed;

    m_blockarray[0].position = sf::Vector2f(x+00, y+00);
    m_blockarray[1].position = sf::Vector2f(x+32, y+00);
    m_blockarray[2].position = sf::Vector2f(x+32, y+32);
    m_blockarray[3].position = sf::Vector2f(x+00, y+32);
}

void FallingBlock::draw()
{
    m_engine->m_window.draw(m_blockarray, &m_engine->m_blocks);
}
