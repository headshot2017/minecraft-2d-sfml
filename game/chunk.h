#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "block.h"
#include "world.h"

class Chunk
{
public:
    Chunk();

    void setBlock(int x, int y, int ind, int block, int layer);
    int getBlock(int ind);
    int getBlockLayer(int ind);
    int getBlockFlags(int ind);
    int getBlockDir(int ind);

    sf::VertexArray& getVertex() {return m_blocks;}

private:
    sf::VertexArray m_blocks;
    Block m_blockflags[CHUNK_W*CHUNK_H];
};

#endif // CHUNK_H_INCLUDED
