#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#define WORLD_W 2048
#define WORLD_H 192
#define CHUNK_W 24
#define CHUNK_H 16

#include <SFML/Graphics.hpp>
#include "../game_engine.h"
#include "chunk.h"
#include "entity.h"
#include <vector>

class Chunk;
class Entity;

class World
{
public:
    World();
    World(GameEngine *engine);
    ~World();

    void setBlock(int x, int y, int block, int layer=LAYER_BUILD);
    int getBlock(int x, int y);
    int getBlockLayer(int x, int y);

    void updateLighting(int x, int y);

    void addEntity(Entity* ent);
    void updateEntities();
    void drawEntities();
    std::vector<Entity*> getNearestEntities(float x, float y, int type, int radius);

    void generateWorld(unsigned int seed, const char *name);
    void loadWorld(const char *worldName);
    void saveWorld();

    sf::VertexArray& getBlocksFromPoint(int x, int y);

private:
    bool loaded;
    char fileName[96];
    GameEngine *m_engine;
    std::vector<std::vector<Chunk>> m_blocks2;
    std::vector<Entity*> m_entities;
};

#endif // WORLD_H_INCLUDED
