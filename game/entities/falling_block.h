#ifndef FALLING_BLOCK_H_INCLUDED
#define FALLING_BLOCK_H_INCLUDED

#include "../entity.h"
#include <SFML/Graphics.hpp>

class FallingBlock : public Entity
{
public:
    FallingBlock() {}
    FallingBlock(World *world, GameEngine *engine, int xx, int yy, int block);

    bool groundCollide();
    void update();
    void draw();

private:
    sf::VertexArray m_blockarray;
};

#endif // FALLING_BLOCK_H_INCLUDED
