#include "world.h"
#include <fstream>
#include <stdlib.h>
#include <math.h>

World::World()
{
    m_blocks2.resize(WORLD_W*WORLD_H*4);
    m_blocks2.setPrimitiveType(sf::Quads);
}

void World::setBlock(int x, int y, int block)
{
    int ind = (y * WORLD_W + x)*4;
    if (x < 0 or x >= WORLD_W or y < 0 or y >= WORLD_H) return;

    //printf("set block at %d,%d with ind %d to %d\n", x, y, ind, block);

    m_blocks2[ind+0].position = sf::Vector2f((x+0)*32, (y+0)*32);
    m_blocks2[ind+1].position = sf::Vector2f((x+1)*32, (y+0)*32);
    m_blocks2[ind+2].position = sf::Vector2f((x+1)*32, (y+1)*32);
    m_blocks2[ind+3].position = sf::Vector2f((x+0)*32, (y+1)*32);

    m_blocks2[ind+0].texCoords = sf::Vector2f(block*32,0);
    m_blocks2[ind+1].texCoords = sf::Vector2f(block*32+32,0);
    m_blocks2[ind+2].texCoords = sf::Vector2f(block*32+32,32);
    m_blocks2[ind+3].texCoords = sf::Vector2f(block*32,32);
}

int World::getBlock(int x, int y)
{
    int ind = (y * WORLD_W + x)*4;
    int block = m_blocks2[ind].texCoords.x / 32;

    return block;
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

    int total = WORLD_W*WORLD_H*4;
    file.write((char*)&total, sizeof(total));
    for(int i=0; i<total; i++)
    {
        int x = i % WORLD_W;
        int y = i / WORLD_W;
        if (x < 0 or x >= WORLD_W or y < 0 or y >= WORLD_H) continue;
        int block = getBlock(x,y);

        file.write((char*)&x, sizeof(x));
        file.write((char*)&y, sizeof(x));
        file.write((char*)&block, sizeof(block));
    }
    file.close();
    printf("world saved to %s\n", fileName);
}

void World::loadWorld(const char *worldName)
{
    sprintf(fileName, "worlds/%s.dat", worldName);
    std::fstream file(fileName, std::fstream::in | std::fstream::binary);

    m_blocks2.clear();

    printf("loading world %s...\n", fileName);

    int worldTotal, pos_x, pos_y, block_type;

    printf("read worldtotal\n");
    file.read((char*)&worldTotal, sizeof(worldTotal));
    printf("got %d\n", worldTotal);
    m_blocks2.resize(worldTotal);
    for(int i=0; i<worldTotal; i++)
    {
        //printf("read %d\n", i);
        file.read((char*)&pos_x, sizeof(pos_x));
        file.read((char*)&pos_y, sizeof(pos_y));
        file.read((char*)&block_type, sizeof(block_type));

        setBlock(pos_x, pos_y, block_type);
    }
    printf("world loaded!\n");
}
