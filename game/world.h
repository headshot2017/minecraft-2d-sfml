#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#define WORLD_W 512
#define WORLD_H 144

#include <SFML/Graphics.hpp>
#include "../game_engine.h"
#include <fstream>

class World
{
public:
    World();

    void setBlock(int x, int y, int block);
    int getBlock(int x, int y);

    void generateWorld(unsigned int seed, const char *name);
    void loadWorld(const char *worldName);
    void saveWorld();
    sf::VertexArray& getBlocks() {return m_blocks2;}

private:
    char fileName[96];
    sf::VertexArray m_blocks2;
};

#endif // WORLD_H_INCLUDED
