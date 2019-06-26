#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#define WORLD_W 2048
#define WORLD_H 192
#define CHUNK_W 25 // * 32 = 800
#define CHUNK_H 15 // * 32 = 480

#include <SFML/Graphics.hpp>
#include "../game_engine.h"
#include "chunk.h"
#include <vector>

class Chunk;

class World
{
public:
    World();
    World(GameEngine *engine);
    ~World();

    void setBlock(int x, int y, int block, int layer=LAYER_BUILD);
    int getBlock(int x, int y);
    int getBlockLayer(int x, int y);

    void generateWorld(unsigned int seed, const char *name);
    void loadWorld(const char *worldName);
    void saveWorld();

    sf::VertexArray& getBlocksFromPoint(int x, int y);

private:
    char fileName[96];
    GameEngine *m_engine;
    std::vector<std::vector<Chunk>> m_blocks2;
};

#endif // WORLD_H_INCLUDED
