#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#define WORLD_W 2048
#define WORLD_H 192
#define CHUNK_W 25 // * 32 = 800
#define CHUNK_H 15 // * 32 = 480

#include <SFML/Graphics.hpp>
#include "../game_engine.h"
#include <fstream>
#include <vector>

class World
{
public:
    World();

    void setBlock(int x, int y, int block);
    int getBlock(int x, int y);

    void generateWorld(unsigned int seed, const char *name);
    void loadWorld(const char *worldName);
    void saveWorld();

    sf::VertexArray& getBlocksFromPoint(int x, int y);

private:
    char fileName[96];
    std::vector<std::vector<sf::VertexArray>> m_blocks2;
};

#endif // WORLD_H_INCLUDED