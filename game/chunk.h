#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "../game_engine.h"
#include "block.h"
#include "world.h"

class Chunk
{
public:
    Chunk();
    Chunk(GameEngine *engine);

    void setBlock(int x, int y, int block, int layer);
    int getBlock(int x, int y);
    int getBlockLayer(int x, int y);
    int getBlockFlags(int x, int y);
    int getBlockDir(int x, int y);

    sf::VertexArray& getVertex() {return m_blocks;}

private:
    sf::VertexArray m_blocks;
    Block m_blockflags[CHUNK_W*CHUNK_H];
    GameEngine *m_engine;
};

#endif // CHUNK_H_INCLUDED
