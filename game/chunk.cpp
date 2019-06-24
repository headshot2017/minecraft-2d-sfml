#include "chunk.h"

Chunk::Chunk()
{
    m_blocks.setPrimitiveType(sf::Quads);
    m_blocks.resize(CHUNK_W*CHUNK_H*4);
}

void Chunk::setBlock(int x, int y, int ind, int block, int layer)
{
    m_blocks[ind+0].position = sf::Vector2f((x+0)*32, (y+0)*32);
    m_blocks[ind+1].position = sf::Vector2f((x+1)*32, (y+0)*32);
    m_blocks[ind+2].position = sf::Vector2f((x+1)*32, (y+1)*32);
    m_blocks[ind+3].position = sf::Vector2f((x+0)*32, (y+1)*32);

    m_blocks[ind+0].texCoords = sf::Vector2f(block*32,0);
    m_blocks[ind+1].texCoords = sf::Vector2f(block*32+32,0);
    m_blocks[ind+2].texCoords = sf::Vector2f(block*32+32,32);
    m_blocks[ind+3].texCoords = sf::Vector2f(block*32,32);

    m_blocks[ind+0].color = (layer != LAYER_DECORATION) ? sf::Color(255, 255, 255) : sf::Color(128, 128, 128);
    m_blocks[ind+1].color = (layer != LAYER_DECORATION) ? sf::Color(255, 255, 255) : sf::Color(128, 128, 128);
    m_blocks[ind+2].color = (layer != LAYER_DECORATION) ? sf::Color(255, 255, 255) : sf::Color(128, 128, 128);
    m_blocks[ind+3].color = (layer != LAYER_DECORATION) ? sf::Color(255, 255, 255) : sf::Color(128, 128, 128);

    m_blockflags[ind/4].m_blockflags = 0;
    m_blockflags[ind/4].setDir(1);
    m_blockflags[ind/4].setLayer(layer);
}

int Chunk::getBlock(int ind)
{
    int block = m_blocks[ind].texCoords.x / 32;
    return block;
}

int Chunk::getBlockLayer(int ind)
{
    int block = m_blockflags[ind/4].getLayer();
    return block;
}

int Chunk::getBlockFlags(int ind)
{
    return m_blockflags[ind/4].m_blockflags;
}

int Chunk::getBlockDir(int ind)
{
    return m_blockflags[ind/4].getDir();
}
