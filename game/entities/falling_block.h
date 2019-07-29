#ifndef FALLING_BLOCK_H_INCLUDED
#define FALLING_BLOCK_H_INCLUDED

#include "../entity.h"
#include <SFML/Graphics.hpp>

class FallingBlock : public Entity
{
public:
    FallingBlock() {}
    FallingBlock(World *world, GameEngine *engine, int xx, int yy, int block);

    sf::Vector2u getSize() {return sf::Vector2u(32, 32);}
    bool groundCollide();
    virtual void update();
    virtual void draw();

protected:
    sf::VertexArray m_blockarray;
};

#endif // FALLING_BLOCK_H_INCLUDED
