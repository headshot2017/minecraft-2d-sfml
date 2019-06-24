#include "world.h"
#include <fstream>
#include <stdlib.h>
#include <math.h>

World::World()
{
    for(int yy=0; yy < WORLD_H/CHUNK_H+1; yy++)
    {
        m_blocks2.push_back(std::vector<Chunk>());
        for (int xx=0; xx < WORLD_W/CHUNK_W+1; xx++)
        {
            m_blocks2.back().push_back(Chunk());
        }
    }
}

void World::setBlock(int x, int y, int block, int layer)
{
    //int ind = (y * WORLD_W + x)*4;
    int x_ind = x/CHUNK_W;
    int y_ind = y/CHUNK_H;
    int x_block_chunk = x % CHUNK_W;
    int y_block_chunk = y % CHUNK_H;
    int ind = (y_block_chunk * CHUNK_W + x_block_chunk)*4;

    if (x < 0 or x >= WORLD_W or y < 0 or y >= WORLD_H) return;

    //printf("set block at %d,%d with ind %d to %d\n", x, y, ind, block);

    m_blocks2[y_ind][x_ind].setBlock(x, y, ind, block, layer);
}

int World::getBlock(int x, int y)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= WORLD_W) x = WORLD_W-1;
    if (y >= WORLD_H) y = WORLD_H-1;

    int x_ind = x/CHUNK_W;
    int y_ind = y/CHUNK_H;
    int x_block_chunk = x % CHUNK_W;
    int y_block_chunk = y % CHUNK_H;
    int ind = (y_block_chunk * CHUNK_W + x_block_chunk)*4;

    int block = m_blocks2[y_ind][x_ind].getBlock(ind);
    return block;
}

int World::getBlockLayer(int x, int y)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= WORLD_W) x = WORLD_W-1;
    if (y >= WORLD_H) y = WORLD_H-1;

    int x_ind = x/CHUNK_W;
    int y_ind = y/CHUNK_H;
    int x_block_chunk = x % CHUNK_W;
    int y_block_chunk = y % CHUNK_H;
    int ind = (y_block_chunk * CHUNK_W + x_block_chunk)*4;

    int block = m_blocks2[y_ind][x_ind].getBlockLayer(ind);
    return block;
}

sf::VertexArray& World::getBlocksFromPoint(int x, int y)
{
    int x_ind = x/CHUNK_W;
    int y_ind = y/CHUNK_H;

    if (x_ind > WORLD_W/CHUNK_W) x_ind = WORLD_W/CHUNK_W;
    if (y_ind > WORLD_H/CHUNK_H) y_ind = WORLD_H/CHUNK_H;
    return m_blocks2[y_ind][x_ind].getVertex();
}

void World::generateWorld(unsigned int seed, const char *name)
{
    srand(seed);
    sprintf(fileName, "worlds/%s.dat", name);

    double pi = 3.141592653589793;
    int heights[] = {58,59,60,61,62,63,64,65,66,67,68,69,70};
    int randomDirtLevel[] = {2,3,3,3};
    int startingHeight = heights[rand() % 13]*32;
    int alternateHeight = startingHeight;
    int dirtLevel, stoneLevel, altDirtLvl;

    for(int xx=0; xx<WORLD_W*32; xx+=32)
    {
        dirtLevel = alternateHeight + 32*randomDirtLevel[rand() % 4];
        stoneLevel = WORLD_H*32;

        setBlock(xx/32, alternateHeight/32, BLOCK_GRASS);

        for(int yy=alternateHeight; yy<dirtLevel; yy+=32)
        {
            setBlock(xx/32, yy/32+1, BLOCK_DIRT);
            altDirtLvl = yy;
        }

        for(int yy=altDirtLvl; yy<altDirtLvl+64; yy+=32)
        {
            int u = rand() % 4;
            if (u == 1)
                setBlock(xx/32, yy/32+2, BLOCK_STONE);
            else
                setBlock(xx/32, yy/32+2, BLOCK_DIRT);
        }

        //stoneLevel
        for(int yy=altDirtLvl+64; yy<stoneLevel; yy+=32)
        {
            if (yy/32+2 < WORLD_H-2)
                setBlock(xx/32, yy/32+2, BLOCK_STONE);
            else
                setBlock(xx/32, yy/32+2, BLOCK_BEDROCK);
        }

        int amp = rand() % 13;
        alternateHeight += int((amp*sin((2*pi)/1728*(xx-864))));
    }

    saveWorld();
    printf("completed!\n");
}

void World::saveWorld()
{
    printf("opening file\n");
    std::fstream file(fileName, std::ios::out | std::ios::binary);

    int width = WORLD_W, height = WORLD_H;
    file.write((char*)&width, sizeof(width));
    file.write((char*)&height, sizeof(height));
    for(int i=0; i<width*height*4; i++)
    {
        int x = i % width;
        int y = i / width;
        if (x < 0 or x >= width or y < 0 or y >= height) continue;
        int block = getBlock(x,y);
        int layer = getBlockLayer(x,y);

        file.write((char*)&x, sizeof(x));
        file.write((char*)&y, sizeof(x));
        file.write((char*)&block, sizeof(block));
        file.write((char*)&layer, sizeof(layer));
    }
    file.close();
    printf("world saved to %s\n", fileName);
}

void World::loadWorld(const char *worldName)
{
    sprintf(fileName, "worlds/%s.dat", worldName);
    std::fstream file(fileName, std::ios::in | std::ios::binary);

    m_blocks2.clear();

    printf("loading world %s...\n", fileName);

    int width, height, pos_x, pos_y, block_type, block_layer;

    file.read((char*)&width, sizeof(width));
    file.read((char*)&height, sizeof(height));
    printf("world size: %d,%d\n", width, height);

    for(int yy=0; yy < height/CHUNK_H+1; yy++)
    {
        m_blocks2.push_back(std::vector<Chunk>());
        for (int xx=0; xx < width/CHUNK_W+1; xx++)
        {
            m_blocks2.back().push_back(Chunk());
        }
    }

    for(int i=0; i<width*height*4; i++)
    {
        //printf("read %d\n", i);
        file.read((char*)&pos_x, sizeof(pos_x));
        file.read((char*)&pos_y, sizeof(pos_y));
        file.read((char*)&block_type, sizeof(block_type));
        file.read((char*)&block_layer, sizeof(block_layer));

        setBlock(pos_x, pos_y, block_type, block_layer);
    }
    printf("world loaded!\n");
}
