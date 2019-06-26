#include "chunk.h"

Chunk::Chunk()
{
    m_blocks.setPrimitiveType(sf::Quads);
    //m_blocks.resize(CHUNK_W*CHUNK_H*4);
}

Chunk::Chunk(GameEngine *engine)
{
    m_engine = engine;
    m_blocks.setPrimitiveType(sf::Quads);
    m_blocks.resize(CHUNK_W*CHUNK_H*4);
}

void Chunk::setBlock(int x, int y, int block, int layer)
{
    int x_block_chunk = x % CHUNK_W;
    int y_block_chunk = y % CHUNK_H;
    int ind = (y_block_chunk * CHUNK_W + x_block_chunk)*4;

    m_blocks[ind+0].position = sf::Vector2f((x+0)*32, (y+0)*32);
    m_blocks[ind+1].position = sf::Vector2f((x+1)*32, (y+0)*32);
    m_blocks[ind+2].position = sf::Vector2f((x+1)*32, (y+1)*32);
    m_blocks[ind+3].position = sf::Vector2f((x+0)*32, (y+1)*32);

    m_blocks[ind+0].texCoords = sf::Vector2f(block*32,0);
    m_blocks[ind+1].texCoords = sf::Vector2f(block*32+32,0);
    m_blocks[ind+2].texCoords = sf::Vector2f(block*32+32,32);
    m_blocks[ind+3].texCoords = sf::Vector2f(block*32,32);

    m_blocks[ind+0].color = sf::Color(255, 255, 255);
    m_blocks[ind+1].color = sf::Color(255, 255, 255);
    m_blocks[ind+2].color = sf::Color(255, 255, 255);
    m_blocks[ind+3].color = sf::Color(255, 255, 255);

    m_blockflags[ind/4].m_blockflags = 0;
    m_blockflags[ind/4].setDir(1);
    m_blockflags[ind/4].setLayer(layer);

    if (not m_engine->Settings()->m_layerlighting)
    {
        m_blocks[ind+0].color = (layer != LAYER_DECORATION) ? sf::Color(255, 255, 255) : sf::Color(192, 192, 192);
        m_blocks[ind+1].color = (layer != LAYER_DECORATION) ? sf::Color(255, 255, 255) : sf::Color(192, 192, 192);
        m_blocks[ind+2].color = (layer != LAYER_DECORATION) ? sf::Color(255, 255, 255) : sf::Color(192, 192, 192);
        m_blocks[ind+3].color = (layer != LAYER_DECORATION) ? sf::Color(255, 255, 255) : sf::Color(192, 192, 192);
    }
    else
    {
        if (layer == LAYER_BUILD)
        {
            if (x_block_chunk > 0)
            {
                int other = (y_block_chunk * CHUNK_W + (x_block_chunk-1))*4;
                if (getBlock(x_block_chunk, y_block_chunk) != BLOCK_AIR and getBlockLayer(x_block_chunk-1, y_block_chunk) == LAYER_DECORATION)
                {
                    m_blocks[other+1].color = sf::Color(192, 192, 192);
                    m_blocks[other+2].color = sf::Color(192, 192, 192);
                }
                else
                {
                    m_blocks[other+1].color = sf::Color(255, 255, 255);
                    m_blocks[other+2].color = sf::Color(255, 255, 255);
                }
            }

            if (x_block_chunk < CHUNK_W-1)
            {
                int other = (y_block_chunk * CHUNK_W + (x_block_chunk+1))*4;
                if (getBlock(x_block_chunk, y_block_chunk) != BLOCK_AIR and getBlockLayer(x_block_chunk+1, y_block_chunk) == LAYER_DECORATION)
                {
                    m_blocks[other+0].color = sf::Color(192, 192, 192);
                    m_blocks[other+3].color = sf::Color(192, 192, 192);
                }
                else
                {
                    m_blocks[other+0].color = sf::Color(255, 255, 255);
                    m_blocks[other+3].color = sf::Color(255, 255, 255);
                }
            }

            if (y_block_chunk > 0)
            {
                int other = ((y_block_chunk-1) * CHUNK_W + x_block_chunk)*4;
                if (getBlock(x_block_chunk, y_block_chunk) != BLOCK_AIR and getBlockLayer(x_block_chunk, y_block_chunk-1) == LAYER_DECORATION)
                {
                    m_blocks[other+2].color = sf::Color(192, 192, 192);
                    m_blocks[other+3].color = sf::Color(192, 192, 192);
                }
                else
                {
                    m_blocks[other+2].color = sf::Color(255, 255, 255);
                    m_blocks[other+3].color = sf::Color(255, 255, 255);
                }
            }

            if (y_block_chunk < CHUNK_H-1)
            {
                int other = ((y_block_chunk+1) * CHUNK_W + x_block_chunk)*4;
                if (getBlock(x_block_chunk, y_block_chunk) != BLOCK_AIR and getBlockLayer(x_block_chunk, y_block_chunk+1) == LAYER_DECORATION)
                {
                    m_blocks[other+0].color = sf::Color(192, 192, 192);
                    m_blocks[other+1].color = sf::Color(192, 192, 192);
                }
                else
                {
                    m_blocks[other+0].color = sf::Color(255, 255, 255);
                    m_blocks[other+1].color = sf::Color(255, 255, 255);
                }
            }
        }

        else if (layer == LAYER_DECORATION)
        {
            if (x_block_chunk > 0)
            {
                if (getBlock(x_block_chunk-1, y_block_chunk) != BLOCK_AIR and getBlockLayer(x_block_chunk-1, y_block_chunk) == LAYER_BUILD)
                {
                    m_blocks[ind+0].color = sf::Color(192, 192, 192);
                    m_blocks[ind+3].color = sf::Color(192, 192, 192);
                }
                else
                {
                    m_blocks[ind+0].color = sf::Color(255, 255, 255);
                    m_blocks[ind+3].color = sf::Color(255, 255, 255);
                }
            }

            if (x_block_chunk < CHUNK_W-1)
            {
                if (getBlock(x_block_chunk+1, y_block_chunk) != BLOCK_AIR and getBlockLayer(x_block_chunk+1, y_block_chunk) == LAYER_BUILD)
                {
                    m_blocks[ind+1].color = sf::Color(192, 192, 192);
                    m_blocks[ind+2].color = sf::Color(192, 192, 192);
                }
                else
                {
                    m_blocks[ind+1].color = sf::Color(255, 255, 255);
                    m_blocks[ind+2].color = sf::Color(255, 255, 255);
                }
            }

            if (y_block_chunk > 0)
            {
                if (getBlock(x_block_chunk, y_block_chunk-1) != BLOCK_AIR and getBlockLayer(x_block_chunk, y_block_chunk-1) == LAYER_BUILD)
                {
                    m_blocks[ind+0].color = sf::Color(192, 192, 192);
                    m_blocks[ind+1].color = sf::Color(192, 192, 192);
                }
                else
                {
                    m_blocks[ind+0].color = sf::Color(255, 255, 255);
                    m_blocks[ind+1].color = sf::Color(255, 255, 255);
                }
            }

            if (y_block_chunk < CHUNK_H-1)
            {
                if (getBlock(x_block_chunk, y_block_chunk+1) != BLOCK_AIR and getBlockLayer(x_block_chunk, y_block_chunk+1) == LAYER_BUILD)
                {
                    m_blocks[ind+2].color = sf::Color(192, 192, 192);
                    m_blocks[ind+3].color = sf::Color(192, 192, 192);
                }
                else
                {
                    m_blocks[ind+2].color = sf::Color(255, 255, 255);
                    m_blocks[ind+3].color = sf::Color(255, 255, 255);
                }
            }
        }
    }
}

int Chunk::getBlock(int x, int y)
{
    int ind = (y * CHUNK_W + x)*4;
    int block = m_blocks[ind].texCoords.x / 32;
    return block;
}

int Chunk::getBlockLayer(int x, int y)
{
    int ind = (y * CHUNK_W + x)*4;
    int block = m_blockflags[ind/4].getLayer();
    return block;
}

int Chunk::getBlockFlags(int x, int y)
{
    int ind = (y * CHUNK_W + x)*4;
    return m_blockflags[ind/4].m_blockflags;
}

int Chunk::getBlockDir(int x, int y)
{
    int ind = (y * CHUNK_W + x)*4;
    return m_blockflags[ind/4].getDir();
}
