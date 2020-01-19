#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#define WORLD_W 2048
#define WORLD_H 192
#define CHUNK_W 16
#define CHUNK_H 16

#include <SFML/Graphics.hpp>
#include "../game_engine.h"
#include "chunk.h"
#include "entity.h"
#include "player.h"
#include <vector>

class Chunk;
class Entity;
class Player;

class World
{
public:
    World();
    World(GameEngine *engine);
    ~World();

    void setBlock(int x, int y, int block, int layer=LAYER_BUILD);
    int getBlock(int x, int y);
    int getBlockLayer(int x, int y);
    int getBlockFlags(int x, int y);

    void updateLighting(int x, int y);

    void addEntity(Entity* ent);
    void updateEntities();
    void drawEntities();
    std::vector<Entity*> getNearestEntities(float x, float y, int type, int radius);

    void generateWorld(unsigned int seed, const char *name, int biome=-1);
    void generateFlatWorld(const char *name, const std::vector<int>& blocks);
    void loadWorld(const char *worldName);
    void saveWorld();

    Player* getPlayer() {return m_player;}
    sf::VertexArray& getBlocksFromPoint(int x, int y, bool front=false);
    std::vector<std::vector<Chunk>>* getBlocks() {return &m_blocks2;}

private:
    bool loaded;
    char fileName[96];
    unsigned int m_seed;
    GameEngine *m_engine;

    Player* m_player; // will later be changed to vectors for multiplayer
    std::vector<std::vector<Chunk>> m_blocks2;
    std::vector<Entity*> m_entities;
};

#endif // WORLD_H_INCLUDED
